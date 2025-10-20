/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 19:28:04 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/20 16:22:14 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	init_mini(t_mini *mini, int argc, char *argv[], char *envp[])
{
	char *path;

	mini->cmds = NULL;
	mini->prompt = NULL;
	mini->input = NULL;
	mini->pwd = NULL;
	mini->exit_sts = 0;
	mini->apology_mode = 0;
	mini->argc = argc;
	mini->cd_home = get_local_env("HOME", envp);
	mini->env = ft_copy_dblptr(envp);
	if (!mini->env)
	{
		ft_putstr_fd("minishell: fatal error: failed to copy environment\n", STDERR_FILENO);
		exit(1);
	}
	mini->argv = ft_copy_dblptr(argv);
	if (!mini->argv)
	{
		free_dblptr(mini->env);
		ft_putstr_fd("minishell: fatal error: failed to copy arguments\n", STDERR_FILENO);
		exit(1);
	}
	path = NULL;
	if (argv[0])
	{
		path = ft_strjoin("PATH=", get_local_env("PATH", envp));
		if (!path)
		{
			free_dblptr(mini->env);
			free_dblptr(mini->argv);
			ft_putstr_fd("minishell: fatal error: failed to alloc arguments\n", STDERR_FILENO);
			exit (1);
		}
		ft_setenv("_", path, &(mini->env));
		free(path);
	}
	else
		ft_setenv("_", "./minishell", &(mini->env));
}
