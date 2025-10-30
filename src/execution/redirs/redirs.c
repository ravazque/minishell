/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ptrapero <ptrapero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 12:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/28 20:54:39 by ptrapero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	handle_input_redir(t_redir *redir)
{
	int	fd;

	fd = open(redir->target, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(redir->target, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		return (1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

static int	handle_heredoc_redir(t_redir *redir)
{
	if (!redir->hd_data || redir->fd == -1)
		return (1);
	if (dup2(redir->fd, STDIN_FILENO) == -1)
		return (1);
	return (0);
}

static int	handle_output_redir_trunc(t_redir *redir)
{
	int	fd;

	fd = open(redir->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(redir->target, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		return (1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

static int	handle_output_redir_add(t_redir *redir)
{
	int	fd;

	fd = open(redir->target, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(redir->target, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		return (1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int	redirections(t_cmd *cmd)
{
	t_redir	*redir;

	if (!cmd || !cmd->redirs)
		return (0);
	redir = cmd->redirs;
	while (redir)
	{
		if (redir->in_redir == 1 && handle_input_redir(redir))
			return (1);
		else if (redir->in_redir == 2 && handle_heredoc_redir(redir))
			return (1);
		else if (redir->out_redir == 1 && handle_output_redir_trunc(redir))
			return (1);
		else if (redir->out_redir == 2 && handle_output_redir_add(redir))
			return (1);
		redir = redir->next;
	}
	return (0);
}
