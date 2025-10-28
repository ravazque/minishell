/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 18:35:48 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/28 16:11:09 by ravazque         ###   ########.fr       */
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

int	process_assignments(t_mini *mini)
{
	t_cmd	*cmd;
	t_token	*tok;
	char	**split;

	if (!mini || !mini->cmds)
		return (0);
	cmd = mini->cmds;
	while (cmd)
	{
		tok = cmd->tokn;
		while (tok && tok->is_assignment)
		{
			split = ft_split(tok->raw, '=');
			if (!split || !split[0])
			{
				if (split)
					free_dblptr(split);
				return (1);
			}
			if (split[1])
				set_local_var(split[0], split[1], mini);
			else
				set_local_var(split[0], "", mini);
			free_dblptr(split);
			tok = tok->next;
		}
		cmd = cmd->next;
	}
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
	if (tokenizer(&mini) == 1)
	{
		ft_putstr_fd(ERR_QUO, STDERR_FILENO);
		mini->exit_sts = 2;
		if (mini->cmds)
		{
			free_cmds(mini->cmds);
			mini->cmds = NULL;
		}
		return ;
	}
	if (lexer(mini) == 1)
	{
		mini->exit_sts = 2;
		if (mini->cmds)
		{
			free_cmds(mini->cmds);
			mini->cmds = NULL;
		}
		return ;
	}
	if (process_assignments(mini) == 1)
	{
		mini->exit_sts = 1;
		if (mini->cmds)
		{
			free_cmds(mini->cmds);
			mini->cmds = NULL;
		}
		return ;
	}
	if (expander(mini) == 1)
	{
		mini->exit_sts = 1;
		if (mini->cmds)
		{
			free_cmds(mini->cmds);
			mini->cmds = NULL;
		}
		return ;
	}
}
