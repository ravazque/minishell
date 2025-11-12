/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 00:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/11 00:00:00 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	set_redir_type(t_redir *r, const char *op)
{
	if (ft_strcmp(op, "<") == 0)
		r->in_redir = 1;
	else if (ft_strcmp(op, ">") == 0)
		r->out_redir = 1;
	else if (ft_strcmp(op, ">>") == 0)
		r->out_redir = 2;
}

static int	check_hd_quotes(t_token *tgt_tok)
{
	t_token_part	*part;

	if (tgt_tok && (tgt_tok->is_squote || tgt_tok->is_dquote))
		return (0);
	if (tgt_tok && tgt_tok->parts)
	{
		part = tgt_tok->parts;
		while (part)
		{
			if (part->is_squote || part->is_dquote)
				return (0);
			part = part->next;
		}
		return (1);
	}
	return (1);
}

static void	set_heredoc_expand(t_redir *r, t_token *tgt_tok)
{
	r->hd_expand = check_hd_quotes(tgt_tok);
}

static t_redir	*alloc_redir(const char *tgt)
{
	t_redir	*r;

	r = (t_redir *)malloc(sizeof(t_redir));
	if (!r)
		return (malloc_error(), NULL);
	ft_bzero(r, sizeof(t_redir));
	r->target = ft_strdup(tgt);
	if (!r->target)
	{
		free(r);
		return (malloc_error(), NULL);
	}
	return (r);
}

t_redir	*mk_redir(const char *tgt, const char *op, t_token *tgt_tok)
{
	t_redir	*r;

	r = alloc_redir(tgt);
	if (!r)
		return (NULL);
	if (ft_strcmp(op, "<<") == 0)
	{
		r->in_redir = 2;
		set_heredoc_expand(r, tgt_tok);
	}
	else
		set_redir_type(r, op);
	r->next = NULL;
	return (r);
}
