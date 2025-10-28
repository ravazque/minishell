/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 01:34:14 by ptrapero          #+#    #+#             */
/*   Updated: 2025/10/28 16:07:53 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_argc(char **argv)
{
	int	i;

	i = 0;
	while (argv[i] != NULL)
		i++;
	return (i);
}

char	*get_local_env(const char *name, char **env)
{
	int		i;
	int		n_len;

	if (!name || !env)
		return (NULL);
	n_len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], name, n_len) && env[i][n_len] == '=')
			return (env[i] + n_len + 1);
		i++;
	}
	return (NULL);
}

char	*get_var_value(const char *name, t_mini *mini)
{
	char	*env_val;
	char	*local_val;

	if (!name || !mini)
		return (NULL);
	env_val = get_local_env(name, mini->env);
	if (env_val)
		return (env_val);
	local_val = get_local_env(name, mini->local_vars);
	if (local_val)
		return (local_val);
	return (NULL);
}

void	set_local_var(char *name, char *value, t_mini *mini)
{
	char	*env_val;

	if (!name || !mini)
		return ;
	env_val = get_local_env(name, mini->env);
	if (env_val)
	{
		ft_setenv(name, value, &(mini->env));
		return ;
	}
	ft_setenv(name, value, &(mini->local_vars));
}

static int	ft_envlen(char **env)
{
	int	i;

	i = 0;
	while (env[i])
		i++;
	return (i);
}

void	ft_setenv(char *name, char *value, char ***env)
{
	char	*var;
	int		i;
	char	**new_env;
	int		n_len;
	int		env_len;

	if (!value)
		var = ft_strdup(name);
	else
		var = ft_strjoin3(name, "=", value);
	if (!var)
		return (malloc_error());
	n_len = ft_strlen(name);
	i = 0;
	while ((*env)[i])
	{
		if (!ft_strncmp((*env)[i], name, n_len)
			&& ((*env)[i][n_len] == '=' || (*env)[i][n_len] == '\0'))
		{
			free((*env)[i]);
			(*env)[i] = var;
			return ;
		}
		i++;
	}
	env_len = ft_envlen(*env);
	new_env = malloc(sizeof(char *) * (env_len + 2));
	if (!new_env)
	{
		free(var);
		return (malloc_error());
	}
	i = 0;
	while (i < env_len)
	{
		new_env[i] = (*env)[i];
		i++;
	}
	new_env[i] = var;
	new_env[i + 1] = NULL;
	free(*env);
	*env = new_env;
}
