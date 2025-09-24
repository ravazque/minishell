/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_aux.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 03:21:54 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/24 03:22:27 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	is_space(int c)
{
	if (c == ' ' || c == '\t')
		return (1);
	return (0);
}

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

int	quotes_balanced(const char *s)
{
	int		sq;
	int		dq;
	size_t	i;

	if (!s)
		return (1);
	sq = 0;
	dq = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] == '\'' && dq == 0)
			sq = !sq;
		else if (s[i] == '\"' && sq == 0)
			dq = !dq;
		i++;
	}
	if (sq == 0 && dq == 0)
		return (1);
	return (0);
}
