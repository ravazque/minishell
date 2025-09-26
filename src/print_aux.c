/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_aux.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 13:45:02 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/26 12:47:37 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	print_tokens(t_mini *mini)
{
	int	i;

	/* Protección contra segfaults */
	if (!mini || !mini->cmds || !mini->cmds->tokens)
	{
		ft_putstr_fd("Error: command not found\n", STDERR_FILENO);
		return ;
	}

	i = 0;
	while (mini->cmds->tokens[i] != NULL)
	{
		printf("args[%d] = ( %s )\n", i, mini->cmds->tokens[i]);
		i++;
	}
}
