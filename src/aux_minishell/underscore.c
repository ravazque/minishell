/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   underscore.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 13:29:45 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/20 13:35:07 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*get_last_arg(char **tokens)
{
	int	i;

	if (!tokens)
		return (NULL);
	if (!tokens[0])
		return (NULL);
	i = 0;
	while (tokens[i])
		i++;
	i--;
	if (i < 0)
		return (NULL);
	return (tokens[i]);
}

static void	update_underscore(t_mini *mini, t_cmd *cmd)
{
	char	*last_arg;

	if (!mini)
		return;
	if (!cmd)
		return;
	if (!cmd->tokens)
		return;
	if (!cmd->tokens[0])
		return;
	last_arg = get_last_arg(cmd->tokens);
	if (last_arg)
		ft_setenv("_", last_arg, &(mini->env));
}

static t_cmd	*get_last_cmd(t_cmd *cmds)
{
	t_cmd	*current;

	if (!cmds)
		return (NULL);
	current = cmds;
	while (current->next)
		current = current->next;
	return (current);
}

void	update_underscore_succ(t_mini *mini)
{
	t_cmd	*last_cmd;

	if (!mini)
		return;
	if (!mini->cmds)
		return;
	if (mini->exit_sts != 0)
		return;
	last_cmd = get_last_cmd(mini->cmds);
	if (last_cmd)
		update_underscore(mini, last_cmd);
}
