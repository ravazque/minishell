/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 19:26:15 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/15 19:41:38 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_cmds(t_cmd *cmds)
{
	int	i;

	if (!cmds)
		return ;
	if (cmds->args)
	{
		i = 0;
		while (cmds->args[i])
		{
			free(cmds->args[i]);
			i++;
		}
		free(cmds->args);
	}
	free(cmds);
}

void	free_split(char **split)
{
	int	i;

	i = 0;
	if (!split)
		return ;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

void	cleanup_mini(t_mini *mini)
{
	if (mini->input)
	{
		free(mini->input);
		mini->input = NULL;
	}
	if (mini->cmds)
	{
		free_cmds(mini->cmds);
		mini->cmds = NULL;
	}
}
