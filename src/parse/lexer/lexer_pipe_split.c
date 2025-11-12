/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_pipe_split.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 00:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/11 00:00:00 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	process_token_group(t_token *curr, t_cmd **cmd_list, int count)
{
	t_cmd	*new_cmd;

	if (count == 0)
		return (0);
	new_cmd = create_cmd_from_tokens(curr, count);
	if (!new_cmd)
	{
		free_cmds(*cmd_list);
		return (1);
	}
	add_cmd_to_lst(cmd_list, new_cmd);
	return (0);
}

static void	cleanup_and_update(t_mini *mini, t_cmd *cmd_list)
{
	free_cmds(mini->cmds);
	mini->cmds = cmd_list;
}

int	split_by_pipes(t_mini *mini)
{
	t_token	*curr;
	t_cmd	*cmd_list;
	int		count;

	if (!mini->cmds || !mini->cmds->tokn)
		return (0);
	if (validate_pipe_syntax(mini->cmds->tokn))
		return (1);
	curr = mini->cmds->tokn;
	cmd_list = NULL;
	while (curr)
	{
		count = count_tokens_until_pipe(curr);
		if (process_token_group(curr, &cmd_list, count))
			return (1);
		curr = skip_to_next_pipe(curr);
	}
	cleanup_and_update(mini, cmd_list);
	return (0);
}
