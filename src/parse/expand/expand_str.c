/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_str.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 18:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/03 18:00:00 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static char	*alloc_result(char *dst, const char *src)
{
	char	*res;
	int		dlen;
	int		slen;

	dlen = 0;
	if (dst)
		dlen = ft_strlen(dst);
	slen = ft_strlen(src);
	res = (char *)malloc(dlen + slen + 1);
	return (res);
}

static void	copy_strings(char *res, char *dst, const char *src)
{
	int	i;
	int	j;
	int	dlen;

	i = 0;
	dlen = 0;
	if (dst)
		dlen = ft_strlen(dst);
	while (i < dlen && dst)
	{
		res[i] = dst[i];
		i++;
	}
	j = 0;
	while (src[j])
	{
		res[i + j] = src[j];
		j++;
	}
	res[i + j] = '\0';
}

char	*str_cat(char *dst, const char *src)
{
	char	*res;

	if (!src)
		return (dst);
	res = alloc_result(dst, src);
	if (!res)
	{
		if (dst)
			free(dst);
		return (NULL);
	}
	copy_strings(res, dst, src);
	if (dst)
		free(dst);
	return (res);
}

char	*str_cat_chr(char *dst, char c)
{
	char	tmp[2];

	tmp[0] = c;
	tmp[1] = '\0';
	return (str_cat(dst, tmp));
}
