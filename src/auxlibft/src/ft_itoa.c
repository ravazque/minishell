/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 17:13:12 by ravazque          #+#    #+#             */
/*   Updated: 2025/06/24 18:45:13 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

static char	*assist(int n, char *ext, int len, int sign)
{
	int	i;

	if (sign == 1)
		ext[0] = '-';
	i = len;
	while (n > 0 && i >= 0)
	{
		ext[i] = (n % 10) + '0';
		n /= 10;
		i--;
	}
	ext[len + 1] = 0;
	return (ext);
}

static int	start(int n)
{
	int	len;

	len = 0;
	while (n / 10)
	{
		len++;
		n = n / 10;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	int		sign;
	int		len;
	int		aux;
	char	*ext;

	len = 0;
	sign = 0;
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	if (n == 0)
		return (ft_strdup("0"));
	if (n < 0)
	{
		sign = 1;
		n = -n;
		len++;
	}
	aux = n;
	len += start(aux);
	ext = malloc(sizeof(char) * (len + 2));
	if (!ext)
		return (NULL);
	return (ext[len] = 0, assist(n, ext, len, sign));
}
