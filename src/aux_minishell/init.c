/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 19:28:04 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/30 17:42:54 by ravazque         ###   ########.fr       */
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
	mini->env = ft_copy_dblptr(envp);
	mini->argc = argc;
	mini->argv = ft_copy_dblptr(argv);
}
