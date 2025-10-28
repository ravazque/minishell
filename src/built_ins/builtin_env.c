/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 22:54:02 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/28 16:07:59 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	print_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strchr(env[i], '='))
		{
			ft_putstr_fd(env[i], STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
		i++;
	}
}

static char	*get_command_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	int		i;

	if (!cmd || !envp)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
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

static void	fork_and_exec(char **tokens, char **envp, t_mini *mini)
{
	pid_t	pid;
	int		status;
	char	*path;

	pid = fork();
	if (pid == -1)
	{
		ft_putstr_fd("minishell: error: fork failed\n", STDERR_FILENO);
		free_dblptr(tokens);
		return ;
	}
	if (pid == 0)
	{
		path = get_command_path(tokens[0], envp);
		if (!path || execve(path, tokens, envp) == -1)
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
	}
	free_dblptr(tokens);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		mini->exit_sts = WEXITSTATUS(status);
}

static void	execute_env_command(t_mini *mini, t_cmd *cmd)
{
	char	**new_tokens;
	char	**exec_env;
	int		i;
	int		j;

	i = 1;
	while (cmd->tokens[i] && ft_strcmp(cmd->tokens[i], "env") == 0)
		i++;
	if (!cmd->tokens[i])
	{
		ft_setenv("_", "/usr/bin/env", &(mini->env));
		print_env(mini->env);
		mini->exit_sts = 0;
		return ;
	}
	new_tokens = malloc(sizeof(char *) * (ft_argc(cmd->tokens) - i + 1));
	if (!new_tokens)
		return (malloc_error());
	j = 0;
	while (cmd->tokens[i])
	{
		new_tokens[j] = ft_strdup(cmd->tokens[i]);
		if (!new_tokens[j])
		{
			free_dblptr(new_tokens);
			return (malloc_error());
		}
		i++;
		j++;
	}
	new_tokens[j] = NULL;
	if (is_builtin_cmd(new_tokens[0]))
	{
		free_dblptr(cmd->tokens);
		cmd->tokens = new_tokens;
		built_ins(mini, cmd);
	}
	else
	{
		exec_env = build_exec_env(mini);
		if (!exec_env)
		{
			free_dblptr(new_tokens);
			return (malloc_error());
		}
		fork_and_exec(new_tokens, exec_env, mini);
		free_dblptr(exec_env);
	}
}

void	builtin_env(t_mini *mini, t_cmd *cmd)
{
	if (!cmd->tokens[1])
	{
		ft_setenv("_", "/usr/bin/env", &(mini->env));
		print_env(mini->env);
		mini->exit_sts = 0;
		return ;
	}
	execute_env_command(mini, cmd);
}
