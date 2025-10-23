/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_wait.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 12:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/23 15:33:01 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	wait_processes(t_exec *exec, t_mini *mini)
{
	int	i;
	int	status;
	int	last_exit;

	if (!exec || !exec->pids)
		return (0);
	last_exit = 0;
	i = 0;
	ignore_sigint_for_wait();
	while (i < exec->n_cmds)
	{
		waitpid(exec->pids[i], &status, 0);
		if (i == exec->n_cmds - 1)
		{
			if (WIFEXITED(status))
				last_exit = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				last_exit = 128 + WTERMSIG(status);
				if (WTERMSIG(status) == SIGINT)
					write(STDOUT_FILENO, "\n", 1);
			}
		}
		i++;
	}
	setup_interactive_signals();
	mini->exit_sts = last_exit;
	return (0);
}
