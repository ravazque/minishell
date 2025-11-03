/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 18:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/03 18:00:00 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	is_empty_str(const char *s)
{
	if (!s)
		return (1);
	if (s[0] == '\0')
		return (1);
	return (0);
}

int	tok_has_quotes(t_token *tok)
{
	t_token_part	*curr;

	if (!tok)
		return (0);
	if (!tok->parts)
		return (0);
	curr = tok->parts;
	while (curr)
	{
		if (curr->is_squote || curr->is_dquote)
			return (1);
		curr = curr->next;
	}
	return (0);
}
