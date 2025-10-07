/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 04:43:21 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/08 01:00:38 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*ft_cd_path(t_mini *mini)
{
	char	*path;

	if (mini->cmds->tokens[1] && mini->cmds->tokens[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		mini->exit_sts = 2;
		return (NULL);
	}
	if (!mini->cmds->tokens[1])
	{
		path = get_localenv("HOME", mini->env);
		if (!path)
		{
			ft_putstr_fd(ERR_HOME, 2);
			mini->exit_sts = 1;
			return (NULL);
		}
		return (path);
	}
	if (!ft_strcmp(mini->cmds->tokens[1], "-"))
	{
		path = get_localenv("OLDPWD", mini->env);
		if (!path)
		{
			ft_putstr_fd(ERR_OLDPWD, 2);
			mini->exit_sts = 1;
			return (NULL);
		}
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
		if (oldpwd)
			free(oldpwd);
		mini->exit_sts = 1;
		return ;
	}
	pwd = getcwd(NULL, 0);
	if (oldpwd)
	{
		ft_setenv("OLDPWD", oldpwd, &(mini->env));
		free(oldpwd);
	}
	if (pwd)
	{
		ft_setenv("PWD", pwd, &(mini->env));
		free(pwd);
	}
	mini->exit_sts = 0;
}
