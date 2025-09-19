/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 22:54:02 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/19 06:26:44 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	builtin_env(t_mini mini)
{
	int		i;

	if (mini.cmds->tokens[1])
	{
		ft_putstr_fd("minishell: env: '", STDERR_FILENO);
		ft_putstr_fd(mini.cmds->tokens[1], STDERR_FILENO);
		ft_putstr_fd("': No such file or directory\n", STDERR_FILENO);
		return ;
	}
	i = 0;
	while (mini.env[i])
	{
		if (ft_strchr(mini.env[i], '='))
		{
			ft_putstr_fd(mini.env[i], STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
		i++;
	}
}
