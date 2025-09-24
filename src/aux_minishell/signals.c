/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 01:53:41 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/24 16:41:07 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_signal(t_mini *mini)
{
	if (!mini->input)
	{
		cleanup_mini(mini);
		write(1, "exit\n", 5);
		exit(0);
	}
}
