/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 19:20:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/23 17:30:51 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	interactive_err(int argc, char *argv[])
{
	if (argc > 2 && ft_strcmp(argv[1], "-c") != 0)
	{
		ft_putstr_fd(ERR_C, STDERR_FILENO);
		exit(127);
	}
	else if (argc == 2 && ft_strcmp(argv[1], "-c") == 0)
	{
		ft_putstr_fd(ERR_C_ARG, STDERR_FILENO);
		exit(127);
	}
	else if (argc == 2 && ft_strcmp(argv[1], "-c") != 0)
	{
		ft_putstr_fd(ERR_C, STDERR_FILENO);
		exit(127);
	}
	else if (argc == 4)
	{
		ft_putstr_fd("minishell: error: too many arguments\n", STDERR_FILENO);
		exit(127);
	}
}

void	malloc_error(void)
{
	ft_putstr_fd("minishell: error: malloc has failed\n", STDERR_FILENO);
	return ;
}
