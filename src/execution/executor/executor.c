/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 23:16:26 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/03 21:01:49 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	fork_single_cmd(t_mini *mini, t_cmd *cmd, t_exec *exec, int idx)
{
	exec->pids[idx] = fork();
	if (exec->pids[idx] == -1)
	{
		ft_putstr_fd("minishell: error: fork failed\n", STDERR_FILENO);
		close_pipes(exec);
		return (1);
	}
	if (exec->pids[idx] == 0)
		execute_child_process(mini, cmd, exec, idx);
	return (0);
}

static int	execute_pipeline(t_mini *mini, t_exec *exec)
{
	t_cmd	*cmd;
	int		idx;

	if (create_pipes(exec))
	{
		ft_putstr_fd("minishell: error: pipe creation failed\n",
			STDERR_FILENO);
		return (1);
	}
	cmd = mini->cmds;
	idx = 0;
	while (cmd)
	{
		if (fork_single_cmd(mini, cmd, exec, idx))
			return (1);
		cmd = cmd->next;
		idx++;
	}
	close_pipes(exec);
	return (wait_processes(exec, mini));
}

static int	check_heredocs(t_mini *mini)
{
	if (heredocs(mini))
	{
		mini->exit_sts = 130;
		return (1);
	}
	return (0);
}

static int	is_exit_builtin(t_mini *mini, int n_cmds)
{
	if (n_cmds != 1)
		return (0);
	if (!mini->cmds->tokens || !mini->cmds->tokens[0])
		return (0);
	if (ft_strcmp(mini->cmds->tokens[0], "exit") == 0)
		return (1);
	return (0);
}

void	executor(t_mini *mini)
{
	t_exec	exec;
	int		n_cmds;

	if (!mini || !mini->cmds)
		return ;
	n_cmds = ft_lstsize(mini->cmds);
	if (check_heredocs(mini) || check_empty_command(mini))
		return ;
	if (is_exit_builtin(mini, n_cmds))
	{
		execute_single_command(mini, mini->cmds);
		return ;
	}
	if (init_exec(&exec, n_cmds))
	{
		ft_putstr_fd("minishell: error: executor init failed\n",
			STDERR_FILENO);
		mini->exit_sts = 1;
		return ;
	}
	if (n_cmds == 1)
		execute_single_command(mini, mini->cmds);
	else
		execute_pipeline(mini, &exec);
	cleanup_exec(&exec);
}
