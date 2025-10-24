/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ptrapero <ptrapero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 19:28:04 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/24 18:53:02 by ptrapero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	init_exec(t_exec *exec, int n_cmds)
{
	exec->n_cmds = n_cmds;
	exec->n_pipes = n_cmds - 1;
	exec->stdin_backup = -1;
	exec->stdout_backup = -1;
	exec->pipe_fds = NULL;
	exec->pids = NULL;
	if (n_cmds > 1)
	{
		exec->pipe_fds = malloc(sizeof(int) * exec->n_pipes * 2);
		if (!exec->pipe_fds)
			return (malloc_error(), 1);
	}
	exec->pids = malloc(sizeof(pid_t) * n_cmds);
	if (!exec->pids)
	{
		if (exec->pipe_fds)
			free(exec->pipe_fds);
		return (malloc_error(), 1);
	}
	return (0);
}

void	init_mini(t_mini *mini, int argc, char *argv[], char *envp[])
{
	char	*path;
	char	*home_value;

	mini->cmds = NULL;
	mini->prompt = NULL;
	mini->input = NULL;
	mini->pwd = NULL;
	mini->exit_sts = 0;
	mini->apology_mode = 0;
	mini->argc = argc;
	mini->cd_home = NULL;
	mini->flag_error = 0;
	mini->env = ft_copy_dblptr(envp);
	if (!mini->env)
	{
		ft_putstr_fd(INIT_ERR_ENV, STDERR_FILENO);
		exit(1);
	}
	setup_mshlvl(mini);
	mini->argv = ft_copy_dblptr(argv);
	if (!mini->argv)
	{
		free_dblptr(mini->env);
		ft_putstr_fd(INIT_ERR_ARGS, STDERR_FILENO);
		exit(1);
	}
	home_value = get_local_env("HOME", envp);
	if (home_value)
	{
		mini->cd_home = ft_strdup(home_value);
		if (!mini->cd_home)
		{
			free_dblptr(mini->env);
			free_dblptr(mini->argv);
			ft_putstr_fd(INIT_ERR_HOME, STDERR_FILENO);
			exit(1);
		}
	}
	path = NULL;
	if (argv[0])
	{
		path = ft_strjoin("PATH=", get_local_env("PATH", envp));
		if (!path)
		{
			if (mini->cd_home)
				free(mini->cd_home);
			free_dblptr(mini->env);
			free_dblptr(mini->argv);
			ft_putstr_fd("minishell: fatal error: failed to alloc arguments\n", STDERR_FILENO);
			exit(1);
		}
		ft_setenv("_", path, &(mini->env));
		free(path);
	}
	else
		ft_setenv("_", "./minishell", &(mini->env));
}
