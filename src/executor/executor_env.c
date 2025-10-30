/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 18:40:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/30 18:40:00 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	copy_env_vars(char **new_env, char **env, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		new_env[i] = ft_strdup(env[i]);
		if (!new_env[i])
		{
			while (i > 0)
				free(new_env[--i]);
			return (-1);
		}
		i++;
	}
	return (i);
}

static int	copy_local_vars(char **new_env, char **local, int start, int count)
{
	int	j;

	j = 0;
	while (j < count)
	{
		new_env[start + j] = ft_strdup(local[j]);
		if (!new_env[start + j])
		{
			while (start + j > 0)
				free(new_env[--start]);
			return (-1);
		}
		j++;
	}
	return (j);
}

char	**build_exec_env(t_mini *mini)
{
	char	**new_env;
	int		env_count;
	int		local_count;
	int		i;

	if (!mini || !mini->env)
		return (NULL);
	env_count = ft_argc(mini->env);
	local_count = ft_argc(mini->local_vars);
	new_env = malloc(sizeof(char *) * (env_count + local_count + 1));
	if (!new_env)
		return (NULL);
	i = copy_env_vars(new_env, mini->env, env_count);
	if (i == -1)
		return (free(new_env), NULL);
	if (copy_local_vars(new_env, mini->local_vars, i, local_count) == -1)
		return (free(new_env), NULL);
	new_env[i + local_count] = NULL;
	return (new_env);
}
