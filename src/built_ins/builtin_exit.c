/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 11:50:27 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/22 17:36:39 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	builtin_exit(t_mini *mini, char *end)
{
	bool	max_ex;

	max_ex = false;
	if (ft_strcmp("bye", end) == 0)
		ft_putstr_fd("bye bye!\n", STDERR_FILENO);
	else
		ft_putstr_fd("exit\n", STDERR_FILENO);
	if (mini->cmds->tokens[1])
	{
		mini->exit_sts = ft_atoi_exit(mini->cmds->tokens[1], &max_ex);
		if (max_ex == true)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(end, STDERR_FILENO);
			ft_putstr_fd(": ", STDERR_FILENO);
			ft_putstr_fd(mini->cmds->tokens[1], STDERR_FILENO);
			write(STDERR_FILENO, ": numeric argument required\n", 28);
			mini->exit_sts = 2;
			return ;
		}
	}
	if (mini->cmds->tokens[1] && mini->cmds->tokens[2])
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(end, STDERR_FILENO);
		ft_putstr_fd(": too many arguments\n", STDERR_FILENO);
		mini->exit_sts = 2;
		return ;
	}
	cleanup_mini(mini);
	exit(mini->exit_sts);
}
