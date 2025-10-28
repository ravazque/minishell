/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   underscore.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 13:29:45 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/28 16:07:11 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ensure_path(t_mini *mini)
{
	char	*path;

	if (!mini || !mini->env)
		return ;
	path = get_local_env("PATH", mini->env);
	if (!path)
		ft_setenv("PATH", ".", &(mini->env));
}

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

void	update_underscore(t_mini *mini)
{
	t_cmd	*last_cmd;
	char	*last_arg;

	if (!mini || !mini->cmds)
		return ;
	last_cmd = get_last_cmd(mini->cmds);
	if (!last_cmd || !last_cmd->tokens || !last_cmd->tokens[0])
		return ;
	if (ft_strcmp(last_cmd->tokens[0], "env") == 0)
		return ;
	last_arg = get_last_arg(last_cmd->tokens);
	if (last_arg)
		ft_setenv("_", last_arg, &(mini->env));
}
