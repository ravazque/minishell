/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 00:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/12 18:30:42 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	is_operator_char(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (1);
	return (0);
}

void	cleanup_tok_data(char **bf, t_token_part **p, t_cmd **cmd)
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

int	add_tok_to_cmd(t_token *new, t_cmd *cmd)
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

int	handle_regular(const char *in, size_t *i, t_normi_ctx *ctx)
{
	ctx->in_tok = 1;
	if (append_char(&ctx->bf, in[*i]))
		return (1);
	(*i)++;
	return (0);
}
