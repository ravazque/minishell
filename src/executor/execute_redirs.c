/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirs.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 12:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/23 17:34:06 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	handle_input_redir(t_redir *redir)
{
	(void)redir;
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

static int	handle_output_redir(t_redir *redir, int append)
{
	(void)redir;
	(void)append;
	return (0);
}

int	setup_redirections(t_cmd *cmd)
{
	t_redir	*redir;

	if (!cmd || !cmd->redirs)
		return (0);
	redir = cmd->redirs;
	while (redir)
	{
		if (redir->in_redir == 1)
		{
			if (handle_input_redir(redir))
				return (1);
		}
		else if (redir->in_redir == 2)
		{
			if (handle_heredoc_redir(redir))
				return (1);
		}
		else if (redir->out_redir == 1)
		{
			if (handle_output_redir(redir, 0))
				return (1);
		}
		else if (redir->out_redir == 2)
		{
			if (handle_output_redir(redir, 1))
				return (1);
		}
		redir = redir->next;
	}
	return (0);
}
