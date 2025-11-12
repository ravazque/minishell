/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_parts.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 00:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/12 00:00:00 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

t_token_part	*mk_tok_part(const char *c, int sq, int dq)
{
	t_token_part	*part;

	part = (t_token_part *)malloc(sizeof(t_token_part));
	if (!part)
		return (NULL);
	if (!c)
		c = "";
	part->content = ft_strdup(c);
	if (!part->content)
	{
		free(part);
		return (NULL);
	}
	part->is_squote = sq;
	part->is_dquote = dq;
	part->next = NULL;
	return (part);
}

void	add_tok_part(t_token_part **list, t_token_part *new)
{
	t_token_part	*curr;

	if (!*list)
	{
		*list = new;
		return ;
	}
	curr = *list;
	while (curr->next)
		curr = curr->next;
	curr->next = new;
}

int	flush_part(char **bf, int sq, int dq, t_token_part **parts)
{
	t_token_part	*new;
	const char		*content;

	content = "";
	if (*bf)
		content = *bf;
	new = mk_tok_part(content, sq, dq);
	if (!new)
		return (1);
	add_tok_part(parts, new);
	if (*bf)
	{
		free(*bf);
		*bf = NULL;
	}
	return (0);
}

char	*join_parts(t_token_part *parts)
{
	char			*res;
	char			*tmp;
	t_token_part	*curr;

	res = NULL;
	curr = parts;
	while (curr)
	{
		tmp = res;
		res = ft_strjoin(res, curr->content);
		if (tmp)
			free(tmp);
		if (!res)
			return (NULL);
		curr = curr->next;
	}
	if (!res)
		res = ft_strdup("");
	return (res);
}

t_token	*mk_tok_from_parts(t_token_part *parts)
{
	t_token	*tok;
	char	*joined;

	if (!parts)
		return (NULL);
	tok = (t_token *)malloc(sizeof(t_token));
	if (!tok)
		return (NULL);
	joined = join_parts(parts);
	if (!joined)
	{
		free(tok);
		return (NULL);
	}
	tok->raw = joined;
	tok->is_squote = 0;
	tok->is_dquote = 0;
	tok->parts = parts;
	tok->next = NULL;
	return (tok);
}
