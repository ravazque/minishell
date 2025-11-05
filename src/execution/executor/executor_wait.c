/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_wait.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 18:32:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/04 13:07:51 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	handle_exit_status(int status, int *last_exit)
{
	if (WIFEXITED(status))
		*last_exit = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		*last_exit = 128 + WTERMSIG(status);
}

static void	check_and_print_signal(int status, int *signal_found)
{
	if (WIFSIGNALED(status))
	{
		if (*signal_found == 0)
		{
			*signal_found = WTERMSIG(status);
		}
	}
}

static void	print_signal_message(int signal_num)
{
	if (signal_num == SIGINT)
		write(STDOUT_FILENO, "\n", 1);
	else if (signal_num == SIGQUIT)
		write(STDOUT_FILENO, "Quit\n", 5);
}

static void	wait_for_stderr_flush(void)
{
	int		dummy_pipe[2];
	char	buf;

	if (pipe(dummy_pipe) == -1)
		return ;
	close(dummy_pipe[1]);
	read(dummy_pipe[0], &buf, 1);
	close(dummy_pipe[0]);
}

int	wait_processes(t_exec *exec, t_mini *mini)
{
	int	i;
	int	status;
	int	last_exit;
	int	signal_found;

	if (!exec || !exec->pids)
		return (0);
	last_exit = 0;
	signal_found = 0;
	i = 0;
	ignore_sigint_for_wait();
	while (i < exec->n_cmds)
	{
		waitpid(exec->pids[i], &status, 0);
		check_and_print_signal(status, &signal_found);
		if (i == exec->n_cmds - 1)
			handle_exit_status(status, &last_exit);
		i++;
	}
	wait_for_stderr_flush();
	setup_interactive_signals();
	if (signal_found)
		print_signal_message(signal_found);
	mini->exit_sts = last_exit;
	return (0);
}
