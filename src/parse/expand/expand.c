/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 14:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/03 18:29:35 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static char	*process_var_expansion(const char *s, int *i, t_mini *mini)
{
	char	*var;
	char	*val;
	int		vend;

	var = extract_var(s, *i + 1, &vend);
	if (!var)
		return (NULL);
	val = expand_var(var, mini);
	free(var);
	*i = vend;
	if (!val)
		return (ft_strdup(""));
	return (val);
}

static int	should_expand_var(const char *s, int i, int exp)
{
	if (s[i] != '$')
		return (0);
	if (!exp)
		return (0);
	if (!s[i + 1])
		return (0);
	if (!is_valid_var_chr(s[i + 1]) && s[i + 1] != '?'
		&& s[i + 1] != '$' && s[i + 1] != '0')
		return (0);
	return (1);
}

static char	*handle_expansion(char *res, const char *s, int *i, t_mini *mini)
{
	char	*val;

	val = process_var_expansion(s, i, mini);
	if (!val)
	{
		if (res)
			free(res);
		return (NULL);
	}
	res = str_cat(res, val);
	free(val);
	return (res);
}

char	*exp_str_part(const char *s, t_mini *mini, int exp)
{
	char	*res;
	int		i;

	res = NULL;
	i = 0;
	while (s[i])
	{
		if (should_expand_var(s, i, exp))
		{
			res = handle_expansion(res, s, &i, mini);
			if (!res)
				return (NULL);
		}
		else
		{
			res = str_cat_chr(res, s[i]);
			if (!res)
				return (NULL);
			i++;
		}
	}
	if (!res)
		return (ft_strdup(""));
	return (res);
}

int	expander(t_mini *mini)
{
	t_cmd	*curr;

	if (!mini || !mini->cmds)
		return (0);
	curr = mini->cmds;
	while (curr)
	{
		if (exp_cmd_toks_with_split(curr, mini))
			return (1);
		if (exp_redirs(curr, mini))
			return (1);
		if (curr->tokn)
		{
			free_token_list(curr->tokn);
			curr->tokn = NULL;
		}
		curr = curr->next;
	}
	return (0);
}
