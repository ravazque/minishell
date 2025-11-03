/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 18:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/03 18:00:00 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	get_expand_flag(t_token_part *part)
{
	if (!part->is_squote)
		return (1);
	return (0);
}

char	*expand_part(t_token_part *curr, t_mini *mini)
{
	int		exp;
	char	*exp_part;

	exp = get_expand_flag(curr);
	exp_part = exp_str_part(curr->content, mini, exp);
	return (exp_part);
}

char	*exp_parts_list(t_token *tok, t_mini *mini)
{
	char			*res;
	char			*exp_part;
	t_token_part	*curr;

	res = NULL;
	curr = tok->parts;
	while (curr)
	{
		exp_part = expand_part(curr, mini);
		if (!exp_part)
		{
			if (res)
				free(res);
			return (NULL);
		}
		res = str_cat(res, exp_part);
		free(exp_part);
		if (!res)
			return (NULL);
		curr = curr->next;
	}
	return (res);
}

char	*exp_tok_parts(t_token *tok, t_mini *mini)
{
	int		exp;

	if (!tok)
		return (ft_strdup(""));
	if (!tok->parts)
	{
		exp = 0;
		if (!tok->is_squote)
			exp = 1;
		return (exp_str_part(tok->raw, mini, exp));
	}
	return (exp_parts_list(tok, mini));
}
