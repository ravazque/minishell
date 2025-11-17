/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_loop.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 22:34:53 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/06 12:43:28 by ravazque         ###   ########.fr       */
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

char	*process_char_exp(char *res, const char *s, int *i, t_normi_ctx *ctx)
{
	char	*val;

	if (should_expand_tilde(s, *i, ctx->exp))
	{
		val = process_tilde_expansion(i, ctx->mini);
		if (!val)
			return (free(res), NULL);
		res = str_cat(res, val);
		free(val);
	}
	else if (should_expand_var(s, *i, ctx->exp))
		res = handle_expansion(res, s, i, ctx->mini);
	else
	{
		res = str_cat_chr(res, s[*i]);
		(*i)++;
	}
	return (res);
}
