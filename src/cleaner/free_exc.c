/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_exc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 15:22:09 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/23 15:22:24 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	cleanup_exec(t_exec *exec)
{
	if (exec->pipe_fds)
	{
		free(exec->pipe_fds);
		exec->pipe_fds = NULL;
	}
	if (exec->pids)
	{
		free(exec->pids);
		exec->pids = NULL;
	}
	if (exec->stdin_backup != -1)
	{
		close(exec->stdin_backup);
		exec->stdin_backup = -1;
	}
	if (exec->stdout_backup != -1)
	{
		close(exec->stdout_backup);
		exec->stdout_backup = -1;
	}
}
