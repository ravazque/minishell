/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 18:02:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/30 18:02:00 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	check_empty_cmd(char **argv)
{
	if (!argv || !argv[0])
	{
		ft_putstr_fd("minishell: : command not found\n", STDERR_FILENO);
		exit(127);
	}
	if (argv[0][0] == '\0')
	{
		ft_putstr_fd("minishell: : command not found\n", STDERR_FILENO);
		exit(127);
	}
}

void	ft_execve(char **argv, char **envp, char ***env_ptr)
{
	char	*path;
	int		error_code;

	check_empty_cmd(argv);
	path = ft_get_path(argv[0], envp);
	if (!path)
		handle_no_path(argv);
	ft_setenv("_", path, env_ptr);
	if (execve(path, argv, *env_ptr) == -1)
	{
		error_code = 127;
		if (errno == EACCES)
			error_code = 126;
		print_exec_error(path, error_code, 1);
		free(path);
		exit(error_code);
	}
}

void	execute_child_process(t_mini *mini, t_cmd *cmd, t_exec *exec, int idx)
{
	int		builtin_type;
	char	**exec_env;

	setup_execution_signals();
	setup_pipe_fds(exec, idx);
	if (redirections(cmd))
		exit(1);
	if (!cmd->tokens || !cmd->tokens[0])
		exit(0);
	builtin_type = is_builtin_cmd(cmd->tokens[0]);
	if (builtin_type)
	{
		built_ins(mini, cmd);
		exit(mini->exit_sts);
	}
	exec_env = build_exec_env(mini);
	if (!exec_env)
		exit(1);
	ft_execve(cmd->tokens, exec_env, &exec_env);
	free_dblptr(exec_env);
}
