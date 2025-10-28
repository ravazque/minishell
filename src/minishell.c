/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 19:07:16 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/23 16:55:53 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	non_interactive(t_mini *mini, char *command)
{
	mini->input = ft_strdup(command);
	if (!mini->input)
	{
		cleanup_mini(mini);
		exit(1);
	}
	parse(mini);
	if (mini->cmds)
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
	else if (argc == 3 && ft_strncmp(argv[1], "-c", 3) == 0)
		non_interactive(&mini, argv[2]);
	cleanup_mini(&mini);
	return (mini.exit_sts);
}
