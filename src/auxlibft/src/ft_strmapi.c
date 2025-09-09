/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 18:26:42 by ravazque          #+#    #+#             */
/*   Updated: 2025/06/24 18:46:57 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	i;
	char			*nwstr;
	int				len;

	i = 0;
	len = ft_strlen(s);
	nwstr = (char *)malloc(len + 1);
	if (nwstr == NULL)
		return (NULL);
	while (s[i] != '\0')
	{
		nwstr[i] = f(i, s[i]);
		i++;
	}
	nwstr[i] = '\0';
	return (nwstr);
}
