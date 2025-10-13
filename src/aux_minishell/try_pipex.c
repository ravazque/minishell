/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 19:15:41 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/13 19:20:42 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	try_pipex(t_mini *mini)
{
	if (!mini || !mini->cmds || !mini->cmds->tokens)
	{
		ft_putstr_fd("minishell: : command not found\n", STDERR_FILENO);
		return ;
	}
}
