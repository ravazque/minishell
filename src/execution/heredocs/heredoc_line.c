/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_line.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 18:35:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/30 18:31:35 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	handle_eof(char *delimiter)
{
	ft_putstr_fd("minishell: warning: ", STDERR_FILENO);
	ft_putstr_fd(ERR_HEREDOC, STDERR_FILENO);
	ft_putstr_fd(delimiter, STDERR_FILENO);
	ft_putstr_fd("')\n", STDERR_FILENO);
	return (0);
}

int	handle_line_error(t_heredoc *ctx)
{
	setup_interactive_signals();
	restore_stdin(ctx->stdin_backup);
	if (*ctx->lines)
		free_dblptr(*ctx->lines);
	return (1);
}

int	handle_interrupt(t_heredoc *ctx)
{
	setup_interactive_signals();
	restore_stdin(ctx->stdin_backup);
	if (ctx->lines && *ctx->lines)
	{
		free_dblptr(*ctx->lines);
		*ctx->lines = NULL;
	}
	ctx->mini->exit_sts = 130;
	return (1);
}
