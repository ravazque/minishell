/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_assign.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 17:10:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/03 17:13:59 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	process_single_assignment(t_token *tok, t_mini *mini)
{
	char	**split;
	char	*value;

	split = ft_split(tok->raw, '=');
	if (!split || !split[0])
	{
		if (split)
			free_dblptr(split);
		return (1);
	}
	value = "";
	if (split[1])
		value = split[1];
	set_local_var(split[0], value, mini);
	free_dblptr(split);
	return (0);
}

static int	process_cmd_assignments(t_cmd *cmd, t_mini *mini)
{
	t_token	*tok;

	tok = cmd->tokn;
	while (tok && tok->is_assignment)
	{
		if (process_single_assignment(tok, mini) == 1)
			return (1);
		tok = tok->next;
	}
	return (0);
}

int	process_assignments(t_mini *mini)
{
	t_cmd	*cmd;

	if (!mini || !mini->cmds)
		return (0);
	cmd = mini->cmds;
	while (cmd)
	{
		if (process_cmd_assignments(cmd, mini) == 1)
			return (1);
		cmd = cmd->next;
	}
	return (0);
}

int	start_assignments(t_mini *mini)
{
	if (process_assignments(mini) == 1)
	{
		mini->exit_sts = 1;
		cleanup_parse_error(mini);
		return (1);
	}
	return (0);
}
