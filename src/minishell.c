/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 19:07:16 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/15 17:11:25 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc, char *argv[])
{
	t_mini	mini;

	mini.exit_sts = 0;
	if (argc == 2 || (argc > 2 && ft_strncmp(argv[1], "-c", 3) != 0))
		return (ft_putstr_fd(ERR_C, STDERR_FILENO), 127);
	if (argc == 1)
	{
		while (1)
		{
			mini.input = readline("minishell> ");
			if (mini.input == NULL)
				break ;
			if (*mini.input)
				add_history(mini.input);
			if (built_ins(mini) == true)
			{
				if (ft_strcmp_ns(mini.input, "exit") == 0)
				{
					free(mini.input);
					break ;
				}
				printf("built-in gestionado por ptrapero\n");
			}
			else
				printf("Input: [ %s ]\n", mini.input);
			free(mini.input);
		}
	}
	else
		non_interactive();
	rl_clear_history();
	return (mini.exit_sts);
}
