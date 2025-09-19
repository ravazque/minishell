/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 19:20:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/19 06:25:15 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	syntax_error(const char *near)
{
	ft_putstr_fd(ERR_TKN, STDERR_FILENO);
	ft_putstr_fd((char *)near, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
	return (2);
}

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

void	malloc_error(void)
{
	ft_putstr_fd("Error: malloc has failed\n", STDERR_FILENO);
	exit(EXIT_FAILURE);
}

void	split_error(void)
{
	ft_putstr_fd("Error: split has failed\n", STDERR_FILENO);
	exit(EXIT_FAILURE);
}
