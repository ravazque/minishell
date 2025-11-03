/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_split_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 18:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/03 18:25:34 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"

int	count_words(const char *str)
{
	int	count;
	int	in_word;
	int	i;

	count = 0;
	in_word = 0;
	i = 0;
	while (str[i])
	{
		if (is_space(str[i]))
			in_word = 0;
		else
		{
			if (in_word == 0)
			{
				count++;
				in_word = 1;
			}
		}
		i++;
	}
	return (count);
}

void	free_split_result(char **result, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (result[i])
			free(result[i]);
		i++;
	}
	free(result);
}

int	needs_word_split(t_token *tok)
{
	t_token_part	*curr;

	if (!tok)
		return (0);
	if (tok->is_squote || tok->is_dquote)
		return (0);
	if (is_assignment(tok->raw))
		return (0);
	if (!tok->parts)
		return (1);
	curr = tok->parts;
	while (curr)
	{
		if (!curr->is_squote && !curr->is_dquote)
			return (1);
		curr = curr->next;
	}
	return (0);
}
