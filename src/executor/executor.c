/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 23:16:26 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/22 23:16:28 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*ft_get_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	int		i;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		return (NULL);
	paths = ft_split(envp[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		path = ft_strjoin3(paths[i], "/", cmd);
		if (access(path, X_OK) == 0)
		{
			free_dblptr(paths);
			return (path);
		}
		else
			free(path);
		i++;
	}
	free_dblptr(paths);
	return (NULL);
}

void	ft_execve(char **argv, char **envp)
{
	char	**cmd;
	char	*path;

	cmd = argv;
	if (!cmd || !cmd[0])
	{
		ft_putstr_fd("minishell: error: empty command\n", STDERR_FILENO);
		exit(127);
	}
	path = ft_get_path(cmd[0], envp);
	if (!path)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		exit(127);
	}
	if (execve(path, cmd, envp) == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		free(path);
		exit(127);
	}
}

void	ft_close(int *fd, int index, int n_fd)
{
	int	i;

	i = 0;
	while (i < n_fd)
	{
		if (i != ((index - 1) * 2) && i != (index * 2 + 1))
			close(fd[i]);
		i++;
	}
}

void	ft_close_and_wait(int *fd, int n_fd, pid_t *pid, int n_cmd)
{
	int	i;

	i = 0;
	while (i < n_fd)
	{
		close(fd[i]);
		i++;
	}
	i = 0;
	while (i < n_cmd)
	{
		waitpid(pid[i], NULL, 0);
		i++;
	}
}

// void	ft_pid(t_cmd *cmd_aux, t_mini *mini)
// {
// 	pid_t	pid;
// }

void	executor(t_mini *mini)
{
	t_cmd	*cmd_aux;
	int		n_cmd;
	pid_t	*pid;
	int		*fd;
	int		i;
	int		index;

	if (!mini || !mini->cmds)
		return ;
	cmd_aux = mini->cmds;
	n_cmd = ft_lstsize(mini->cmds);
	if (handle_heredocs(mini))
		return ;
	if (n_cmd == 1 && is_builtin_cmd(cmd_aux->tokens[0]) == 1)
	{
		built_ins(mini, cmd_aux);
		return ;
	}
	pid = malloc(sizeof(pid_t) * n_cmd);
	fd = malloc(sizeof(int) * 2 * (n_cmd - 1));
	if (!fd)
	{
		free(pid);
		return ;
	}
	i = 0;
	index = 0;
	//ft_pipes();
	while (i < (n_cmd - 1))
	{
		if (pipe(&fd[i * 2]) == -1)
		{
			ft_putstr_fd("minishell: error: pipe creation failed\n", STDERR_FILENO);
			free(fd);
			free(pid);
			return ;
		}
		i++;
	}
	while (cmd_aux)
	{
		//ft_pid(cmd_aux, mini);
		pid[index] = fork();
		if (pid[index] == -1)
		{
			ft_putstr_fd("minishell: error: fork failed\n", STDERR_FILENO);
			ft_close_and_wait(fd, 2 * (n_cmd - 1), pid, index);
			free(fd);
			free(pid);
			return ;
		}
		else if (pid[index] == 0)
		{
			ft_close(fd, index, 2 * (n_cmd - 1));
			if (index == 0)
			{
				dup2(fd[1], STDOUT_FILENO);
			}
			else if (cmd_aux->next == NULL)
			{
				dup2(fd[(index - 1) * 2], STDIN_FILENO);
			}
			else
			{
				dup2(fd[(index - 1) * 2], STDIN_FILENO);
				dup2(fd[index * 2 + 1], STDOUT_FILENO);
			}
			i = 0;
			while (i < (2 * (n_cmd - 1)))
			{
				close(fd[i]);
				i++;
			}
			if (built_ins(mini, cmd_aux) == false)
				ft_execve(cmd_aux->tokens, mini->env);
			else
				exit(EXIT_SUCCESS);
		}
		cmd_aux = cmd_aux->next;
		index++;
	}
	ft_close_and_wait(fd, 2 * (n_cmd - 1), pid, n_cmd);
	free(fd);
	free(pid);
}

// ============================================================================== //

// void	executor(t_mini *mini)	// mi versión del ejecutor con pseudo-cat
// {
// 	int	cmd_count;

// 	if (!mini || !mini->cmds)
// 		return ;
// 	cmd_count = ft_lstsize(mini->cmds);
// 	if (handle_heredocs(mini))
// 		return ;
// 	if (cmd_count == 1)
// 		execute_simple_command(mini);
// 	else
// 		execute_pipeline(mini);
// 	built_ins(mini);
// 	printf("=========== ARGS ============\n");
// 	print_dblptr(mini->cmds->tokens);
// 	printf("=============================\n");
// }

// ============================================================================== //
