/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 19:07:16 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/19 18:55:30 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	print_tokens(char **mini_cmds)
{
	int	i;

	i = 0;
	if (!mini_cmds)
	{
		printf("ERROR!\n");
		return ;
	}
	while (mini_cmds[i] != NULL)
	{
		printf("args[%d] = ( %s )\n", i, mini_cmds[i]);
		i++;
	}
	// (void)mini_cmds;
	// ft_putstr_fd("Error: command not found", STDERR_FILENO);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_mini	mini;

	interactive_err(argc, argv);
	init_mini(&mini, argc, argv, envp);
	if (argc == 1)
	{
		while (1)
		{
			mini.prompt = build_prompt(&mini);
			if (!mini.prompt)
				mini.prompt = ft_strdup("$ ");
			mini.input = readline(mini.prompt);
			ft_signal(&mini);
			if (*mini.input)
				add_history(mini.input);
			parse(&mini);
			if (built_ins(&mini) == false)
			{
				if (mini.cmds && mini.cmds->tokens)		//cambiar a futuro por la parte dejecutora
					print_tokens(mini.cmds->tokens);
			}
			cleanup_mini(&mini);
		}
	}
	else
	{
		mini.input = ft_strjoin(mini.input, argv[2]);
		parse(&mini);
		if (mini.cmds && mini.cmds->tokens && built_ins(&mini) == false)	//cambiar a futuro por la parte dejecutora
			print_tokens(mini.cmds->tokens);
	}
	cleanup_mini(&mini);
	rl_clear_history();
	return (mini.exit_sts);
}
