/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 18:02:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/13 14:06:29 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	check_empty_cmd(char **argv)
{
	if (!argv || !argv[0])
	{
		ft_putstr_fd("minishell: : command not found\n", STDERR_FILENO);
		cleanup_child_fds();
		exit(127);
	}
	if (argv[0][0] == '\0')
	{
		ft_putstr_fd("minishell: : command not found\n", STDERR_FILENO);
		cleanup_child_fds();
		exit(127);
	}
}

void	ft_execve(char **argv, char **envp, char ***env_ptr, char *cd_home)
{
	char	*path;
	int		error_code;

	check_empty_cmd(argv);
	path = ft_get_path(argv[0], envp);
	if (!path)
		handle_no_path(argv, cd_home);
	ft_setenv("_", path, env_ptr);
	if (execve(path, argv, *env_ptr) == -1)
	{
		error_code = 127;
		if (errno == EACCES)
			error_code = 126;
		print_exec_error(path, error_code, 1);
		free(path);
		cleanup_child_fds();
		exit(error_code);
	}
}

static void	child_fail(int out)
{
	cleanup_child_fds();
	exit(out);
}

static void	check_exec_env(char **exec_env)
{
	if (!exec_env)
		child_fail(1);
}

void	execute_child_process(t_mini *mini, t_cmd *cmd, t_exec *exec, int idx)
{
	int		builtin_type;
	char	**exec_env;
	t_cmd	*original_cmds;

	setup_execution_signals();
	setup_pipe_fds(exec, idx);
	if (redirections(cmd))
		child_fail(1);
	if (!cmd->tokens || !cmd->tokens[0])
		child_fail(0);
	builtin_type = is_builtin_cmd(cmd->tokens[0]);
	if (builtin_type)
	{
		original_cmds = mini->cmds;
		mini->cmds = cmd;
		built_ins(mini, cmd);
		mini->cmds = original_cmds;
		cleanup_child_fds();
		exit(mini->exit_sts);
	}
	exec_env = build_exec_env(mini);
	check_exec_env(exec_env);
	ft_execve(cmd->tokens, exec_env, &exec_env, mini->cd_home);
	free_dblptr(exec_env);
	cleanup_child_fds();
}
