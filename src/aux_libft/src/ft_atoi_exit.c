/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 19:23:30 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/18 17:01:33 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

static size_t	skip_ws(const char *s, size_t i)
{
	while (s[i] == ' ' || (s[i] >= 9 && s[i] <= 13))
		i++;
	return (i);
}

static size_t	read_sign(const char *s, size_t i, int *sign)
{
	*sign = 1;
	if (s[i] == '-' || s[i] == '+')
	{
		if (s[i] == '-')
			*sign = -1;
		i++;
	}
	return (i);
}

static int	add_digit(unsigned long long *n, int sign, char c)
{
	unsigned long long	limit;
	unsigned long long	d;

	limit = (unsigned long long)LLONG_MAX;
	if (sign != 1)
		limit = limit + 1;
	d = (unsigned long long)(c - '0');
	if (*n > (limit - d) / 10)
		return (0);
	*n = (*n * 10) + d;
	return (1);
}

static int	parse_digits(const char *s, size_t *i, int sign, bool *bad)
{
	unsigned long long	n;
	int					mod;

	n = 0;
	mod = 0;
	while (s[*i] >= '0' && s[*i] <= '9')
	{
		if (!add_digit(&n, sign, s[*i]))
		{
			*bad = true;
			return (2);
		}
		mod = mod * 10 + (s[*i] - '0');
		while (mod >= 256)
			mod -= 256;
		(*i)++;
	}
	if (sign == -1 && mod != 0)
		mod = 256 - mod;
	return (mod);
}

int	ft_atoi_exit(const char *s, bool *max_ex)
{
	size_t	i;
	int		sign;
	int		mod;

	if (max_ex)
		*max_ex = false;
	i = skip_ws(s, 0);
	i = read_sign(s, i, &sign);
	if (s[i] < '0' || s[i] > '9')
	{
		*max_ex = true;
		return (2);
	}
	mod = parse_digits(s, &i, sign, max_ex);
	i = skip_ws(s, i);
	if (s[i] != '\0')
	{
		*max_ex = true;
		return (2);
	}
	return (mod);
}
