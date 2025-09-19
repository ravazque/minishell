/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 11:59:02 by ravazque          #+#    #+#             */
/*   Updated: 2025/06/24 18:47:07 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int		i;
	char	*lastapr;

	i = 0;
	lastapr = NULL;
	while (s[i])
	{
		if (s[i] == (char)c)
			lastapr = (char *)&s[i];
		i++;
	}
	if (s[i] == (char)c)
		return ((char *)&s[i]);
	return (lastapr);
}
