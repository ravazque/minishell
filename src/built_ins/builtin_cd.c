/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 04:43:21 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/30 22:03:21 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*cd_getenv(char **env, const char *key)
{
	size_t	j;
	int		i;

	if (!env || !key)
		return (NULL);
	j = ft_strlen(key);
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], key, j) && env[i][j] == '=')
			return (env[i] + j + 1);
		i++;
	}
	return (NULL);
}

static int	check_args(t_mini *mini, char **tgt)
{
	if (!mini->cmds->tokens[1])
	{
		*tgt = cd_getenv(mini->env, "HOME");
		if (!*tgt || !**tgt)
		{
			ft_putstr_fd("minishell: cd: path not provided\n", 2);
			mini->exit_sts = 1;
			return (1);
		}
	}
	else if (mini->cmds->tokens[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		mini->exit_sts = 2;
		return (1);
	}
	else
		*tgt = mini->cmds->tokens[1];
	return (0);
}

void	builtin_cd(t_mini *mini)
{
	char	*tgt;
	char	*oldpwd;
	char	*pwd;

	if (check_args(mini, &tgt))
		return ;
	oldpwd = getcwd(NULL, 0);
	if (chdir(tgt) < 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(tgt, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		mini->exit_sts = 1;
		free(oldpwd);
		return ;
	}
	pwd = getcwd(NULL, 0);
	free(oldpwd);
	free(pwd);
	mini->exit_sts = 0;
}

// actualizar builtin de ptrapero