/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env_fork.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:36:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/30 17:57:28 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	exec_error_exit(char **tokens, char *path, char **envp)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(tokens[0], STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
	if (path)
		free(path);
	free_dblptr(tokens);
	free_dblptr(envp);
	exit(127);
}

static void	child_exec(char **tokens, char **envp)
{
	char	*path;

	path = get_command_path(tokens[0], envp);
	if (!path || execve(path, tokens, envp) == -1)
		exec_error_exit(tokens, path, envp);
}

void	fork_and_exec(char **tokens, char **envp, t_mini *mini)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		ft_putstr_fd("minishell: error: fork failed\n", STDERR_FILENO);
		free_dblptr(tokens);
		return ;
	}
	if (pid == 0)
		child_exec(tokens, envp);
	free_dblptr(tokens);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		mini->exit_sts = WEXITSTATUS(status);
}
