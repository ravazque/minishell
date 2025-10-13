/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 20:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/13 21:07:53 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	count_commands(t_cmd *cmds)
{
	int		count;
	t_cmd	*curr;

	count = 0;
	curr = cmds;
	while (curr)
	{
		count++;
		curr = curr->next;
	}
	return (count);
}

static int	is_parent_builtin(t_cmd *cmd)
{
	if (!cmd || !cmd->tokens || !cmd->tokens[0])
		return (0);
	if (ft_strcmp(cmd->tokens[0], "exit") == 0)
		return (1);
	if (ft_strcmp(cmd->tokens[0], "cd") == 0)
		return (1);
	if (ft_strcmp(cmd->tokens[0], "export") == 0)
		return (1);
	if (ft_strcmp(cmd->tokens[0], "unset") == 0)
		return (1);
	return (0);
}

void	executor(t_mini *mini)
{
	int	cmd_count;

	if (!mini || !mini->cmds)
		return ;
	
	// // Procesar heredocs primero
	// if (handle_heredocs(mini) != 0)
	// {
	// 	mini->exit_sts = 1;
	// 	return ;
	// }

	cmd_count = count_commands(mini->cmds);
	if (cmd_count == 1)
	{
		if (is_parent_builtin(mini->cmds))
			return ;
		// execute_single_cmd(mini, mini->cmds);
		return ;
	}
	// execute_pipeline(mini);
}
