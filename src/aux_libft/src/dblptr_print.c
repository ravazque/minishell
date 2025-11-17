/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dblptr_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 00:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/06 13:06:23 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

void	dblptr_print(char **dblptr)
{
	int	i;

	if (!dblptr)
	{
		printf("(null)\n");
		return ;
	}
	i = 0;
	while (dblptr[i])
	{
		printf("[%d]: %s\n", i, dblptr[i]);
		i++;
	}
}
