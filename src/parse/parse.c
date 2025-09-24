/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 18:35:48 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/24 01:36:25 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	parse(t_mini *mini)
{
	if (!mini || !mini->input)
		return ;
	if (!quotes_balanced(mini->input))
	{
		ft_putstr_fd(ERR_QUO, STDERR_FILENO);
		mini->exit_sts = 2;
		return ;
	}
	if (tokenizer(&mini) == 1)
	{
		ft_putstr_fd(ERR_QUO, STDERR_FILENO);
		mini->exit_sts = 2;
		return ;
	}
	// lexer con redirecciones [ abrir cada archivo con redirrecion y seleccionar los últimos ]
}
