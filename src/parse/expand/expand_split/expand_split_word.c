/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_split_word.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 18:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/03 18:25:39 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"

char	*extract_word(const char *str, int *start)
{
	int		len;
	char	*word;

	skip_spaces(str, start);
	len = get_word_len(str, *start);
	word = alloc_and_copy(str, *start, len);
	if (!word)
		return (NULL);
	*start = *start + len;
	return (word);
}

static char	**fill_result(const char *str, int word_count)
{
	char	**result;
	int		pos;
	int		i;
	char	*word;

	result = (char **)malloc(sizeof(char *) * (word_count + 1));
	if (!result)
		return (NULL);
	pos = 0;
	i = 0;
	while (i < word_count)
	{
		word = extract_word(str, &pos);
		if (!word)
		{
			free_split_result(result, i);
			return (NULL);
		}
		result[i] = word;
		i++;
	}
	result[word_count] = NULL;
	return (result);
}

char	**split_by_spaces(const char *str)
{
	int	word_count;

	if (!str)
		return (NULL);
	word_count = count_words(str);
	if (word_count == 0)
		return (NULL);
	return (fill_result(str, word_count));
}
