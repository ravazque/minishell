/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 16:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/13 02:24:46 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	cleanup_child_fds(void)
{
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
}

int	has_redirs(t_cmd *cmd)
{
	if (!cmd)
		return (0);
	return (cmd->redirs != NULL);
}

int	count_args(char **tokens)
{
	int	i;

	i = 0;
	if (!tokens)
		return (0);
	while (tokens[i])
		i++;
	return (i);
}

int	is_empty_cmd(t_cmd *cmd)
{
	if (!cmd)
		return (1);
	if (!cmd->tokens || !cmd->tokens[0])
		return (1);
	if (cmd->tokens[0][0] == '\0')
		return (1);
	return (0);
}

static void puterr(char *target)
{
	
}

static int	validate_redir_files(t_redir *redirs)
{
	t_redir	*redir;
	int		fd;

	redir = redirs;
	while (redir)
	{
		if (redir->in_redir == 1)
		{
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
			close(fd);
		}
		else if (redir->out_redir == 1)
		{
			fd = open(redir->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
				return (ft_putstr_fd("minishell: ", STDERR_FILENO),
					ft_putstr_fd(redir->target, STDERR_FILENO),
					ft_putstr_fd(": ", STDERR_FILENO),
					ft_putstr_fd(strerror(errno), STDERR_FILENO),
					ft_putstr_fd("\n", STDERR_FILENO), 1);
			close(fd);
		}
		else if (redir->out_redir == 2)
		{
			fd = open(redir->target, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
				return (ft_putstr_fd("minishell: ", STDERR_FILENO),
					ft_putstr_fd(redir->target, STDERR_FILENO),
					ft_putstr_fd(": ", STDERR_FILENO),
					ft_putstr_fd(strerror(errno), STDERR_FILENO),
					ft_putstr_fd("\n", STDERR_FILENO), 1);
			close(fd);
		}
		redir = redir->next;
	}
	return (0);
}

static int	handle_only_redirections(t_mini *mini)
{
	if (validate_redir_files(mini->cmds->redirs))
	{
		mini->exit_sts = 1;
		return (1);
	}
	mini->exit_sts = 0;
	return (0);
}

int	check_empty_command(t_mini *mini)
{
	if (!mini->cmds->tokens || !mini->cmds->tokens[0])
	{
		if (mini->cmds->redirs)
			return (handle_only_redirections(mini));
		mini->exit_sts = 0;
		return (1);
	}
	if (mini->cmds->tokens[0][0] == '\0')
	{
		ft_putstr_fd("minishell: : command not found\n", STDERR_FILENO);
		mini->exit_sts = 127;
		return (1);
	}
	return (0);
}
