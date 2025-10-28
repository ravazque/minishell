/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 03:22:36 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/28 16:13:48 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static t_token_part	*mk_tok_part(const char *c, int sq, int dq)
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

static void	add_tok_part(t_token_part **list, t_token_part *new)
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

static int	append_char(char **dst, char c)
{
	char	*old;
	size_t	len;
	size_t	i;
	char	*newp;

	old = *dst;
	len = 0;
	if (old)
		len = ft_strlen(old);
	newp = (char *)malloc(len + 2);
	if (!newp)
		return (1);
	i = 0;
	while (i < len)
	{
		newp[i] = old[i];
		i++;
	}
	newp[len] = c;
	newp[len + 1] = '\0';
	if (old)
		free(old);
	*dst = newp;
	return (0);
}

static int	flush_part(char **bf, int sq, int dq, t_token_part **parts)
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

static int	read_quoted(const char *in, size_t *i, char q, char **bf)
{
	size_t	start;

	start = *i;
	while (in[*i] && in[*i] != q)
		(*i)++;
	if (!in[*i])
		return (1);
	while (start < *i)
	{
		if (append_char(bf, in[start]))
			return (1);
		start++;
	}
	(*i)++;
	return (0);
}

static char	*join_parts(t_token_part *parts)
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

static int	is_operator_char(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (1);
	return (0);
}

void	mark_assignments(t_cmd *cmd)
{
	t_token	*curr;
	int		found_non_assignment;

	if (!cmd || !cmd->tokn)
		return ;
	found_non_assignment = 0;
	curr = cmd->tokn;
	while (curr)
	{
		if (is_operator_char(curr->raw[0]))
			found_non_assignment = 1;
		if (!found_non_assignment && is_assignment(curr->raw))
			curr->is_assignment = 1;
		else
		{
			curr->is_assignment = 0;
			if (!is_space(curr->raw[0]))
				found_non_assignment = 1;
		}
		curr = curr->next;
	}
}

static t_token	*mk_tok_from_parts(t_token_part *parts)
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

static void	cleanup_tok_data(char **bf, t_token_part **p, t_cmd **cmd)
{
	if (bf && *bf)
	{
		free(*bf);
		*bf = NULL;
	}
	if (p && *p)
	{
		free_token_parts(*p);
		*p = NULL;
	}
	if (cmd && *cmd)
	{
		free_cmds(*cmd);
		*cmd = NULL;
	}
}

static int	add_tok_to_cmd(t_token *new, t_cmd *cmd)
{
	t_token	*curr;

	if (!cmd->tokn)
		cmd->tokn = new;
	else
	{
		curr = cmd->tokn;
		while (curr->next)
			curr = curr->next;
		curr->next = new;
	}
	return (0);
}

static int	finalize_tok(char **bf, t_token_part **tp, t_cmd *cmd)
{
	t_token	*new;

	if (*bf)
	{
		if (flush_part(bf, 0, 0, tp))
			return (1);
	}
	if (*tp)
	{
		new = mk_tok_from_parts(*tp);
		if (!new)
			return (1);
		if (add_tok_to_cmd(new, cmd))
			return (1);
		*tp = NULL;
	}
	return (0);
}

static int	handle_operator(const char *in, size_t *i, int *in_tok, char **bf, t_token_part **tp, t_cmd *cmd)
{
	char	op[3];
	int		len;

	if (*in_tok && (*bf || *tp))
	{
		if (finalize_tok(bf, tp, cmd))
			return (1);
		*in_tok = 0;
	}
	len = 0;
	op[0] = in[*i];
	len = 1;
	if (in[*i + 1] && ((in[*i] == '<' && in[*i + 1] == '<') || (in[*i] == '>' && in[*i + 1] == '>')))
	{
		op[1] = in[*i + 1];
		len = 2;
		(*i)++;
	}
	op[len] = '\0';
	(*i)++;
	*bf = ft_strdup(op);
	if (!*bf)
		return (1);
	if (flush_part(bf, 0, 0, tp))
		return (1);
	if (finalize_tok(bf, tp, cmd))
		return (1);
	*in_tok = 0;
	return (0);
}

static int	handle_space(const char *in, size_t *i, int *in_tok, char **bf, t_token_part **tp, t_cmd *cmd)
{
	if (*in_tok)
	{
		if (finalize_tok(bf, tp, cmd))
			return (1);
		*in_tok = 0;
	}
	while (in[*i] && is_space(in[*i]))
		(*i)++;
	return (0);
}

static int	handle_quote(const char *in, size_t *i, int *in_tok, char **bf, t_token_part **tp)
{
	char	q;
	int		sq;
	int		dq;

	q = in[*i];
	sq = 0;
	dq = 0;
	if (q == '\'')
		sq = 1;
	else
		dq = 1;
	(*i)++;
	*in_tok = 1;
	if (*bf)
	{
		if (flush_part(bf, 0, 0, tp))
			return (1);
	}
	if (read_quoted(in, i, q, bf))
		return (1);
	if (flush_part(bf, sq, dq, tp))
		return (1);
	return (0);
}

static int	handle_regular(const char *in, size_t *i, int *in_tok, char **bf)
{
	*in_tok = 1;
	if (append_char(bf, in[*i]))
		return (1);
	(*i)++;
	return (0);
}

int	tokenizer(t_mini **mini)
{
	char			*in;
	size_t			i;
	char			*bf;
	t_token_part	*tp;
	t_cmd			*cmd;
	int				in_tok;

	in = (*mini)->input;
	i = 0;
	bf = NULL;
	tp = NULL;
	in_tok = 0;
	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
		return (1);
	ft_bzero(cmd, sizeof(t_cmd));
	while (in && in[i])
	{
		if (is_space(in[i]))
		{
			if (handle_space(in, &i, &in_tok, &bf, &tp, cmd))
			{
				cleanup_tok_data(&bf, &tp, &cmd);
				return (1);
			}
		}
		else if (in[i] == '\'' || in[i] == '\"')
		{
			if (handle_quote(in, &i, &in_tok, &bf, &tp))
			{
				cleanup_tok_data(&bf, &tp, &cmd);
				return (1);
			}
		}
		else if (is_operator_char(in[i]))
		{
			if (handle_operator(in, &i, &in_tok, &bf, &tp, cmd))
			{
				cleanup_tok_data(&bf, &tp, &cmd);
				return (1);
			}
		}
		else
		{
			if (handle_regular(in, &i, &in_tok, &bf))
			{
				cleanup_tok_data(&bf, &tp, &cmd);
				return (1);
			}
		}
	}
	if (in_tok)
	{
		if (finalize_tok(&bf, &tp, cmd))
		{
			cleanup_tok_data(&bf, &tp, &cmd);
			return (1);
		}
	}
	(*mini)->cmds = cmd;
	return (0);
}
