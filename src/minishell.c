/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 19:07:16 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/24 17:01:04 by ravazque         ###   ########.fr       */
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
		while (1)
		{
			mini.prompt = build_prompt(&mini);
			if (!mini.prompt)
				mini.prompt = ft_strdup("~ $ ");
			mini.input = readline(mini.prompt);
			ft_signal(&mini);
			if (mini.input && *mini.input)
				add_history(mini.input);
			parse(&mini);
			if (built_ins(&mini) == false)
			{
				if (mini.cmds)
					print_tokens(&mini);
			}
			free_args(&mini);
			if (mini.cmds)
			{
				free_cmds(mini.cmds);
				mini.cmds = NULL;
			}
		}
	}
	else
	{
		mini.input = ft_strdup(argv[2]);
		if (!mini.input)
		{
			cleanup_mini(&mini);
			return (1);
		}
		parse(&mini);
		if (mini.cmds && built_ins(&mini) == false)
			print_tokens(&mini);
	}
	
	cleanup_mini(&mini);
	rl_clear_history();
	return (mini.exit_sts);
}
