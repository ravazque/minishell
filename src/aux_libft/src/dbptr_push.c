/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dbptr_push.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 17:27:43 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/22 17:25:32 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static size_t	dblptr_len(char **p)
{
	size_t	n;

	n = 0;
	if (!p)
		return (0);
	while (p[n])
		n++;
	return (n);
}

char	**dbpt_push(char ***dst, const char *s)
{
	size_t	n;
	size_t	i;
	char	**newp;

	n = dblptr_len(*dst);
	newp = (char **)malloc(sizeof(char *) * (n + 2));
	if (!newp)
		malloc_error();
	i = 0;
	while (i < n)
	{
		newp[i] = (*dst)[i];
		i++;
	}
	newp[n] = ft_strdup(s);
	if (!newp[n])
	{
		free(newp);
		malloc_error();
	}
	newp[n + 1] = NULL;
	if (*dst)
		free(*dst);
	*dst = newp;
	return (*dst);
}
