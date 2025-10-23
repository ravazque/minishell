/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_collect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 14:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/23 18:29:32 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	restore_stdin(int stdin_backup)
{
	if (stdin_backup == -1)
		return ;
	dup2(stdin_backup, STDIN_FILENO);
	close(stdin_backup);
}

static int	handle_eof(char *delimiter)
{
	ft_putstr_fd(ERR_HEREDOC, STDERR_FILENO);
	ft_putstr_fd(delimiter, STDERR_FILENO);
	ft_putstr_fd("')\n", STDERR_FILENO);
	return (0);
}

static int	handle_interrupt(char ***lines, int stdin_backup, t_mini *mini)
{
	setup_interactive_signals();
	restore_stdin(stdin_backup);
	if (lines && *lines)
	{
		free_dblptr(*lines);
		*lines = NULL;
	}
	mini->exit_sts = 130;
	return (1);
}

static int	read_single_line(char **line, t_mini *mini, int stdin_backup, char ***lines)
{
	g_signal_received = 0;
	*line = readline("> ");
	if (g_signal_received == SIGINT)
	{
		if (*line)
		{
			free(*line);
			*line = NULL;
		}
		return (handle_interrupt(lines, stdin_backup, mini));
	}
	if (!*line)
		return (-1);
	return (0);
}

static int	process_line(char *line, char *delimiter, int expand, t_mini *mini, char ***lines)
{
	char	*expanded;

	if (ft_strcmp(line, delimiter) == 0)
	{
		free(line);
		return (1);
	}
	if (expand)
		expanded = exp_str_part(line, mini, 1);
	else
		expanded = ft_strdup(line);
	free(line);
	if (!expanded)
		return (-1);
	dbpt_push(lines, expanded);
	free(expanded);
	return (0);
}

int	collect_heredoc_lines(char *delimiter, t_mini *mini, int expand, char ***lines)
{
	char	*line;
	int		stdin_backup;
	int		result;

	*lines = NULL;
	stdin_backup = dup(STDIN_FILENO);
	if (stdin_backup == -1)
		return (1);
	setup_heredoc_signals();
	while (1)
	{
		result = read_single_line(&line, mini, stdin_backup, lines);
		if (result == 1)
			return (1);
		if (result == -1)
		{
			setup_interactive_signals();
			restore_stdin(stdin_backup);
			return (handle_eof(delimiter));
		}
		result = process_line(line, delimiter, expand, mini, lines);
		if (result == 1)
			break ;
		if (result == -1)
		{
			setup_interactive_signals();
			restore_stdin(stdin_backup);
			if (*lines)
				free_dblptr(*lines);
			return (1);
		}
	}
	setup_interactive_signals();
	restore_stdin(stdin_backup);
	return (0);
}
