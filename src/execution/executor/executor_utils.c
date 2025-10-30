/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 16:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/30 18:37:06 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	has_redirs(t_cmd *cmd)
{
	if (!cmd)
		return (0);
	return (cmd->redirs != NULL);
}

int	count_args(char **tokens)
{
	int	i;

	i = 0;
	if (!tokens)
		return (0);
	while (tokens[i])
		i++;
	return (i);
}

int	is_empty_cmd(t_cmd *cmd)
{
	if (!cmd)
		return (1);
	if (!cmd->tokens || !cmd->tokens[0])
		return (1);
	if (cmd->tokens[0][0] == '\0')
		return (1);
	return (0);
}
