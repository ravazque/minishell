/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 22:54:02 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/30 17:57:30 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	print_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strchr(env[i], '='))
		{
			ft_putstr_fd(env[i], STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
		i++;
	}
}

void	builtin_env(t_mini *mini, t_cmd *cmd)
{
	if (!cmd->tokens[1])
	{
		ft_setenv("_", "/usr/bin/env", &(mini->env));
		print_env(mini->env);
		mini->exit_sts = 0;
		return ;
	}
	execute_env_command(mini, cmd);
}
