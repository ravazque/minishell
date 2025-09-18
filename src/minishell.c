/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 19:07:16 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/18 16:41:34 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	print_args(char **mini_cmds)
{
	int	i;

	i = 0;
	if (!mini_cmds)
	{
		printf("The arguments are empty\n");
		return ;
	}
	while (mini_cmds[i] != NULL)
	{
		printf("args[%d] = ( %s )\n", i, mini_cmds[i]);
		i++;
	}
}

int	main(int argc, char *argv[])
{
	t_mini	mini;

	interactive_err(argc, argv);
	init_mini(&mini);
	if (argc == 1)
	{
		while (1)
		{
			mini.prompt = ms_make_prompt();
			if (!mini.prompt)
				mini.prompt = ft_strdup("$ ");
			mini.input = readline(mini.prompt);
			free(mini.prompt);
			if (mini.input == NULL)
				break ;
			if (*mini.input)
				add_history(mini.input);
			parse(&mini);
			if (built_ins(&mini) == true)
				break ;
			if (mini.cmds && mini.cmds->args)
				print_args(mini.cmds->args);
			cleanup_mini(&mini);
		}
	}
	else
	{
		mini.input = ft_strjoin(mini.input, argv[2]);
		parse(&mini);
		if (mini.cmds && mini.cmds->args && built_ins(&mini) == false)
			print_args(mini.cmds->args);
	}
	cleanup_mini(&mini);
	rl_clear_history();
	return (mini.exit_sts);
}
