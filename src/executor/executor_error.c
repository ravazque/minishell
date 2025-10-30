/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_error.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 18:01:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/30 18:01:00 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	print_exec_error(char *cmd, int error_type, int is_path)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	if (error_type == 126)
	{
		if (is_directory(cmd))
			ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
		else
			ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
	}
	else if (error_type == 127)
	{
		if (is_path)
			ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		else
			ft_putstr_fd(": command not found\n", STDERR_FILENO);
	}
	else
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
}

static void	handle_path_error(char *cmd)
{
	if (access(cmd, F_OK) == 0)
	{
		print_exec_error(cmd, 126, 1);
		exit(126);
	}
	else
	{
		print_exec_error(cmd, 127, 1);
		exit(127);
	}
}

void	handle_no_path(char **argv)
{
	if (ft_strchr(argv[0], '/'))
		handle_path_error(argv[0]);
	else
	{
		print_exec_error(argv[0], 127, 0);
		exit(127);
	}
}
