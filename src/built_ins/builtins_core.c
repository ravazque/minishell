/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_core.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 17:10:07 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/20 13:05:24 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	is_builtin_cmd(const char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "exit") == 0 || ft_strcmp(cmd, "bye") == 0)
		return (1);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	return (0);
}

bool	built_ins(t_mini *mini)
{
	char	*cmd;

	if (!mini || !mini->cmds || !mini->cmds->tokens || !mini->cmds->tokens[0])
		return (false);
	cmd = mini->cmds->tokens[0];
	if (!is_builtin_cmd(cmd))
		return (false);
	if (ft_strcmp(cmd, "exit") == 0 || ft_strcmp(cmd, "bye") == 0)
		builtin_exit(mini);
	else if (ft_strcmp(cmd, "echo") == 0)
		builtin_echo(mini);
	else if (ft_strcmp(cmd, "cd") == 0)
		builtin_cd(mini);
	else if (ft_strcmp(cmd, "pwd") == 0)
		builtin_pwd(mini);
	else if (ft_strcmp(cmd, "export") == 0)
		builtin_export(mini);
	else if (ft_strcmp(cmd, "unset") == 0)
		builtin_unset(mini);
	else if (ft_strcmp(cmd, "env") == 0)
		builtin_env(mini);
	return (true);
}
