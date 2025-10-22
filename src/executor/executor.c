/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 20:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/22 17:08:19 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	execute_simple_command(t_mini *mini)
{
	(void)mini;
}

static void	execute_pipeline(t_mini *mini)
{
	(void)mini;
}

void	executor(t_mini *mini)
{
	int	cmd_count;

	if (!mini || !mini->cmds)
		return ;
	cmd_count = ft_lstsize(mini->cmds);
	if (handle_heredocs(mini))
		return ;

	// ------------------------------------------------------------------------
	// PASO 2: DECIDIR TIPO DE EJECUCION
	// ------------------------------------------------------------------------
	//
	// COMANDO SIMPLE (cmd_count == 1, sin pipes):
	//   1. Verificar si es builtin:
	//      - Si es builtin CON redirecciones:
	//        * Guardar stdin/stdout originales con dup()
	//        * Abrir archivos de redirecciones
	//        * Aplicar redirecciones con dup2()
	//        * Ejecutar builtin (en el padre)
	//        * Restaurar stdin/stdout originales
	//      - Si es builtin SIN redirecciones:
	//        * Ejecutar builtin directamente (en el padre)
	//
	//   2. Si NO es builtin (comando externo):
	//      - fork()
	//      - En hijo:
	//        * setup_execution_signals() (SIGINT/SIGQUIT default)
	//        * Abrir archivos de redirecciones (>, >>, <)
	//        * Aplicar redirecciones con dup2()
	//        * Aplicar heredoc si existe (dup2 del fd guardado)
	//        * Cerrar todos los fds
	//        * Buscar ejecutable en PATH
	//        * execve(path, tokens, env)
	//      - En padre:
	//        * Cerrar fds de heredoc
	//        * waitpid(pid, &status, 0)
	//        * mini->exit_sts = WEXITSTATUS(status)
	//
	// PIPELINE (cmd_count > 1, con pipes |):
	//   1. Crear pipes: n_pipes = cmd_count - 1
	//      pipes[0][0], pipes[0][1]  → Entre cmd0 y cmd1
	//      pipes[1][0], pipes[1][1]  → Entre cmd1 y cmd2
	//      ...
	//
	//   2. Para cada comando (índice i):
	//      - fork()
	//      - En hijo:
	//        * setup_execution_signals()
	//
	//        * Conectar pipes según posición:
	//          - Si NO es primer comando (i > 0):
	//            dup2(pipes[i-1][0], STDIN_FILENO)  (leer del pipe anterior)
	//          - Si NO es último comando (i < cmd_count-1):
	//            dup2(pipes[i][1], STDOUT_FILENO)   (escribir al pipe siguiente)
	//
	//        * Cerrar TODOS los pipes en el hijo
	//
	//        * Abrir archivos de redirecciones propias del comando
	//        * Aplicar redirecciones con dup2() (prevalecen sobre pipes)
	//        * Aplicar heredoc si existe
	//        * Cerrar fds de redirecciones
	//
	//        * Verificar si es builtin:
	//          - Si es builtin: ejecutar builtin y exit(exit_sts)
	//          - Si NO es builtin: buscar en PATH y execve()
	//
	//      - En padre:
	//        * Guardar pid del hijo
	//        * Continuar con siguiente comando
	//
	//   3. Después del loop:
	//      - Cerrar TODOS los pipes en el padre
	//      - waitpid() para cada hijo en orden
	//      - mini->exit_sts = status del ULTIMO comando
	//
	// CASOS ESPECIALES:
	// - Heredoc con pipes: El heredoc se procesa ANTES, su fd se usa en el hijo
	// - Builtin entre pipes: Se ejecuta en hijo (fork), NO en padre
	// - Redirecciones múltiples: La ULTIMA prevalece
	// - Comando vacío: return sin error
	//
	// ------------------------------------------------------------------------

	if (cmd_count == 1)
		execute_simple_command(mini);
	else
		execute_pipeline(mini);

	built_ins(mini);
	// printf("=========== ARGS ============\n");
	// print_dblptr(mini->cmds->tokens);
	// printf("=============================\n");
}
