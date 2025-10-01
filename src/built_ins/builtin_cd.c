/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 04:43:21 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/01 17:39:12 by ravazque         ###   ########.fr       */
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

static void	ft_setenv(char *name, char *value, char ***env)
{
	char	*var;
	int		i;
	char	**new_env;
	int		n_len;

	var = ft_strjoin3(name, "=", value);
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

static char	*ft_cd_path(t_mini mini)
{
	char	*path;

	if (mini.cmds->tokens[2])
		return (ft_putstr_fd("minishell: cd: too many arguments\n", 2), NULL);
	else if (!mini.cmds->tokens[1])
	{
		path = getenv("HOME");
		if (!path)
			return (ft_putstr_fd("minishell: cd: HOME not set\n", 2), NULL);
	}
	else if (mini.cmds->tokens[1] && !ft_strcmp(mini.cmds->tokens[1], "-"))
	{
		path = getenv("OLDPWD");
		if (!path)
			return (ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2), NULL);
		printf("%s\n", path);
	}
	else
		path = mini.cmds->tokens[1];
	return (path);
}

void	builtin_cd(t_mini *mini)
{
	char	*oldpwd;
	char	*pwd;
	char	*path;

	path = ft_cd_path(*mini);
	if (!path)
		return ;
	oldpwd = getcwd(NULL, 0);
	if (chdir(path) < 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		free(oldpwd);
		return ;
	}
	pwd = getcwd(NULL, 0);
	if (oldpwd)
		ft_setenv("OLDPWD", oldpwd, &(mini->env));
	if (pwd)
		ft_setenv("PWD", pwd, &(mini->env));
	free(oldpwd);
	free(pwd);
}
