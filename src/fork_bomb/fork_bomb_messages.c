/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_bomb_messages.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 16:57:44 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/30 17:00:37 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	print_step_message(int step)
{
	if (step == 0)
	{
		ft_putstr_fd(RL_RED, STDOUT_FILENO);
		ft_putstr_fd(MSG_APOLOGY, STDOUT_FILENO);
		ft_putstr_fd(RL_RST, STDOUT_FILENO);
	}
	else
	{
		ft_putstr_fd(RL_YEL, STDOUT_FILENO);
		ft_putstr_fd(MSG_CONFIRM, STDOUT_FILENO);
		ft_putstr_fd(RL_RST, STDOUT_FILENO);
	}
}

void	print_wrong_answer(void)
{
	ft_putstr_fd(RL_BLD, STDOUT_FILENO);
	ft_putstr_fd(RL_RED, STDOUT_FILENO);
	ft_putstr_fd(MSG_WRONG, STDOUT_FILENO);
	ft_putstr_fd(RL_RST, STDOUT_FILENO);
}

void	correct_message(void)
{
	ft_putstr_fd(RL_BLD, STDOUT_FILENO);
	ft_putstr_fd(RL_GRN, STDOUT_FILENO);
	ft_putstr_fd(MSG_FORGIVEN, STDOUT_FILENO);
	ft_putstr_fd(RL_RST, STDOUT_FILENO);
}
