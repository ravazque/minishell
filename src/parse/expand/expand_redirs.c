/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 18:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/03 18:00:00 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	should_exp_redir(t_redir *redir)
{
	if (redir->in_redir == 2)
		return (0);
	return (1);
}

int	exp_redirs(t_cmd *cmd, t_mini *mini)
{
	t_redir	*curr;
	char	*exp;

	curr = cmd->redirs;
	while (curr)
	{
		if (should_exp_redir(curr))
		{
			exp = exp_str_part(curr->target, mini, 1);
			if (!exp)
				return (1);
			free(curr->target);
			curr->target = exp;
		}
		curr = curr->next;
	}
	return (0);
}
