/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ptrapero <ptrapero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 23:16:26 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/24 18:53:53 by ptrapero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	is_directory(const char *path)
{
	struct stat	statbuf;

	if (stat(path, &statbuf) == 0)
	{
		if (S_ISDIR(statbuf.st_mode))
			return (1);
	}
	return (0);
}

static char	*ft_get_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	int		i;

	if (!cmd || !envp)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (is_directory(cmd))
			return (NULL);
		if (access(cmd, F_OK) == 0)
		{
			if (access(cmd, X_OK) == 0)
				return (ft_strdup(cmd));
			return (NULL);
		}
		return (NULL);
	}
	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		return (NULL);
	paths = ft_split(envp[i] + 5, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		path = ft_strjoin3(paths[i], "/", cmd);
		if (!path)
		{
			free_dblptr(paths);
			return (NULL);
		}
		if (access(path, X_OK) == 0)
		{
			free_dblptr(paths);
			return (path);
		}
		free(path);
		i++;
	}
	free_dblptr(paths);
	return (NULL);
}

// char	*ft_get_path(char *cmd, char **envp)
// {
// 	char	**paths;
// 	char	*path;
// 	int		i;

// 	i = 0;
// 	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
// 		i++;
// 	if (!envp[i])
// 		return (NULL);
// 	paths = ft_split(envp[i] + 5, ':');
// 	i = 0;
// 	while (paths[i])
// 	{
// 		path = ft_strjoin3(paths[i], "/", cmd);
// 		if (access(path, X_OK) == 0)
// 		{
// 			free_dblptr(paths);
// 			return (path);
// 		}
// 		else
// 			free(path);
// 		i++;
// 	}
// 	free_dblptr(paths);
// 	return (NULL);
// }

static void	print_exec_error(t_mini *mini, char *cmd, int error_type, int is_path)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);

	if (error_type == 126 && mini->flag_error == 0)
	{
		mini->flag_error = 1;
		if (is_directory(cmd))
			ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
		else
			ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
	}
	else if (error_type == 127 && mini->flag_error == 0)
	{
		mini->flag_error = 1;
		if (is_path)
			ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		else
			ft_putstr_fd(": command not found\n", STDERR_FILENO);
	}
	else if (mini->flag_error == 0)
	{
		mini->flag_error = 1;
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
	}
}

static void	ft_execve(t_mini *mini, char **argv, char **envp, char ***env_ptr)
{
	char	*path;
	int		error_code;

	if (!argv || !argv[0])
	{
		ft_putstr_fd("minishell: error: empty command\n", STDERR_FILENO);
		exit(127);
	}

	path = ft_get_path(argv[0], envp);

	if (!path)
	{
		if (ft_strchr(argv[0], '/'))
		{
			if (access(argv[0], F_OK) == 0)
			{
				print_exec_error(mini, argv[0], 126, 1);
				exit(126);
			}
			else
			{
				print_exec_error(mini, argv[0], 127, 1);
				exit(127);
			}
		}
		else
		{
			print_exec_error(mini, argv[0], 127, 0);
			exit(127);
		}
	}

	ft_setenv("_", path, env_ptr);
	if (execve(path, argv, *env_ptr) == -1)
	{
		error_code = 127;
		if (errno == EACCES)
			error_code = 126;
		print_exec_error(mini, path, error_code, 1);
		free(path);
		exit(error_code);
	}
}

// void	ft_execve(char **argv, char **envp)
// {
// 	char	**cmd;
// 	char	*path;

// 	cmd = argv;
// 	if (!cmd || !cmd[0])
// 	{
// 		ft_putstr_fd("minishell: error: empty command\n", STDERR_FILENO);
// 		exit(127);
// 	}
// 	path = ft_get_path(cmd[0], envp);
// 	if (!path)
// 	{
// 		ft_putstr_fd("minishell: ", STDERR_FILENO);
// 		ft_putstr_fd(cmd[0], STDERR_FILENO);
// 		ft_putstr_fd(": command not found\n", STDERR_FILENO);
// 		exit(127);
// 	}
// 	if (execve(path, cmd, envp) == -1)
// 	{
// 		ft_putstr_fd("minishell: ", STDERR_FILENO);
// 		ft_putstr_fd(cmd[0], STDERR_FILENO);
// 		ft_putstr_fd(": command not found\n", STDERR_FILENO);
// 		free(path);
// 		exit(127);
// 	}
// }

static void	execute_child_process(t_mini *mini, t_cmd *cmd, t_exec *exec, int idx)
{
	int	builtin_type;

	setup_execution_signals();
	setup_pipe_fds(exec, idx);
	if (redirections(mini, cmd))
		exit(1);
	builtin_type = is_builtin_cmd(cmd->tokens[0]);
	if (builtin_type == 1 && exec->n_cmds > 1)
		exit(0);
	if (builtin_type)
	{
		built_ins(mini, cmd);
		exit(mini->exit_sts);
	}
	ft_execve(mini, cmd->tokens, mini->env, &(mini->env));
}

static int	execute_pipeline(t_mini *mini, t_exec *exec)
{
	t_cmd	*cmd;
	int		idx;

	if (create_pipes(exec))
	{
		ft_putstr_fd("minishell: error: pipe creation failed\n", STDERR_FILENO);
		return (1);
	}
	cmd = mini->cmds;
	idx = 0;
	while (cmd)
	{
		exec->pids[idx] = fork();
		if (exec->pids[idx] == -1)
		{
			ft_putstr_fd("minishell: error: fork failed\n", STDERR_FILENO);
			close_pipes(exec);
			return (1);
		}
		if (exec->pids[idx] == 0)
			execute_child_process(mini, cmd, exec, idx);
		cmd = cmd->next;
		idx++;
	}
	close_pipes(exec);
	return (wait_processes(exec, mini));
}

static int	execute_single_command(t_mini *mini, t_cmd *cmd)
{
	pid_t	pid;
	int		status;

	if (is_builtin_cmd(cmd->tokens[0]) == 1)
	{
		if (redirections(mini, cmd))
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
	{
		setup_execution_signals();
		if (redirections(mini, cmd))
			exit(1);
		if (is_builtin_cmd(cmd->tokens[0]))
		{
			built_ins(mini, cmd);
			exit(mini->exit_sts);
		}
		ft_execve(mini, cmd->tokens, mini->env, &(mini->env));
	}
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
	}
	return (0);
}

void	executor(t_mini *mini)
{
	t_exec	exec;
	int		n_cmds;

	if (!mini || !mini->cmds)
		return ;
	n_cmds = ft_lstsize(mini->cmds);
	if (heredocs(mini))
	{
		mini->exit_sts = 130;
		return ;
	}
	if (is_empty_cmd(mini->cmds))
		return ;
	if (init_exec(&exec, n_cmds))
	{
		ft_putstr_fd("minishell: error: executor init failed\n", STDERR_FILENO);
		mini->exit_sts = 1;
		return ;
	}
	if (n_cmds == 1)
		execute_single_command(mini, mini->cmds);
	else
		execute_pipeline(mini, &exec);
	cleanup_exec(&exec);
}
