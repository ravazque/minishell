/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 17:06:13 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/11 00:00:00 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	finalize_cmd(t_cmd *curr)
{
	if (curr->tokn)
	{
		curr->tokens = toks_to_arr(curr->tokn);
		if (!curr->tokens)
			return (1);
	}
	return (0);
}

static int	finalize_cmds(t_mini *mini)
{
	t_cmd	*curr;

	curr = mini->cmds;
	while (curr)
	{
		if (finalize_cmd(curr))
			return (1);
		curr = curr->next;
	}
	return (0);
}

static int	process_cmds(t_mini *mini)
{
	t_cmd	*curr;

	curr = mini->cmds;
	while (curr)
	{
		mark_assignments(curr);
		if (proc_redirs(curr))
			return (1);
		curr = curr->next;
	}
	return (0);
}

int	lexer(t_mini *mini)
{
	if (!mini || !mini->cmds)
		return (0);
	if (split_by_pipes(mini))
		return (1);
	if (process_cmds(mini))
		return (1);
	if (finalize_cmds(mini))
		return (1);
	return (0);
}
