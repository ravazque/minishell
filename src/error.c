/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 19:45:49 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/15 19:51:10 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	malloc_error(void)
{
	ft_putstr_fd("malloc error\n", STDERR_FILENO);
	exit(STDOUT_FILENO);
}

void	split_error(void)
{
	ft_putstr_fd("split error\n", STDERR_FILENO);
	exit(STDOUT_FILENO);
}
