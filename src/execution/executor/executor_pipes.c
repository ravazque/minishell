/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 12:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/30 18:37:02 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	create_pipes(t_exec *exec)
{
	int	i;

	if (!exec || !exec->pipe_fds)
		return (0);
	i = 0;
	while (i < exec->n_pipes)
	{
		if (pipe(&exec->pipe_fds[i * 2]) == -1)
		{
			while (i > 0)
			{
				i--;
				close(exec->pipe_fds[i * 2]);
				close(exec->pipe_fds[i * 2 + 1]);
			}
			return (1);
		}
		i++;
	}
	return (0);
}

void	close_pipes(t_exec *exec)
{
	int	i;

	if (!exec || !exec->pipe_fds)
		return ;
	i = 0;
	while (i < exec->n_pipes * 2)
	{
		close(exec->pipe_fds[i]);
		i++;
	}
}

void	setup_pipe_fds(t_exec *exec, int cmd_idx)
{
	if (!exec)
		return ;
	if (cmd_idx > 0)
		dup2(exec->pipe_fds[(cmd_idx - 1) * 2], STDIN_FILENO);
	if (cmd_idx < exec->n_cmds - 1)
		dup2(exec->pipe_fds[cmd_idx * 2 + 1], STDOUT_FILENO);
	close_pipes(exec);
}
