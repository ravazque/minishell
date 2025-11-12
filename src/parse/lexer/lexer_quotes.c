/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 00:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/12 15:08:43 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	check_part_quotes(t_token_part *part)
{
	while (part)
	{
		if (part->is_squote)
			return (1);
		if (part->is_dquote)
			return (1);
		part = part->next;
	}
	return (0);
}

int	tok_has_any_quotes(t_token *tok)
{
	if (!tok)
		return (0);
	if (tok->is_squote)
		return (1);
	if (tok->is_dquote)
		return (1);
	if (!tok->parts)
		return (0);
	return (check_part_quotes(tok->parts));
}
