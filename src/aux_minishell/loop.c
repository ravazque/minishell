/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 19:28:30 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/07 21:55:53 by ravazque         ###   ########.fr       */
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
	if (mini->cmds)
	{
		if (!built_ins(mini))
		{
			print_tokens(mini);
		}
	}
}

void	loop(t_mini *mini)
{
	while (1)
	{
		mini->prompt = prompt(mini);
		if (!mini->prompt)
			mini->prompt = ft_strdup("~ $ ");
		mini->input = readline(mini->prompt); // al reescalar la pantalla se rompe el comportamiento de readline (jvidal-t)
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
		cleanup_iteration(mini);
	}
}
