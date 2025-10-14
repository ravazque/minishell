/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 19:07:16 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/14 20:55:05 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	execute_command_mode(t_mini *mini, char *command)
{
	mini->input = ft_strdup(command);
	if (!mini->input)
	{
		cleanup_mini(mini);
		exit(1);
	}
	parse(mini);
	if (mini->cmds && built_ins(mini) == false)
		executor(mini);
	cleanup_mini(mini);
	exit(mini->exit_sts);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_mini	mini;

	interactive_err(argc, argv);
	init_mini(&mini, argc, argv, envp);
	if (argc == 1)
	{
		setup_interactive_signals();
		loop(&mini);
	}
	else
		execute_command_mode(&mini, argv[2]);
	cleanup_mini(&mini);
	return (mini.exit_sts);
}
