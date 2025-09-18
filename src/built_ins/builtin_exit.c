/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 11:50:27 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/17 11:52:44 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void builtin_exit(bool **ex, t_mini *mini)
{
	bool max_ex;
	
	max_ex = false;
	write(STDOUT_FILENO, "exit\n", 5);
	if (mini->cmds->args[2])
	{
		mini->exit_sts = 2;
		write(STDERR_FILENO, "Error: exit: too many arguments\n", 32);
		**ex = false;
		return ;
	}
	if (mini->cmds->args[1])
	{
		mini->exit_sts = ft_atoi(mini->cmds->args[1], &max_ex);
		if(max_ex == true)
		{
			write(STDERR_FILENO, "Error: exit: ", 13);
			ft_putstr_fd(mini->cmds->args[1], STDERR_FILENO);
			write(STDERR_FILENO, ": numeric argument required\n", 28);
			**ex = false;
			return ;
		}
	}
	**ex = true;
}
