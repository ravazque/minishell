/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 00:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/11 00:00:00 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

t_token	*mk_tok_node(const char *raw, int sq, int dq)
{
	t_token	*tok;

	tok = (t_token *)malloc(sizeof(t_token));
	if (!tok)
		return (malloc_error(), NULL);
	tok->raw = ft_strdup(raw);
	if (!tok->raw)
	{
		free(tok);
		return (malloc_error(), NULL);
	}
	tok->is_squote = sq;
	tok->is_dquote = dq;
	tok->is_assignment = 0;
	tok->parts = NULL;
	tok->next = NULL;
	return (tok);
}

void	add_tok(t_cmd *cmd, t_token *new)
{
	t_token	*curr;

	if (!cmd->tokn)
	{
		cmd->tokn = new;
		return ;
	}
	curr = cmd->tokn;
	while (curr->next)
		curr = curr->next;
	curr->next = new;
}
