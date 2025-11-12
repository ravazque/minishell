/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_pipe_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 00:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/11 00:00:00 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	add_cmd_to_lst(t_cmd **lst, t_cmd *new)
{
	t_cmd	*curr;

	if (!*lst)
		*lst = new;
	else
	{
		curr = *lst;
		while (curr->next)
			curr = curr->next;
		curr->next = new;
	}
}

int	count_tokens_until_pipe(t_token *start)
{
	int		count;
	t_token	*curr;

	count = 0;
	curr = start;
	while (curr && !is_pipe(curr->raw, curr))
	{
		count++;
		curr = curr->next;
	}
	return (count);
}

t_token	*skip_to_next_pipe(t_token *start)
{
	t_token	*curr;

	curr = start;
	while (curr && !is_pipe(curr->raw, curr))
		curr = curr->next;
	if (curr && is_pipe(curr->raw, curr))
		return (curr->next);
	return (NULL);
}
