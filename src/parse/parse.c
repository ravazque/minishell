/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 18:35:48 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/03 17:13:11 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	parse(t_mini *mini)
{
	if (!mini || !mini->input || is_empty_or_whitespace(mini->input))
		return ;
	if (!quotes_balanced(mini->input))
	{
		ft_putstr_fd(ERR_QUO, STDERR_FILENO);
		mini->exit_sts = 2;
		return ;
	}
	if (start_tokenizer(mini) == 1)
		return ;
	if (start_lexer(mini) == 1)
		return ;
	if (start_assignments(mini) == 1)
		return ;
	if (start_expander(mini) == 1)
		return ;
}
