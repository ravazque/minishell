/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_split_count.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 18:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/03 18:25:26 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"

static int	count_split_words(char **split)
{
	int	i;

	i = 0;
	while (split[i])
		i++;
	return (i);
}

static int	count_split_tokens(char *expanded, t_token *curr)
{
	char	**split;
	int		count;

	if (needs_word_split(curr) && !is_empty_str(expanded))
	{
		split = split_by_spaces(expanded);
		free(expanded);
		if (split)
		{
			count = count_split_words(split);
			free_dblptr(split);
			return (count);
		}
		return (0);
	}
	else
	{
		free(expanded);
		return (1);
	}
}

static int	handle_empty_expansion(char *expanded, t_token *curr)
{
	if (is_empty_str(expanded) && !tok_has_quotes(curr))
	{
		free(expanded);
		return (1);
	}
	return (0);
}

int	count_expanded_tokens(t_token *tokens, t_mini *mini)
{
	t_token	*curr;
	char	*expanded;
	int		total;

	total = 0;
	curr = tokens;
	while (curr)
	{
		if (curr->is_assignment)
		{
			curr = curr->next;
			continue ;
		}
		expanded = exp_tok_parts(curr, mini);
		if (!expanded)
			return (-1);
		if (handle_empty_expansion(expanded, curr))
		{
			curr = curr->next;
			continue ;
		}
		total = total + count_split_tokens(expanded, curr);
		curr = curr->next;
	}
	return (total);
}
