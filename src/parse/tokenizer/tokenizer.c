/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 03:22:36 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/12 18:30:03 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

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

static int	init_ctx(t_normi_ctx*ctx, t_mini *mini)
{
	ctx->mini = mini;
	ctx->bf = NULL;
	ctx->tp = NULL;
	ctx->in_tok = 0;
	ctx->cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!ctx->cmd)
		return (1);
	ft_bzero(ctx->cmd, sizeof(t_cmd));
	return (0);
}

static int	process_char(const char *in, size_t *i, t_normi_ctx*ctx)
{
	if (is_space(in[*i]))
		return (handle_space(in, i, ctx));
	else if (in[*i] == '\'' || in[*i] == '\"')
		return (handle_quote(in, i, ctx));
	else if (is_operator_char(in[*i]))
		return (handle_operator(in, i, ctx));
	else
		return (handle_regular(in, i, ctx));
}

static int	tokenize_loop(const char *in, t_normi_ctx*ctx)
{
	size_t	i;

	i = 0;
	while (in && in[i])
	{
		if (process_char(in, &i, ctx))
		{
			cleanup_tok_data(&ctx->bf, &ctx->tp, &ctx->cmd);
			return (1);
		}
	}
	return (0);
}

int	tokenizer(t_mini **mini)
{
	char			*in;
	t_normi_ctx		ctx;

	if (init_ctx(&ctx, *mini))
		return (1);
	in = (*mini)->input;
	if (tokenize_loop(in, &ctx))
		return (1);
	if (ctx.in_tok)
	{
		if (finalize_tok(&ctx.bf, &ctx.tp, ctx.cmd))
		{
			cleanup_tok_data(&ctx.bf, &ctx.tp, &ctx.cmd);
			return (1);
		}
	}
	(*mini)->cmds = ctx.cmd;
	return (0);
}
