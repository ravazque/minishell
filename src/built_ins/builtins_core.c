/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_core.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ptrapero <ptrapero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 17:10:07 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/22 21:36:41 by ptrapero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_builtin_cmd(const char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	if (ft_strcmp(cmd, "echo") == 0)
		return (2);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (2);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (2);
	return (0);
}

bool	built_ins(t_mini *mini, t_cmd *command)
{
	char	*cmd;

	if (!mini || !command || !command->tokens || !command->tokens[0])
		return (false);
	cmd = command->tokens[0];
	if (!is_builtin_cmd(cmd))
		return (false);
	if (ft_strcmp(cmd, "exit") == 0)
		builtin_exit(mini);
	else if (ft_strcmp(cmd, "echo") == 0)
		builtin_echo(command);
	else if (ft_strcmp(cmd, "cd") == 0)
		builtin_cd(mini);
	else if (ft_strcmp(cmd, "pwd") == 0)
		builtin_pwd(mini);
	else if (ft_strcmp(cmd, "export") == 0)
		builtin_export(mini);
	else if (ft_strcmp(cmd, "unset") == 0)
		builtin_unset(mini);
	else if (ft_strcmp(cmd, "env") == 0)
		builtin_env(mini, command);
	return (true);
}
