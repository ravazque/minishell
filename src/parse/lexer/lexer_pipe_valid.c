/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_pipe_valid.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 00:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/11 00:00:00 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	check_first_pipe(int first_token)
{
	if (first_token)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd("syntax error near unexpected token `|'\n", 2);
		return (1);
	}
	return (0);
}

static int	check_double_pipe(t_token *curr)
{
	if (curr->next && is_pipe(curr->next->raw, curr->next))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd("syntax error near unexpected token `|'\n", 2);
		return (1);
	}
	return (0);
}

static int	check_end_pipe(t_token *curr)
{
	if (!curr->next)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd("syntax error: unexpected end after `|'\n", 2);
		return (1);
	}
	return (0);
}

int	validate_pipe_syntax(t_token *tokens)
{
	t_token	*curr;
	int		first_token;

	if (!tokens)
		return (0);
	curr = tokens;
	first_token = 1;
	while (curr)
	{
		if (is_pipe(curr->raw, curr))
		{
			if (check_first_pipe(first_token))
				return (1);
			if (check_double_pipe(curr))
				return (1);
			if (check_end_pipe(curr))
				return (1);
		}
		first_token = 0;
		curr = curr->next;
	}
	return (0);
}
