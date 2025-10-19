/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 04:43:21 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/19 22:02:33 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*ft_cd_path(t_mini *mini)
{
	char	*path;

	if (mini->cmds->tokens[2] && mini->cmds->tokens[1])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (mini->exit_sts = 2, NULL);
	}
	if (!mini->cmds->tokens[1])
	{
		path = get_local_env("HOME", mini->env);
		if (!path)
			return (ft_putstr_fd(ERR_HOME, 2), mini->exit_sts = 1, NULL);
		return (path);
	}
	if (!ft_strcmp(mini->cmds->tokens[1], "-"))
	{
		path = get_local_env("OLDPWD", mini->env);
		if (!path)
			return (ft_putstr_fd(ERR_OLDPWD, 2), mini->exit_sts = 1, NULL);
		printf("%s\n", path);
		return (path);
	}
	if (!ft_strcmp(mini->cmds->tokens[1], "~"))
	{
		path = get_local_env("HOME", mini->env);
		if (!path)
			return (ft_putstr_fd(ERR_HOME, 2), mini->exit_sts = 1, NULL);
		return (path);
	}
	return (mini->cmds->tokens[1]);
}

static char	*join_path(const char *base, const char *component)
{
	char	*temp;
	char	*result;
	size_t	base_len;

	if (!base || !component)
		return (NULL);
	base_len = ft_strlen(base);
	if (base_len == 0)
		return (ft_strdup(component));
	if (base[base_len - 1] == '/')
		return (ft_strjoin(base, component));
	temp = ft_strjoin(base, "/");
	if (!temp)
		return (NULL);
	result = ft_strjoin(temp, component);
	free(temp);
	return (result);
}

static char	*get_logical_pwd(const char *current_pwd, const char *path)
{
	if (!current_pwd)
		return (NULL);
	if (path[0] == '/')
		return (ft_strdup(path));
	if (!ft_strcmp(path, "."))
		return (ft_strdup(current_pwd));
	return (join_path(current_pwd, path));
}

void	builtin_cd(t_mini *mini)
{
	char	*oldpwd;
	char	*pwd;
	char	*path;

	path = ft_cd_path(mini);
	if (!path)
		return ;
	oldpwd = get_local_env("PWD", mini->env);
	if (chdir(path) < 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		mini->exit_sts = 1;
		return ;
	}
	pwd = getcwd(NULL, 0);
	if (!pwd)
		pwd = get_logical_pwd(oldpwd, path);
	if (oldpwd)
		ft_setenv("OLDPWD", oldpwd, &(mini->env));
	if (pwd)
	{
		ft_setenv("PWD", pwd, &(mini->env));
		free(pwd);
	}
	mini->exit_sts = 0;
}
