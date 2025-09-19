/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_mini.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 19:20:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/19 04:30:45 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_dblptr(char **dblptr)
{
	size_t	i;

	if (!dblptr)
		return ;
	i = 0;
	while (dblptr[i])
	{
		free(dblptr[i]);
		i++;
	}
	free(dblptr);
}

static void	free_args(t_mini *mini)
{
	if (mini->env)
	{
		free_dblptr(mini->env);
		mini->env = NULL;
	}
	if (mini->argv)
	{
		free_dblptr(mini->argv);
		mini->argv = NULL;
	}
}

void	cleanup_mini(t_mini *mini)
{
	if (mini->input)
	{
		free(mini->input);
		mini->input = NULL;
	}
	if (mini->prompt)
	{
		free(mini->prompt);
		mini->prompt = NULL;
	}
	if (mini->pwd)
	{
		free(mini->pwd);
		mini->pwd = NULL;
	}
	free_args(mini);
	if (mini->cmds)
	{
		free_cmds(mini->cmds);
		mini->cmds = NULL;
	}
}
