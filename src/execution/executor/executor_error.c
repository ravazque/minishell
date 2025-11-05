/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_error.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 18:01:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/04 13:46:59 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

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
		if (is_directory(cmd))
		{
			print_exec_error(cmd, 126, 1);
			exit(126);
		}
		print_exec_error(cmd, 126, 1);
		exit(126);
	}
	else
	{
		print_exec_error(cmd, 127, 1);
		exit(127);
	}
}

static int	is_tilde_expansion(char *cmd, char *cd_home)
{
	size_t	home_len;

	if (!cd_home || !cmd)
		return (0);
	home_len = ft_strlen(cd_home);
	if (ft_strncmp(cmd, cd_home, home_len) == 0)
	{
		if (cmd[home_len] == '\0' || cmd[home_len] == '/')
			return (1);
	}
	return (0);
}

void	handle_no_path(char **argv, char *cd_home)
{
	if (is_tilde_expansion(argv[0], cd_home))
		handle_path_error(argv[0]);
	else if (ft_strchr(argv[0], '/'))
		handle_path_error(argv[0]);
	else
	{
		print_exec_error(argv[0], 127, 0);
		exit(127);
	}
}
