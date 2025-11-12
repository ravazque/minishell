/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 18:35:48 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/11 14:54:40 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	quotes_balanced(const char *s)
{
	int		sq;
	int		dq;
	size_t	i;

	if (!s)
		return (1);
	sq = 0;
	dq = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] == '\'' && dq == 0)
			sq = !sq;
		else if (s[i] == '\"' && sq == 0)
			dq = !dq;
		i++;
	}
	if (sq == 0 && dq == 0)
		return (1);
	return (0);
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
	if (start_tokenizer(mini) == 1)
		return ;
	if (start_lexer(mini) == 1)
		return ;
	if (start_assignments(mini) == 1)
		return ;
	if (start_expander(mini) == 1)
		return ;
}
