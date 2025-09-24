/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 11:50:27 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/24 16:45:15 by ravazque         ###   ########.fr       */
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
		mini->exit_sts = 2;
		write(STDERR_FILENO, "minishell: exit: too many arguments\n", 36);
		return ;
	}
	
	if (mini->cmds->tokens[1])
	{
		mini->exit_sts = ft_atoi_exit(mini->cmds->tokens[1], &max_ex);
		if (max_ex == true)
		{
			write(STDERR_FILENO, "minishell: exit: ", 17);
			ft_putstr_fd(mini->cmds->tokens[1], STDERR_FILENO);
			write(STDERR_FILENO, ": numeric argument required\n", 28);
			mini->exit_sts = 2;
			cleanup_mini(mini);
			exit(mini->exit_sts);
		}
	}
	cleanup_mini(mini);
	exit(mini->exit_sts);
}
