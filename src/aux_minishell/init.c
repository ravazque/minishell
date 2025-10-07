/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 19:28:04 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/08 00:56:11 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	init_mini(t_mini *mini, int argc, char *argv[], char *envp[])
{
	mini->cmds = NULL;
	mini->prompt = NULL;
	mini->input = NULL;
	mini->pwd = NULL;
	mini->exit_sts = 0;
	mini->argc = argc;
	mini->apology_mode = 0;
	mini->env = ft_copy_dblptr(envp);
	if (!mini->env)
	{
		ft_putstr_fd("minishell: fatal: cannot copy environment\n", 2);
		exit(1);
	}
	mini->argv = ft_copy_dblptr(argv);
	if (!mini->argv)
	{
		free_dblptr(mini->env);
		ft_putstr_fd("minishell: fatal: cannot copy arguments\n", 2);
		exit(1);
	}
}
