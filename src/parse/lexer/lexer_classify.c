/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_classify.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 00:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/12 15:07:06 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	is_pipe(const char *str, t_token *tok)
{
	if (!str)
		return (0);
	if (tok_has_any_quotes(tok))
		return (0);
	if (ft_strcmp(str, "|") == 0)
		return (1);
	return (0);
}

static int	check_redir_type(const char *str)
{
	if (ft_strcmp(str, "<") == 0)
		return (1);
	if (ft_strcmp(str, ">") == 0)
		return (1);
	if (ft_strcmp(str, ">>") == 0)
		return (1);
	if (ft_strcmp(str, "<<") == 0)
		return (1);
	return (0);
}

int	is_redir(const char *str, t_token *tok)
{
	if (!str)
		return (0);
	if (tok_has_any_quotes(tok))
		return (0);
	return (check_redir_type(str));
}

static int	is_valid_filename(const char *name)
{
	if (!name)
		return (0);
	if (!name[0])
		return (0);
	if (name[0] == '\0')
		return (0);
	return (1);
}

int	validate_redir_target(t_token *tgt_tok)
{
	if (!tgt_tok)
		return (0);
	if (!tgt_tok->raw)
		return (0);
	if (tgt_tok->raw[0] == '\0')
		return (0);
	if (!is_valid_filename(tgt_tok->raw))
		return (0);
	return (1);
}
