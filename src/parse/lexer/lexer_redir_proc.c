/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_redir_proc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 00:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/11 00:00:00 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	rm_redir_toks(t_token **head, t_token *curr, t_token *prev)
{
	t_token	*next;

	if (!curr || !curr->next)
		return (1);
	next = curr->next;
	if (prev)
		prev->next = next->next;
	else
		*head = next->next;
	if (curr->raw)
		free(curr->raw);
	if (curr->parts)
		free_token_parts(curr->parts);
	free(curr);
	if (next->raw)
		free(next->raw);
	if (next->parts)
		free_token_parts(next->parts);
	free(next);
	return (0);
}

static int	print_redir_error(t_token *next)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(next->raw, 2);
	ft_putstr_fd("'\n", 2);
	return (1);
}

static int	process_single_redir(t_cmd *cmd, t_token *curr, t_token *next)
{
	t_redir	*r;

	if (!next)
		return (ft_putstr_fd(ERR_RDI, 2), 1);
	if (is_redir(next->raw, next))
		return (print_redir_error(next));
	if (!validate_redir_target(next))
		return (ft_putstr_fd(ERR_RDI, 2), 1);
	r = mk_redir(next->raw, curr->raw, next);
	if (!r)
		return (1);
	add_redir(cmd, r);
	return (0);
}

static void	update_loop_vars(t_token **curr, t_token *prev, t_cmd *cmd)
{
	*curr = prev;
	if (*curr)
		*curr = (*curr)->next;
	else
		*curr = cmd->tokn;
}

int	proc_redirs(t_cmd *cmd)
{
	t_token	*curr;
	t_token	*next;
	t_token	*prev;

	if (!cmd || !cmd->tokn)
		return (0);
	curr = cmd->tokn;
	prev = NULL;
	while (curr)
	{
		next = curr->next;
		if (is_redir(curr->raw, curr))
		{
			if (process_single_redir(cmd, curr, next))
				return (1);
			rm_redir_toks(&cmd->tokn, curr, prev);
			update_loop_vars(&curr, prev, cmd);
			continue ;
		}
		prev = curr;
		curr = next;
	}
	return (0);
}
