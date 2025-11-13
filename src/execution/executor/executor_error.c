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

static char	*get_error_msg(int error_type, int is_path, char *cmd)
{
	if (error_type == 126)
	{
		if (is_directory(cmd))
			return (": Is a directory\n");
		else
			return (": Permission denied\n");
	}
	else if (error_type == 127)
	{
		if (is_path)
			return (": No such file or directory\n");
		else
			return (": command not found\n");
	}
	else
		return (": No such file or directory\n");
}

void	print_exec_error(char *cmd, int error_type, int is_path)
{
	char	*prefix;
	char	*error_msg;
	char	*temp;
	char	*full_msg;

	prefix = "minishell: ";
	error_msg = get_error_msg(error_type, is_path, cmd);
	temp = ft_strjoin(prefix, cmd);
	if (!temp)
		return ;
	full_msg = ft_strjoin(temp, error_msg);
	free(temp);
	if (!full_msg)
		return ;
	write(STDERR_FILENO, full_msg, ft_strlen(full_msg));
	free(full_msg);
}

static void	handle_path_error(char *cmd)
{
	if (access(cmd, F_OK) == 0)
	{
		if (is_directory(cmd))
		{
			print_exec_error(cmd, 126, 1);
			cleanup_child_fds();
			exit(126);
		}
		print_exec_error(cmd, 126, 1);
		cleanup_child_fds();
		exit(126);
	}
	else
	{
		print_exec_error(cmd, 127, 1);
		cleanup_child_fds();
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
		cleanup_child_fds();
		exit(127);
	}
}
