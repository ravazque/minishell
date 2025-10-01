/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 04:43:21 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/01 17:50:06 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	ft_envlen(char **env)
{
	int	i;

	i = 0;
	while (env[i])
		i++;
	return (i);
}

static char	*get_localenv(const char *name, char **env)
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

static void	ft_setenv(char *name, char *value, char ***env)
{
	char	*var;
	int		i;
	char	**new_env;
	int		n_len;

	var = ft_strjoin3(name, "=", value);
	if (!var)
		return ;
	n_len = ft_strlen(name);
	i = 0;
	while ((*env)[i])
	{
		if (!ft_strncmp((*env)[i], name, n_len) && (*env)[i][n_len] == '=')
		{
			free((*env)[i]);
			(*env)[i] = var;
			return ;
		}
		i++;
	}
	new_env = malloc(sizeof(char *) * (ft_envlen(*env) + 2));
	if (!new_env)
	{
		free(var);
		return ;
	}
	i = 0;
	while ((*env)[i])
	{
		new_env[i] = (*env)[i];
		i++;
	}
	new_env[i] = var;
	new_env[i + 1] = NULL;
	free(*env);
	*env = new_env;
}

static char	*ft_cd_path(t_mini *mini)
{
	char	*path;

	if (mini->cmds->tokens[2] && mini->cmds->tokens[1])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (mini->exit_sts = 1, NULL);
	}
	if (!mini->cmds->tokens[1])
	{
		path = get_localenv("HOME", mini->env);
		if (!path)
			return (ft_putstr_fd(ERR_HOME, 2), mini->exit_sts = 1, NULL);
		return (path);
	}
	if (!ft_strcmp(mini->cmds->tokens[1], "-"))
	{
		path = get_localenv("OLDPWD", mini->env);
		if (!path)
			return (ft_putstr_fd(ERR_OLDPWD, 2), mini->exit_sts = 1, NULL);
		printf("%s\n", path);
		return (path);
	}
	return (mini->cmds->tokens[1]);
}

void	builtin_cd(t_mini *mini)
{
	char	*oldpwd;
	char	*pwd;
	char	*path;

	path = ft_cd_path(mini);
	if (!path)
		return ;
	oldpwd = getcwd(NULL, 0);
	if (chdir(path) < 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		free(oldpwd);
		mini->exit_sts = 1;
		return ;
	}
	pwd = getcwd(NULL, 0);
	if (oldpwd)
		ft_setenv("OLDPWD", oldpwd, &(mini->env));
	if (pwd)
		ft_setenv("PWD", pwd, &(mini->env));
	free(oldpwd);
	free(pwd);
	mini->exit_sts = 0;
}
