/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 14:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/06 12:43:28 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

char	*exp_str_part(const char *s, t_mini *mini, int exp)
{
	char		*res;
	int			i;
	t_normi_ctx	ctx;

	ctx.mini = mini;
	ctx.exp = exp;
	res = NULL;
	i = 0;
	while (s[i])
	{
		res = process_char_exp(res, s, &i, &ctx);
		if (!res)
			return (NULL);
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
