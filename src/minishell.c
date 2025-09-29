/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 19:07:16 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/29 19:32:44 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
	{
		mini.input = ft_strdup(argv[2]);
		if (!mini.input)
			return (cleanup_mini(&mini), 1);
		parse(&mini);
		if (mini.cmds && built_ins(&mini) == false)
			print_tokens(&mini);
	}
	cleanup_mini(&mini);
	rl_clear_history();
	return (mini.exit_sts);
}
