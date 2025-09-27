/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 18:35:48 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/27 13:17:29 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	is_empty_or_whitespace(const char *str)
{
	int	i;

	if (!str)
		return (1);
	i = 0;
	while (str[i])
	{
		if (!is_space(str[i]))
			return (0);
		i++;
	}
	return (1);
}

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
	if (tokenizer(&mini) == 1)
	{
		ft_putstr_fd(ERR_QUO, STDERR_FILENO);
		mini->exit_sts = 2;
		return ;
	}
	if (lexer(mini) == 1)
	{
		ft_putstr_fd(ERR_QUO, STDERR_FILENO);
		mini->exit_sts = 2;
		return ;
	}
	if (expander(mini) == 1)
	{
		ft_putstr_fd(ERR_VAR, STDERR_FILENO);
		mini->exit_sts = 1;
		return ;
	}
}
