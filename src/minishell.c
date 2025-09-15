/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 19:07:16 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/15 20:51:23 by ravazque         ###   ########.fr       */
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
		printf("args[%d] = %s\n", i, mini_cmds[i]);
		i++;
	}
}

int	main(int argc, char *argv[])
{
	t_mini	mini;

	if (argc == 2 || (argc > 2 && ft_strncmp(argv[1], "-c", 3) != 0))
		return (ft_putstr_fd(ERR_C, STDERR_FILENO), 127);
	init_mini(&mini);
	if (argc == 1)
	{
		while (1)
		{
			mini.input = readline("minishell> ");
			if (mini.input == NULL)
				break ;
			if (*mini.input)
				add_history(mini.input);
			parse(&mini);
			if (built_ins(mini) == true)
			{
				if (ft_strcmp_ns(mini.cmds->args[0], "exit") == 0)
				{
					free(mini.input);
					break ;
				}
				printf("Paula's built-in\n");
			}
			else
			{
				if (mini.cmds && mini.cmds->args)
					print_args(mini.cmds->args);
			}
			if (mini.cmds)
			{
				free_cmds(mini.cmds);
				mini.cmds = NULL;
			}
			if (mini.input)
			{
				free(mini.input);
				mini.input = NULL;
			}
		}
	}
	else
		non_interactive();
	cleanup_mini(&mini);
	rl_clear_history();
	return (mini.exit_sts);
}
