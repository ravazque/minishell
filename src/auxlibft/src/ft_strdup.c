/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 11:59:02 by ravazque          #+#    #+#             */
/*   Updated: 2025/06/24 18:46:38 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

char	*ft_strdup(const char *s1)
{
	size_t	len;
	char	*dup_str;
	size_t	i;

	len = 0;
	while (s1[len] != '\0')
	{
		len++;
	}
	dup_str = (char *)malloc(len + 1);
	if (!dup_str)
	{
		return (NULL);
	}
	i = 0;
	while (i <= len)
	{
		dup_str[i] = s1[i];
		i++;
	}
	return (dup_str);
}
