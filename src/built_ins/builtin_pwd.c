/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 20:07:14 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/18 20:20:43 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	builtin_pwd(t_mini *mini)
{
	mini->exit_sts = 0;
	ft_putstr_fd(mini->pwd, STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
}
