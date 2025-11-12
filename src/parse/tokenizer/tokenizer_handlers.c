/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_handlers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 00:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/12 18:30:51 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	finalize_tok(char **bf, t_token_part **tp, t_cmd *cmd)
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

static int	handle_double_op(const char *in, size_t *i, char *op)
{
	int	len;

	len = 1;
	if (in[*i + 1] && ((in[*i] == '<' && in[*i + 1] == '<')
			|| (in[*i] == '>' && in[*i + 1] == '>')))
	{
		op[1] = in[*i + 1];
		len = 2;
		(*i)++;
	}
	op[len] = '\0';
	(*i)++;
	return (0);
}

int	handle_operator(const char *in, size_t *i, t_normi_ctx *ctx)
{
	char	op[3];

	if (ctx->in_tok && (ctx->bf || ctx->tp))
	{
		if (finalize_tok(&ctx->bf, &ctx->tp, ctx->cmd))
			return (1);
		ctx->in_tok = 0;
	}
	op[0] = in[*i];
	handle_double_op(in, i, op);
	ctx->bf = ft_strdup(op);
	if (!ctx->bf)
		return (1);
	if (flush_part(&ctx->bf, 0, 0, &ctx->tp))
		return (1);
	if (finalize_tok(&ctx->bf, &ctx->tp, ctx->cmd))
		return (1);
	ctx->in_tok = 0;
	return (0);
}

int	handle_space(const char *in, size_t *i, t_normi_ctx *ctx)
{
	if (ctx->in_tok)
	{
		if (finalize_tok(&ctx->bf, &ctx->tp, ctx->cmd))
			return (1);
		ctx->in_tok = 0;
	}
	while (in[*i] && is_space(in[*i]))
		(*i)++;
	return (0);
}

int	handle_quote(const char *in, size_t *i, t_normi_ctx *ctx)
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
	ctx->in_tok = 1;
	if (ctx->bf)
	{
		if (flush_part(&ctx->bf, 0, 0, &ctx->tp))
			return (1);
	}
	if (read_quoted(in, i, q, &ctx->bf))
		return (1);
	if (flush_part(&ctx->bf, sq, dq, &ctx->tp))
		return (1);
	return (0);
}
