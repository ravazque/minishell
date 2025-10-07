/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 19:20:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/07 23:20:15 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	interactive_err(int argc, char *argv[])
{
	if (argc > 2 && ft_strncmp(argv[1], "-c", 3) != 0)
	{
		ft_putstr_fd(ERR_C, STDERR_FILENO);
		exit(127);
	}
	else if (argc == 2 && ft_strncmp(argv[1], "-c", 3) == 0)
	{
		ft_putstr_fd(ERR_C_ARG, STDERR_FILENO);
		exit(127);
	}
	else if (argc == 2 && ft_strncmp(argv[1], "-c", 3) != 0)
	{
		ft_putstr_fd(ERR_C, STDERR_FILENO);
		exit(127);
	}
}

void	malloc_error(void) // hay que hacer sus respectivos free
{
	ft_putstr_fd("minishell: error: malloc has failed\n", STDERR_FILENO);
	return ;
}
