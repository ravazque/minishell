/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 04:26:47 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/26 12:21:49 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	free_token_parts(t_token_part *parts)
{
	t_token_part	*current;
	t_token_part	*next;

	current = parts;
	while (current)
	{
		next = current->next;
		if (current->content)
			free(current->content);
		free(current);
		current = next;
	}
}

static void	free_token_list(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	current = tokens;
	while (current)
	{
		next = current->next;
		if (current->raw)
			free(current->raw);
		if (current->parts)
			free_token_parts(current->parts);
		free(current);
		current = next;
	}
}

void	free_cmds(t_cmd *cmd)
{
	t_cmd	*n;
	size_t	i;

	while (cmd)
	{
		n = cmd->next;
		if (cmd->tokens)
		{
			i = 0;
			while (cmd->tokens[i])
			{
				free(cmd->tokens[i]);
				i++;
			}
			free(cmd->tokens);
		}
		if (cmd->tokn)
			free_token_list(cmd->tokn);
		if (cmd->redirs)
			free_redirs(cmd->redirs);
		free(cmd);
		cmd = n;
	}
}
