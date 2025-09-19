/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 18:26:42 by ravazque          #+#    #+#             */
/*   Updated: 2025/06/24 18:46:27 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

static void	freemem(char **res, int words)
{
	int	i;

	i = 0;
	while (i < words)
	{
		free(res[i]);
		i++;
	}
	free(res);
}

static int	cntwrd(char const *s, char c)
{
	int	i;
	int	wrd;

	i = 0;
	wrd = 0;
	while (s[i])
	{
		if (s[i] != c && (i == 0 || s[i - 1] == c))
			wrd++;
		i++;
	}
	return (wrd);
}

static int	wdlen(char const *s, char c)
{
	int	len;

	len = 0;
	while (s[len] && s[len] != c)
		len++;
	return (len);
}

static char	**place_res(char const *s, char c, char **res)
{
	int	i;
	int	j;
	int	aux;

	i = 0;
	j = 0;
	while (s[i])
	{
		if (s[i] == c)
			i++;
		else
		{
			aux = wdlen(&s[i], c);
			res[j] = (char *)malloc((aux + 1) * sizeof(char));
			if (!res[j])
			{
				return (freemem(res, j), NULL);
			}
			aux = 0;
			while (s[i] && s[i] != c)
				res[j][aux++] = s[i++];
			res[j++][aux] = '\0';
		}
	}
	return (res[j] = NULL, res);
}

char	**ft_split(char const *s, char c)
{
	int		wrd;
	char	**res;

	if (!s)
		return (NULL);
	wrd = cntwrd(s, c);
	res = (char **)malloc((wrd + 1) * sizeof(char *));
	if (!res)
		return (NULL);
	return (place_res(s, c, res));
}
