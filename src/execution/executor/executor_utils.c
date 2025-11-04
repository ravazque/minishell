/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 16:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/03 21:01:53 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	has_redirs(t_cmd *cmd)
{
	if (!cmd)
		return (0);
	return (cmd->redirs != NULL);
}

int	count_args(char **tokens)
{
	int	i;

	i = 0;
	if (!tokens)
		return (0);
	while (tokens[i])
		i++;
	return (i);
}

int	is_empty_cmd(t_cmd *cmd)
{
	if (!cmd)
		return (1);
	if (!cmd->tokens || !cmd->tokens[0])
		return (1);
	if (cmd->tokens[0][0] == '\0')
		return (1);
	return (0);
}

static int	handle_only_redirections(t_mini *mini)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdin == -1 || saved_stdout == -1)
		return (1);
	if (redirections(mini->cmds))
	{
		mini->exit_sts = 1;
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		return (1);
	}
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	mini->exit_sts = 0;
	return (0);
}

int	check_empty_command(t_mini *mini)
{
	if (!mini->cmds->tokens || !mini->cmds->tokens[0])
	{
		if (mini->cmds->redirs)
			return (handle_only_redirections(mini));
		mini->exit_sts = 0;
		return (1);
	}
	if (mini->cmds->tokens[0][0] == '\0')
	{
		ft_putstr_fd("minishell: : command not found\n", STDERR_FILENO);
		mini->exit_sts = 127;
		return (1);
	}
	return (0);
}
