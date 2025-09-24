/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 04:43:21 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/24 16:13:26 by ravazque         ###   ########.fr       */
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

void	builtin_cd(t_mini *mini)
{
	char	*target;

	if (!mini->cmds->tokens[1])
	{
		target = cd_getenv(mini->env, "HOME");
		if (!target || !*target)
		{
			ft_putstr_fd("minishell: cd: $HOME not set\n", 2);
			return ;
		}
	}
	else if (mini->cmds->tokens[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		mini->exit_sts = 2;
		return ;
	}
	else
		target = mini->cmds->tokens[1];
	if (chdir(target) < 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(target, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		mini->exit_sts = 1;
		return ;
	}
	mini->exit_sts = 0;
}
