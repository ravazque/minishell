/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 11:50:27 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/14 20:00:53 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	handle_invalid_arg(t_mini *mini, char *arg)
{
	write(STDERR_FILENO, "minishell: exit: ", 17);
	ft_putstr_fd(arg, STDERR_FILENO);
	write(STDERR_FILENO, ": numeric argument required\n", 28);
	mini->exit_sts = 2;
}

void	builtin_exit(t_mini *mini)
{
	bool	max_ex;
	int		exit_code;

	max_ex = false;
	write(STDOUT_FILENO, "exit\n", 5);
	if (mini->cmds->tokens[1])
	{
		mini->exit_sts = ft_atoi_exit(mini->cmds->tokens[1], &max_ex);
		if (max_ex == true)
			return (handle_invalid_arg(mini, mini->cmds->tokens[1]));
	}
	if (mini->cmds->tokens[1] && mini->cmds->tokens[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		mini->exit_sts = 2;
		return ;
	}
	exit_code = mini->exit_sts;
	cleanup_mini(mini);
	exit(exit_code);
}

// `exit` acts as in the latest version of `bash` with regard to whether or not
// it should actually exit and some `exit status`.
