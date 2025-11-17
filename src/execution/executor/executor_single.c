/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_single.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 18:03:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/04 13:51:32 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	execute_single_child(t_mini *mini, t_cmd *cmd)
{
	char	**exec_env;

	setup_execution_signals();
	if (redirections(cmd))
	{
		cleanup_child_fds();
		exit(1);
	}
	if (is_builtin_cmd(cmd->tokens[0]))
	{
		built_ins(mini, cmd);
		cleanup_child_fds();
		exit(mini->exit_sts);
	}
	exec_env = build_exec_env(mini);
	if (!exec_env)
	{
		cleanup_child_fds();
		exit(1);
	}
	ft_execve(cmd->tokens, exec_env, &exec_env, mini->cd_home);
	free_dblptr(exec_env);
	cleanup_child_fds();
}

static void	wait_single_child(pid_t pid, t_mini *mini)
{
	int	status;

	ignore_sigint_for_wait();
	waitpid(pid, &status, 0);
	setup_interactive_signals();
	if (WIFEXITED(status))
		mini->exit_sts = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		mini->exit_sts = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		else if (WTERMSIG(status) == SIGQUIT)
			write(STDOUT_FILENO, "Quit\n", 5);
	}
}

int	execute_single_command(t_mini *mini, t_cmd *cmd)
{
	pid_t	pid;

	if (!cmd->tokens || !cmd->tokens[0])
		return (0);
	if (is_builtin_cmd(cmd->tokens[0]) == 1)
	{
		if (redirections(cmd))
			return (1);
		built_ins(mini, cmd);
		return (0);
	}
	pid = fork();
	if (pid == -1)
	{
		ft_putstr_fd("minishell: error: fork failed\n", STDERR_FILENO);
		return (1);
	}
	if (pid == 0)
		execute_single_child(mini, cmd);
	wait_single_child(pid, mini);
	return (0);
}
