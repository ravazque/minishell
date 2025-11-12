/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 00:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/12 00:00:00 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	append_char(char **dst, char c)
{
	char	*old;
	size_t	len;
	size_t	i;
	char	*newp;

	old = *dst;
	len = 0;
	if (old)
		len = ft_strlen(old);
	newp = (char *)malloc(len + 2);
	if (!newp)
		return (1);
	i = 0;
	while (i < len)
	{
		newp[i] = old[i];
		i++;
	}
	newp[len] = c;
	newp[len + 1] = '\0';
	if (old)
		free(old);
	*dst = newp;
	return (0);
}

int	read_quoted(const char *in, size_t *i, char q, char **bf)
{
	size_t	start;

	start = *i;
	while (in[*i] && in[*i] != q)
		(*i)++;
	if (!in[*i])
		return (1);
	while (start < *i)
	{
		if (append_char(bf, in[start]))
			return (1);
		start++;
	}
	(*i)++;
	return (0);
}
