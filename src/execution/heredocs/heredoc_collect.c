/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_collect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 14:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/30 18:24:00 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	restore_stdin(int stdin_backup)
{
	if (stdin_backup == -1)
		return ;
	dup2(stdin_backup, STDIN_FILENO);
	close(stdin_backup);
}

static int	read_single_line(char **line, t_heredoc *ctx)
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
		return (handle_interrupt(ctx));
	}
	if (!*line)
		return (-1);
	return (0);
}

static int	process_line(char *line, t_heredoc *ctx)
{
	char	*expanded;

	if (ft_strcmp(line, ctx->delimiter) == 0)
	{
		free(line);
		return (1);
	}
	if (ctx->expand)
		expanded = exp_str_part(line, ctx->mini, 1);
	else
		expanded = ft_strdup(line);
	free(line);
	if (!expanded)
		return (-1);
	dbpt_push(ctx->lines, expanded);
	free(expanded);
	return (0);
}

static int	init_heredoc_stdin(t_heredoc *ctx, char ***lines, int expand)
{
	ctx->expand = expand;
	ctx->lines = lines;
	*lines = NULL;
	ctx->stdin_backup = dup(STDIN_FILENO);
	if (ctx->stdin_backup == -1)
		return (1);
	return (0);
}

int	collect_lines(char *delim, t_mini *mini, int expand, char ***lines)
{
	t_heredoc	ctx;
	char			*line;
	int				result;

	ctx.delimiter = delim;
	ctx.mini = mini;
	if (init_heredoc_stdin(&ctx, lines, expand))
		return (1);
	setup_heredoc_signals();
	while (1)
	{
		result = read_single_line(&line, &ctx);
		if (result == 1)
			return (1);
		if (result == -1)
			return (setup_interactive_signals(),
				restore_stdin(ctx.stdin_backup), handle_eof(delim));
		result = process_line(line, &ctx);
		if (result == 1)
			break ;
		if (result == -1)
			return (handle_line_error(&ctx));
	}
	return (setup_interactive_signals(), restore_stdin(ctx.stdin_backup), 0);
}
