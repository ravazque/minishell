/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:37:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/30 17:57:23 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static char	**copy_tokens(char **tokens, int start)
{
	char	**new_tokens;
	int		i;
	int		j;

	new_tokens = malloc(sizeof(char *) * (ft_argc(tokens) - start + 1));
	if (!new_tokens)
		return (NULL);
	i = start;
	j = 0;
	while (tokens[i])
	{
		new_tokens[j] = ft_strdup(tokens[i]);
		if (!new_tokens[j])
		{
			free_dblptr(new_tokens);
			return (NULL);
		}
		i++;
		j++;
	}
	new_tokens[j] = NULL;
	return (new_tokens);
}

static int	find_non_env_token(char **tokens)
{
	int	i;

	i = 1;
	while (tokens[i])
	{
		if (ft_strcmp(tokens[i], "env") != 0)
			return (i);
		i++;
	}
	return (i);
}

static void	execute_env_builtin(t_mini *mini, t_cmd *cmd, char **new_tokens)
{
	free_dblptr(cmd->tokens);
	cmd->tokens = new_tokens;
	built_ins(mini, cmd);
}

static void	execute_env_external(t_mini *mini, char **new_tokens)
{
	char	**exec_env;

	exec_env = build_exec_env(mini);
	if (!exec_env)
		return (free_dblptr(new_tokens), malloc_error());
	fork_and_exec(new_tokens, exec_env, mini);
	free_dblptr(exec_env);
}

void	execute_env_command(t_mini *mini, t_cmd *cmd)
{
	char	**new_tokens;
	int		i;

	i = find_non_env_token(cmd->tokens);
	if (!cmd->tokens[i])
	{
		ft_setenv("_", "/usr/bin/env", &(mini->env));
		print_env(mini->env);
		mini->exit_sts = 0;
		return ;
	}
	new_tokens = copy_tokens(cmd->tokens, i);
	if (!new_tokens)
		return (malloc_error());
	if (is_builtin_cmd(new_tokens[0]))
		execute_env_builtin(mini, cmd, new_tokens);
	else
		execute_env_external(mini, new_tokens);
}
