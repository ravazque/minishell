/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 19:07:16 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/09 22:22:59 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc, char *argv[])
{
	char	*input;

	if (argc == 2 || (argc > 2 && ft_strncmp(argv[1], "-c", 3) != 0))
		return (ft_putstr_fd(ERR_C, STDERR_FILENO), 127);
	if (argc == 1)
	{
		while (1)
		{
			input = readline("minishell> ");
			if (input == NULL)
				break ;
			if (*input)
				add_history(input);
			if (ft_strcmp_ns(input, "exit") == 0)
			{
				free(input);
				break ;
			}
			printf("Input: '%s'\n", input);
			free(input);
		}
	}
	// else
	// 	token_parsec_exec(argv[2], &data, false);
	rl_clear_history();
	return (0);
}
