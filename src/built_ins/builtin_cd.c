/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 04:43:21 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/30 17:42:18 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*cd_getenv(char **env, const char *key)
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

static int	do_chdir(t_mini *mini, char *tgt)
{
	if (chdir(tgt) < 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(tgt, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		mini->exit_sts = 1;
		return (1);
	}
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
	if (do_chdir(mini, tgt))
	{
		free(oldpwd);
		return ;
	}
	pwd = getcwd(NULL, 0);
	free(oldpwd);
	free(pwd);
	mini->exit_sts = 0;
}
