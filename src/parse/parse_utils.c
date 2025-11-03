/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 17:10:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/03 17:13:50 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_empty_or_whitespace(const char *str)
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

void	cleanup_parse_error(t_mini *mini)
{
	if (mini->cmds)
	{
		free_cmds(mini->cmds);
		mini->cmds = NULL;
	}
}

int	start_tokenizer(t_mini *mini)
{
	if (tokenizer(&mini) == 1)
	{
		ft_putstr_fd(ERR_QUO, STDERR_FILENO);
		mini->exit_sts = 2;
		cleanup_parse_error(mini);
		return (1);
	}
	return (0);
}

int	start_lexer(t_mini *mini)
{
	if (lexer(mini) == 1)
	{
		mini->exit_sts = 2;
		cleanup_parse_error(mini);
		return (1);
	}
	return (0);
}

int	start_expander(t_mini *mini)
{
	if (expander(mini) == 1)
	{
		mini->exit_sts = 1;
		cleanup_parse_error(mini);
		return (1);
	}
	return (0);
}
