/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 19:45:49 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/16 19:34:50 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
