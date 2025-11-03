/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_split_add.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 18:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/03 18:25:19 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"

static int	add_split_tokens(char **result, int *j, char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		result[*j] = split[i];
		(*j)++;
		i++;
	}
	free(split);
	return (0);
}

static int	add_single_token(char **result, int *j, char *expanded)
{
	result[*j] = expanded;
	(*j)++;
	return (0);
}

static int	handle_split_expansion(char **result, int *j, char *expanded)
{
	char	**split;

	split = split_by_spaces(expanded);
	free(expanded);
	if (!split)
		return (1);
	return (add_split_tokens(result, j, split));
}

int	add_expanded_tokens(char **result, int *j, t_token *tok, t_mini *mini)
{
	char	*expanded;

	expanded = exp_tok_parts(tok, mini);
	if (!expanded)
		return (1);
	if (is_empty_str(expanded) && !tok_has_quotes(tok))
	{
		free(expanded);
		return (0);
	}
	if (needs_word_split(tok) && !is_empty_str(expanded))
		return (handle_split_expansion(result, j, expanded));
	else
		return (add_single_token(result, j, expanded));
}
