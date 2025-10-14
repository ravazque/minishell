/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 20:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/14 20:42:59 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	executor(t_mini *mini)
{
	int	cmd_count;

	if (!mini || !mini->cmds)
		return ;
	cmd_count = ft_lstsize(mini->cmds);
	// TODO: Implement execution logic
	(void)cmd_count;
}
