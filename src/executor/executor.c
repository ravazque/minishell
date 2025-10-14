/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 20:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/14 15:46:09 by ravazque         ###   ########.fr       */
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
				// Si hd_expand == 0, NO expandir variables en el contenido
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

static void	execute_single_cmd(t_mini *mini, t_cmd *cmd)
{
	int	i;

	printf("\n=== Comando Simple ===\n");
	printf("Comando: %s\n", cmd->tokens[0]);
	
	// Imprimir argumentos
	i = 1;
	while (cmd->tokens[i])
	{
		printf("  Arg[%d]: %s\n", i, cmd->tokens[i]);
		i++;
	}
	
	// Procesar redirecciones de este comando
	if (cmd->redirs)
	{
		printf("Redirecciones:\n");
		process_redirections(cmd);
	}
	
	// Verificar si es builtin
	if (built_ins(mini))
	{
		printf("Es un builtin - ejecutado\n");
		return;
	}
	
	printf("Es un comando externo - TODO: implementar execve\n");
	// TODO: fork() y execve()
}

static void	execute_pipeline(t_mini *mini)
{
	t_cmd	*cmd;
	int	cmd_count;
	int	cmd_index;

	// Contar comandos
	cmd_count = 0;
	cmd = mini->cmds;
	while (cmd)
	{
		cmd_count++;
		cmd = cmd->next;
	}
	
	printf("\n=== Pipeline con %d comandos ===\n", cmd_count);
	
	// Ejecutar cada comando
	cmd = mini->cmds;
	cmd_index = 0;
	while (cmd)
	{
		printf("\nComando %d: %s\n", cmd_index, cmd->tokens[0]);
		
		// Mostrar argumentos
		int i = 1;
		while (cmd->tokens[i])
		{
			printf("  Arg[%d]: %s\n", i, cmd->tokens[i]);
			i++;
		}
		
		// Procesar redirecciones de este comando
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
