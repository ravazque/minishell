/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_cmd_create.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 00:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/11 00:00:00 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static t_token_part	*copy_part(t_token_part *src)
{
	t_token_part	*new;

	new = (t_token_part *)malloc(sizeof(t_token_part));
	if (!new)
		return (NULL);
	new->content = ft_strdup(src->content);
	if (!new->content)
	{
		free(new);
		return (NULL);
	}
	new->is_squote = src->is_squote;
	new->is_dquote = src->is_dquote;
	new->next = NULL;
	return (new);
}

static int	copy_parts_list(t_token *new_tok, t_token_part *src_parts)
{
	t_token_part	*part_curr;
	t_token_part	*new_part;
	t_token_part	*last_part;

	part_curr = src_parts;
	last_part = NULL;
	while (part_curr)
	{
		new_part = copy_part(part_curr);
		if (!new_part)
			return (0);
		if (!new_tok->parts)
			new_tok->parts = new_part;
		else
			last_part->next = new_part;
		last_part = new_part;
		part_curr = part_curr->next;
	}
	return (1);
}

static t_token	*create_token_copy(t_token *curr)
{
	t_token	*new_tok;

	new_tok = mk_tok_node(curr->raw, curr->is_squote, curr->is_dquote);
	if (!new_tok)
		return (NULL);
	new_tok->is_assignment = curr->is_assignment;
	if (curr->parts)
	{
		if (!copy_parts_list(new_tok, curr->parts))
		{
			free_token_list(new_tok);
			return (NULL);
		}
	}
	return (new_tok);
}

static t_cmd	*alloc_new_cmd(void)
{
	t_cmd	*new_cmd;

	new_cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new_cmd)
		return (malloc_error(), NULL);
	ft_bzero(new_cmd, sizeof(t_cmd));
	return (new_cmd);
}

t_cmd	*create_cmd_from_tokens(t_token *start, int count)
{
	t_cmd	*new_cmd;
	t_token	*curr;
	t_token	*new_tok;
	int		i;

	new_cmd = alloc_new_cmd();
	if (!new_cmd)
		return (NULL);
	curr = start;
	i = 0;
	while (i < count && curr)
	{
		new_tok = create_token_copy(curr);
		if (!new_tok)
		{
			free_cmds(new_cmd);
			return (NULL);
		}
		add_tok(new_cmd, new_tok);
		curr = curr->next;
		i++;
	}
	return (new_cmd);
}
