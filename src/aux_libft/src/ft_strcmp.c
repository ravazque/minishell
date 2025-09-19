/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 11:59:02 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/16 17:38:56 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i])
	{
		i++;
	}
	if (s1[i] != s2[i])
	{
		return (-1);
	}
	i = 0;
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
		{
			return (-1);
		}
		i++;
	}
	return (0);
}
