/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ptrapero <ptrapero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 20:07:14 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/22 20:59:51 by ptrapero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	builtin_pwd(t_mini *mini)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		ft_putstr_fd(cwd, STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		free(cwd);
		mini->exit_sts = 0;
	}
	else if (mini->pwd)
	{
		ft_putstr_fd(mini->pwd, STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		mini->exit_sts = 0;
	}
	else
	{
		ft_putstr_fd("minishell: pwd: error retrieving current directory\n",
			STDERR_FILENO);
		mini->exit_sts = 1;
	}
}
