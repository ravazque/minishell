/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 19:28:30 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/30 17:14:27 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	loop(t_mini *mini)
{
	while (1)
	{
		mini->prompt = prompt(mini);
		if (!mini->prompt)
			mini->prompt = ft_strdup("~ $ ");
		mini->input = readline(mini->prompt);
		ft_signal(mini);
		if (mini->input && *mini->input)
			add_history(mini->input);
		parse(mini);
		if (built_ins(mini) == false)
		{
			if (mini->cmds)
				print_tokens(mini);
		}
		free_args(mini);
		if (mini->cmds)
		{
			free_cmds(mini->cmds);
			mini->cmds = NULL;
		}
	}
}
