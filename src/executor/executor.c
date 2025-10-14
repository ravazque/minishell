/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 20:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/14 18:24:01 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	process_heredocs(t_mini *mini)
{
	t_cmd	*cmd;
	t_redir	*redir;

	cmd = mini->cmds;
	while (cmd)
	{
		redir = cmd->redirs;
		while (redir)
		{
			if (redir->in_redir == 2)
			{
				printf("Heredoc encontrado con delimitador: %s\n", redir->target);
				if (redir->hd_expand)
					printf("  -> Se expandirán variables\n");
				else
					printf("  -> NO se expandirán variables (delimitador con comillas)\n");
				// TODO: Implementar creación del heredoc
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
}

static void	process_redirections(t_cmd *cmd)
{
	t_redir	*redir;

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->in_redir == 1)
			printf("  Redirección de entrada: < %s\n", redir->target);
		else if (redir->in_redir == 2)
			printf("  Heredoc: << %s (expand=%d)\n", redir->target, redir->hd_expand);
		else if (redir->out_redir == 1)
			printf("  Redirección de salida: > %s\n", redir->target);
		else if (redir->out_redir == 2)
			printf("  Redirección append: >> %s\n", redir->target);
		// TODO: Implementar apertura de archivos y redirecciones
		redir = redir->next;
	}
}

static int	cmd_is_valid(t_cmd *cmd)
{
	if (!cmd)
		return (0);
	if (!cmd->tokens)
		return (0);
	if (!cmd->tokens[0])
		return (0);
	return (1);
}

static void	execute_single_cmd(t_mini *mini, t_cmd *cmd)
{
	int	i;

	if (!cmd_is_valid(cmd))
	{
		mini->exit_sts = 127;
		return ;
	}
	printf("\n=== Comando Simple ===\n");
	printf("Comando: %s\n", cmd->tokens[0]);
	i = 1;
	while (cmd->tokens[i])
	{
		printf("  Arg[%d]: %s\n", i, cmd->tokens[i]);
		i++;
	}
	if (cmd->redirs)
	{
		printf("Redirecciones:\n");
		process_redirections(cmd);
	}
	if (built_ins(mini))
	{
		printf("Es un builtin - ejecutado\n");
		return ;
	}
	printf("Es un comando externo - TODO: implementar execve\n");
	// TODO: fork() y execve()
}

static void	execute_pipeline(t_mini *mini)
{
	t_cmd	*cmd;
	int		cmd_count;
	int		cmd_index;
	int		i;

	cmd_count = 0;
	cmd = mini->cmds;
	while (cmd)
	{
		cmd_count++;
		cmd = cmd->next;
	}
	printf("\n=== Pipeline con %d comandos ===\n", cmd_count);
	cmd = mini->cmds;
	cmd_index = 0;
	while (cmd)
	{
		if (!cmd_is_valid(cmd))
		{
			cmd = cmd->next;
			cmd_index++;
			continue ;
		}
		printf("\nComando %d: %s\n", cmd_index, cmd->tokens[0]);
		i = 1;
		while (cmd->tokens[i])
		{
			printf("  Arg[%d]: %s\n", i, cmd->tokens[i]);
			i++;
		}
		if (cmd->redirs)
		{
			printf("Redirecciones:\n");
			process_redirections(cmd);
		}
		// TODO: Crear pipes y fork para cada comando
		cmd = cmd->next;
		cmd_index++;
	}
	printf("\nTODO: Implementar pipes y ejecución\n");
}

int	ft_lstsize(t_cmd *lst)
{
	int	count;

	count = 0;
	while (lst)
	{
		count++;
		lst = lst->next;
	}
	return (count);
}

void	executor(t_mini *mini)
{
	int	cmd_count;

	if (!mini || !mini->cmds)
		return ;
	process_heredocs(mini);
	cmd_count = ft_lstsize(mini->cmds);
	if (cmd_count == 1)
		execute_single_cmd(mini, mini->cmds);
	else
		execute_pipeline(mini);
}
