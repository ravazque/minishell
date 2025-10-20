/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 19:28:30 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/20 13:06:17 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	cleanup_iteration(t_mini *mini)
{
	free_args(mini);
	if (mini->cmds)
	{
		free_cmds(mini->cmds);
		mini->cmds = NULL;
	}
}

static void	process_command(t_mini *mini)
{
	parse(mini);
	if (mini->cmds && built_ins(mini) == false) // quitar comprobacion de buitins y pasarlo al executor
		executor(mini);
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

static void	update_underscore_if_success(t_mini *mini)
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

void	loop(t_mini *mini)
{
	while (1)
	{
		mini->prompt = prompt(mini);
		if (!mini->prompt)
			mini->prompt = ft_strdup("~ $ ");
		mini->input = readline(mini->prompt);
		ft_signal(mini);
		if (!mini->input)
		{
			cleanup_mini(mini);
			write(STDOUT_FILENO, "exit\n", 5);
			exit(mini->exit_sts);
		}
		if (*mini->input)
			add_history(mini->input);
		if (mini->input && is_fork_bomb(mini->input))
		{
			handle_fork_bomb(mini);
			cleanup_iteration(mini);
			continue ;
		}
		process_command(mini);
		update_underscore_if_success(mini);
		cleanup_iteration(mini);
	}
}
