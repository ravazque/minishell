/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 16:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/28 16:10:48 by ravazque         ###   ########.fr       */
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

int	has_redirs(t_cmd *cmd)
{
	if (!cmd)
		return (0);
	return (cmd->redirs != NULL);
}

int	count_args(char **tokens)
{
	int	i;

	i = 0;
	if (!tokens)
		return (0);
	while (tokens[i])
		i++;
	return (i);
}

int	is_empty_cmd(t_cmd *cmd)
{
	if (!cmd)
		return (1);
	if (!cmd->tokens || !cmd->tokens[0])
		return (1);
	if (cmd->tokens[0][0] == '\0')
		return (1);
	return (0);
}

char	**build_exec_env(t_mini *mini)
{
	char	**new_env;
	int		env_count;
	int		local_count;
	int		i;
	int		j;

	if (!mini || !mini->env)
		return (NULL);
	env_count = 0;
	while (mini->env[env_count])
		env_count++;
	local_count = 0;
	if (mini->local_vars)
	{
		while (mini->local_vars[local_count])
			local_count++;
	}
	new_env = malloc(sizeof(char *) * (env_count + local_count + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < env_count)
	{
		new_env[i] = ft_strdup(mini->env[i]);
		if (!new_env[i])
		{
			while (i > 0)
				free(new_env[--i]);
			free(new_env);
			return (NULL);
		}
		i++;
	}
	j = 0;
	while (j < local_count)
	{
		new_env[i + j] = ft_strdup(mini->local_vars[j]);
		if (!new_env[i + j])
		{
			while (i + j > 0)
				free(new_env[--i]);
			free(new_env);
			return (NULL);
		}
		j++;
	}
	new_env[i + j] = NULL;
	return (new_env);
}
