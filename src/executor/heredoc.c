/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 19:05:09 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/21 21:57:20 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

extern volatile sig_atomic_t	g_signal_received;

static int	read_heredoc_line(char **line, t_mini *mini)
{
	g_signal_received = 0;
	*line = readline("> ");
	if (g_signal_received == SIGINT)
	{
		if (*line)
			free(*line);
		*line = NULL;
		mini->exit_sts = 130;
		return (1);
	}
	return (0);
}

static void	show_heredoc_warning(char *delimiter)
{
	ft_putstr_fd(ERR_HEREDOC, STDERR_FILENO);
	ft_putstr_fd(delimiter, STDERR_FILENO);
	ft_putstr_fd("')\n", STDERR_FILENO);
}

static int	process_heredoc_lines(t_cmd *cmd, t_mini *mini, char ***heredoc)
{
	char	*line;
	char	*expanded;

	while (1)
	{
		if (read_heredoc_line(&line, mini))
			return (1);
		if (!line)
		{
			show_heredoc_warning(cmd->redirs->target);
			return (0);
		}
		if (ft_strcmp(line, cmd->redirs->target) == 0)
		{
			free(line);
			return (0);
		}
		expanded = exp_str_part(line, mini, cmd->redirs->hd_expand);
		free(line);
		dbpt_push(heredoc, expanded);
		free(expanded);
	}
}

static int	handle_single_heredoc(t_cmd *cmd, t_mini *mini)
{
	char	**heredoc;
	int		result;
	int		stdin_backup;

	heredoc = NULL;
	stdin_backup = dup(STDIN_FILENO);
	setup_heredoc_signals();
	result = process_heredoc_lines(cmd, mini, &heredoc);
	setup_interactive_signals();
	if (g_signal_received == SIGINT)
	{
		dup2(stdin_backup, STDIN_FILENO);
		g_signal_received = 0;
	}
	close(stdin_backup);
	if (heredoc)
	{
		printf("========== HEREDOC ==========\n");
		print_dblptr(heredoc);						// printeo para COMPROBACIÓN [ debugging ]
		printf("=============================\n");
		free_dblptr(heredoc);
	}
	return (result);
}

int	handle_heredocs(t_mini *mini)
{
	t_cmd	*cmd;

	cmd = mini->cmds;
	while (cmd)
	{
		if (cmd->redirs && cmd->redirs->in_redir == 2)
		{
			if (handle_single_heredoc(cmd, mini))
				return (1);
		}
		cmd = cmd->next;
	}
	return (0);
}
