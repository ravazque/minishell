/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 17:10:07 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/17 00:56:32 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void builtin_exit(bool **ex, t_mini *mini)
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

bool	built_ins(t_mini *mini, bool *ex)
{
	if (!mini->cmds || !mini->cmds->args || !mini->cmds->args[0])
		return (false);
	if (ft_strcmp(mini->cmds->args[0], "exit") == 0)
		return(builtin_exit(&ex, mini), true);
	else if (ft_strcmp(mini->cmds->args[0], "echo") == 0)
		return (printf("Paula's built-in\n"), *ex = false, true);
	else if (ft_strcmp(mini->cmds->args[0], "cd") == 0)
		return (printf("Paula's built-in\n"), *ex = false, true);
	else if (ft_strcmp(mini->cmds->args[0], "pwd") == 0)
		return (printf("Paula's built-in\n"), *ex = false, true);
	else if (ft_strcmp(mini->cmds->args[0], "export") == 0)
		return (printf("Paula's built-in\n"), *ex = false, true);
	else if (ft_strcmp(mini->cmds->args[0], "unset") == 0)
		return (printf("Paula's built-in\n"), *ex = false, true);
	else if (ft_strcmp(mini->cmds->args[0], "env") == 0)
		return (printf("Paula's built-in\n"), *ex = false, true);
	else
		return (false);
}
