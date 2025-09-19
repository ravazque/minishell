/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 04:26:47 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/19 06:23:48 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
		if (cmd->redirs)
			free_redirs(cmd->redirs);
		free(cmd);
		cmd = n;
	}
}
