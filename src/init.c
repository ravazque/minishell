/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 19:28:04 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/17 00:35:29 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	init_mini(t_mini *mini)
{
	mini->cmds = NULL;
	mini->prompt = NULL;
	mini->input = NULL;
	mini->i = 0;
	mini->exit_sts = 0;
}
