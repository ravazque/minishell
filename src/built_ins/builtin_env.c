/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ptrapero <ptrapero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 22:54:02 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/24 19:09:23 by ptrapero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	builtin_env(t_mini *mini, t_cmd *cmd)
{
	int		i;
	int		j;

	j = 1;
	while (cmd->tokens[j])
	{
		if (ft_strcmp(cmd->tokens[j], "env") == 0)
			j++;
		else
		{
			ft_putstr_fd("minishell: env: '", STDERR_FILENO);
			ft_putstr_fd(cmd->tokens[1], STDERR_FILENO);
			ft_putstr_fd("': No such file or directory\n", STDERR_FILENO);
			mini->exit_sts = 127;
			return ;
		}
	}
	ft_setenv("_", "/usr/bin/env", &(mini->env));
	i = 0;
	while (mini->env[i])
	{
		if (ft_strchr(mini->env[i], '='))
		{
			ft_putstr_fd(mini->env[i], STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
		i++;
	}
	mini->exit_sts = 0;
}
