/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 11:50:27 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/08 01:01:01 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	builtin_exit(t_mini *mini)
{
	bool	max_ex;

	max_ex = false;
	write(STDOUT_FILENO, "exit\n", 5);
	if (mini->cmds->tokens[1] && mini->cmds->tokens[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		mini->exit_sts = 2;
		return ;
	}
	mini->exit_sts = 0;
	if (mini->cmds->tokens[1])
	{
		mini->exit_sts = ft_atoi_exit(mini->cmds->tokens[1], &max_ex);
		if (max_ex == true)
		{
			write(STDERR_FILENO, "minishell: exit: ", 17);
			ft_putstr_fd(mini->cmds->tokens[1], STDERR_FILENO);
			write(STDERR_FILENO, ": numeric argument required\n", 28);
			mini->exit_sts = 2;
			return ;
		}
	}
	cleanup_mini(mini);
	exit(mini->exit_sts);
}
