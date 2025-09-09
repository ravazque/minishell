/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp_ns.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 17:55:59 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/09 23:17:31 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

int	ft_strcmp_ns(const char *s1, const char *s2)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s1[i] && (s1[i] == ' ' || s1[i] == '\t'))
		i++;
	while (s2[j] && (s2[j] == ' ' || s2[j] == '\t'))
		j++;
	while (s1[i] && s2[j])
	{
		if (s1[i] != s2[j])
			return (-1);
		i++;
		j++;
	}
	while (s1[i] && (s1[i] == ' ' || s1[i] == '\t'))
		i++;
	while (s2[j] && (s2[j] == ' ' || s2[j] == '\t'))
		j++;
	if (s1[i] == '\0' && s2[j] == '\0')
		return (0);
	return (-1);
}
