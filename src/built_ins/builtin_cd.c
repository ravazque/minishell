/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 04:43:21 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/24 07:45:50 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*ms_getenv(char **env, const char *key)
{
	size_t	klen;
	int		i;

	if (!env || !key)
		return (NULL);
	klen = ft_strlen(key);
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], key, klen) && env[i][klen] == '=')
			return (env[i] + klen + 1);
		i++;
	}
	return (NULL);
}

void	builtin_cd(t_mini mini)
{
	char	*target;
	char	*oldpwd;
	char	*pwd;

	if (!mini.cmds->tokens[1])
	{
		target = ms_getenv(mini.env, "HOME");
		if (!target || !*target)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			return ;
		}
	}
	else if (mini.cmds->tokens[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return ;
	}
	else
		target = mini.cmds->tokens[1];
	oldpwd = getcwd(NULL, 0);
	if (chdir(target) < 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(target, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		free(oldpwd);
		return ;
	}
	pwd = getcwd(NULL, 0);
	free(oldpwd);
	free(pwd);
}
