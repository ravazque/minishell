/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_core.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 17:10:07 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/07 23:30:47 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	built_ins(t_mini *mini)
{
	if (!mini || !mini->cmds || !mini->cmds->tokens || !mini->cmds->tokens[0])
		return (false);
	if (ft_strcmp(mini->cmds->tokens[0], "exit") == 0)
		return (builtin_exit(mini), true);
	else if (ft_strcmp(mini->cmds->tokens[0], "echo") == 0)
		return (builtin_echo(mini), true);
	else if (ft_strcmp(mini->cmds->tokens[0], "cd") == 0)
		return (builtin_cd(mini), true);
	else if (ft_strcmp(mini->cmds->tokens[0], "pwd") == 0)
		return (builtin_pwd(mini), true);
	else if (ft_strcmp(mini->cmds->tokens[0], "export") == 0)
		return (builtin_export(mini), mini->exit_sts = 0, true);
	else if (ft_strcmp(mini->cmds->tokens[0], "unset") == 0)
		return (builtin_unset(mini), mini->exit_sts = 0, true);
	else if (ft_strcmp(mini->cmds->tokens[0], "env") == 0)
		return (builtin_env(mini), true);
	else
		return (false);
}
