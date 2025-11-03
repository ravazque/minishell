/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_setenv.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:30:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/30 17:57:43 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	ft_envlen(char **env)
{
	int	i;

	i = 0;
	while (env[i])
		i++;
	return (i);
}

static int	try_update_existing(char *name, char *var, char ***env)
{
	int	i;
	int	n_len;

	n_len = ft_strlen(name);
	i = 0;
	while ((*env)[i])
	{
		if (!ft_strncmp((*env)[i], name, n_len))
		{
			if ((*env)[i][n_len] == '=' || (*env)[i][n_len] == '\0')
			{
				free((*env)[i]);
				(*env)[i] = var;
				return (1);
			}
		}
		i++;
	}
	return (0);
}

static char	**create_expanded_env(char ***env, int env_len)
{
	char	**new_env;
	int		i;

	new_env = malloc(sizeof(char *) * (env_len + 2));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < env_len)
	{
		new_env[i] = (*env)[i];
		i++;
	}
	return (new_env);
}

static void	add_new_var(char *var, char ***env)
{
	char	**new_env;
	int		env_len;
	int		i;

	env_len = ft_envlen(*env);
	new_env = create_expanded_env(env, env_len);
	if (!new_env)
	{
		free(var);
		return (malloc_error());
	}
	i = env_len;
	new_env[i] = var;
	new_env[i + 1] = NULL;
	free(*env);
	*env = new_env;
}

void	ft_setenv(char *name, char *value, char ***env)
{
	char	*var;

	if (!value)
		var = ft_strdup(name);
	else
		var = ft_strjoin3(name, "=", value);
	if (!var)
		return (malloc_error());
	if (try_update_existing(name, var, env))
		return ;
	add_new_var(var, env);
}
