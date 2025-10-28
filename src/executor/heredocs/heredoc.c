/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 19:05:09 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/23 18:29:34 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	write_lines_to_pipe(int write_fd, char **lines)
{
	int		i;
	size_t	len;

	if (!lines)
		return (0);
	i = 0;
	while (lines[i])
	{
		len = ft_strlen(lines[i]);
		if (write(write_fd, lines[i], len) == -1)
			return (1);
		if (write(write_fd, "\n", 1) == -1)
			return (1);
		i++;
	}
	return (0);
}

static t_heredoc_data	*create_heredoc_data(char **lines)
{
	t_heredoc_data	*data;

	data = malloc(sizeof(t_heredoc_data));
	if (!data)
		return (malloc_error(), NULL);
	if (pipe(data->pipe_fd) == -1)
	{
		free(data);
		return (NULL);
	}
	if (write_lines_to_pipe(data->pipe_fd[1], lines))
	{
		close(data->pipe_fd[0]);
		close(data->pipe_fd[1]);
		free(data);
		return (NULL);
	}
	close(data->pipe_fd[1]);
	data->lines = lines;
	return (data);
}

int	process_heredoc(t_redir *redir, t_mini *mini)
{
	char			**heredoc_lines;
	t_heredoc_data	*data;

	heredoc_lines = NULL;
	if (collect_heredoc_lines(redir->target, mini,
			redir->hd_expand, &heredoc_lines))
		return (1);
	data = create_heredoc_data(heredoc_lines);
	if (!data)
	{
		if (heredoc_lines)
			free_dblptr(heredoc_lines);
		return (1);
	}
	redir->hd_data = data;
	redir->fd = data->pipe_fd[0];
	return (0);
}

static int	process_cmd_heredocs(t_cmd *cmd, t_mini *mini)
{
	t_redir	*redir;

	if (!cmd || !cmd->redirs)
		return (0);
	redir = cmd->redirs;
	while (redir)
	{
		if (redir->in_redir == 2)
		{
			if (process_heredoc(redir, mini))
				return (1);
		}
		redir = redir->next;
	}
	return (0);
}

int	heredocs(t_mini *mini)
{
	t_cmd	*cmd;

	if (!mini || !mini->cmds)
		return (0);
	cmd = mini->cmds;
	while (cmd)
	{
		if (process_cmd_heredocs(cmd, mini))
			return (1);
		cmd = cmd->next;
	}
	return (0);
}
