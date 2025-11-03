/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_split.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 00:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/03 18:25:45 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"

static char	**alloc_result(int total_count)
{
	char	**new;

	if (total_count == 0)
	{
		new = (char **)malloc(sizeof(char *) * 1);
		if (!new)
			return (NULL);
		new[0] = NULL;
		return (new);
	}
	new = (char **)malloc(sizeof(char *) * (total_count + 1));
	if (!new)
		return (NULL);
	return (new);
}

static int	process_tokens(char **new, t_token *curr, t_mini *mini)
{
	int	j;

	j = 0;
	while (curr)
	{
		if (add_expanded_tokens(new, &j, curr, mini))
		{
			free_split_result(new, j);
			return (1);
		}
		curr = curr->next;
	}
	new[j] = NULL;
	return (0);
}

int	exp_cmd_toks_with_split(t_cmd *cmd, t_mini *mini)
{
	char	**new;
	int		total_count;

	if (!cmd->tokn)
		return (0);
	total_count = count_expanded_tokens(cmd->tokn, mini);
	if (total_count < 0)
		return (1);
	new = alloc_result(total_count);
	if (!new)
		return (1);
	if (total_count == 0)
	{
		if (cmd->tokens)
			free_dblptr(cmd->tokens);
		cmd->tokens = new;
		return (0);
	}
	if (process_tokens(new, cmd->tokn, mini))
		return (1);
	if (cmd->tokens)
		free_dblptr(cmd->tokens);
	cmd->tokens = new;
	return (0);
}
