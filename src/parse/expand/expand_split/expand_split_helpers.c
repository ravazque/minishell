/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_split_helpers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 18:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/03 18:25:29 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"

void	skip_spaces(const char *str, int *start)
{
	while (str[*start] && is_space(str[*start]))
		(*start)++;
}

int	get_word_len(const char *str, int start)
{
	int	len;
	int	i;

	len = 0;
	i = start;
	while (str[i] && !is_space(str[i]))
	{
		len++;
		i++;
	}
	return (len);
}

char	*alloc_and_copy(const char *str, int start, int len)
{
	char	*word;
	int		j;

	if (len == 0)
		return (NULL);
	word = (char *)malloc(len + 1);
	if (!word)
		return (NULL);
	j = 0;
	while (j < len)
	{
		word[j] = str[start + j];
		j++;
	}
	word[len] = '\0';
	return (word);
}
