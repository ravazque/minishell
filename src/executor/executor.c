/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 20:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/14 20:36:54 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	executor(t_mini *mini)
{
	int	cmd_count;

	if (!mini || !mini->cmds)
		return ;
	process_heredocs(mini);
	cmd_count = ft_lstsize(mini->cmds);
	
	// if (cmd_count == 1)
	// 	execute_single_cmd(mini, mini->cmds);
	// else
	// 	execute_pipeline(mini);
}

// ====================================================== //

// static void	process_heredocs(t_mini *mini)
// {
// 	t_cmd	*cmd;
// 	t_redir	*redir;

// 	cmd = mini->cmds;
// 	while (cmd)
// 	{
// 		redir = cmd->redirs;
// 		while (redir)
// 		{
// 			if (redir->in_redir == 2)
// 			{
// 				printf("Heredoc encontrado con delimitador: %s\n", redir->target);
// 				if (redir->hd_expand)
// 					printf("  -> Se expandirán variables\n");
// 				else
// 					printf("  -> NO se expandirán variables (delimitador con comillas)\n");

// 				// TODO: Implementar creación del heredoc
// 			}
// 			redir = redir->next;
// 		}
// 		cmd = cmd->next;
// 	}
// }

// static void	process_redirections(t_cmd *cmd)
// {
// 	t_redir	*redir;

// 	redir = cmd->redirs;
// 	while (redir)
// 	{
// 		if (redir->in_redir == 1)
// 			printf("  Redirección de entrada: < %s\n", redir->target);
// 		else if (redir->in_redir == 2)
// 			printf("  Heredoc: << %s (expand=%d)\n", redir->target, redir->hd_expand);
// 		else if (redir->out_redir == 1)
// 			printf("  Redirección de salida: > %s\n", redir->target);
// 		else if (redir->out_redir == 2)
// 			printf("  Redirección append: >> %s\n", redir->target);

// 		// TODO: Implementar apertura de archivos y redirecciones

// 		redir = redir->next;
// 	}
// }

// static void	print_cmd_info(t_cmd *cmd, int index)
// {
// 	int	i;

// 	if (index >= 0)
// 		printf("\n--- Comando %d ---\n", index);
// 	else
// 		printf("\n=== Comando Simple ===\n");
	
// 	if (!cmd->tokens || !cmd->tokens[0])
// 	{
// 		printf("(comando vacío)\n");
// 		return;
// 	}
	
// 	printf("Comando: %s\n", cmd->tokens[0]);
// 	i = 1;
// 	while (cmd->tokens[i])
// 	{
// 		printf("  Arg[%d]: %s\n", i, cmd->tokens[i]);
// 		i++;
// 	}
// 	if (cmd->redirs)
// 	{
// 		printf("Redirecciones:\n");
// 		process_redirections(cmd);
// 	}
// }

// static void	execute_single_cmd(t_mini *mini, t_cmd *cmd)
// {
// 	print_cmd_info(cmd, -1);
// 	if (!cmd->tokens || !cmd->tokens[0])
// 	{
// 		mini->exit_sts = 0;
// 		return;
// 	}
// 	if (built_ins(mini))
// 	{
// 		printf("-> Es un builtin - ejecutado\n");
// 		return;
// 	}
	
// 	printf("-> Es un comando externo - TODO: implementar execve\n");

// 	// TODO: fork() y execve()
// }

// static void	execute_pipeline(t_mini *mini)
// {
// 	t_cmd	*cmd;
// 	int		cmd_count;
// 	int		cmd_index;

// 	cmd_count = ft_lstsize(mini->cmds);
// 	printf("\n========== Pipeline con %d comandos ==========\n", cmd_count);
	
// 	cmd = mini->cmds;
// 	cmd_index = 0;
// 	while (cmd)
// 	{
// 		print_cmd_info(cmd, cmd_index);
		
// 		if (cmd->tokens && cmd->tokens[0])
// 		{
// 			if (built_ins(mini))
// 				printf("-> Es un builtin\n");
// 			else
// 				printf("-> Es un comando externo\n");
// 		}
		
// 		cmd = cmd->next;
// 		cmd_index++;
// 	}
	
// 	printf("\n==============================================\n");
// 	printf("TODO: Implementar pipes y ejecución\n");
// 	printf("==============================================\n\n");
// }
