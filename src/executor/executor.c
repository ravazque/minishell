/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ptrapero <ptrapero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 20:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/22 22:15:48 by ptrapero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// void	executor(t_mini *mini)
// {
// 	int	cmd_count;

// 	if (!mini || !mini->cmds)
// 		return ;
// 	cmd_count = ft_lstsize(mini->cmds);
// 	if (handle_heredocs(mini))
// 		return ;

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

	// if (cmd_count == 1)
	// 	execute_simple_command(mini);
	// else
	// 	execute_pipeline(mini);

	// built_ins(mini);
	// printf("=========== ARGS ============\n");
	// print_dblptr(mini->cmds->tokens);
	// printf("=============================\n");
// }

void	ft_error(char *message)
{
	ft_putstr_fd("\033[1;41;5m ERROR \033[0m\n", 2);
	ft_putstr_fd("\033[91m", 2);
	perror(message);
	ft_putstr_fd("\033[0m", 2);
	exit(EXIT_FAILURE);
}

char	*ft_get_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	int		i;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		return (NULL);
	paths = ft_split(envp[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		path = ft_strjoin3(paths[i], "/", cmd);
		if (access(path, X_OK) == 0)
		{
			free_dblptr(paths);
			return (path);
		}
		else
			free(path);
		i++;
	}
	free_dblptr(paths);
	return (NULL);
}

void	ft_execve(char **argv, char **envp)
{
	char	**cmd;
	char	*path;

	cmd = argv;
	if (!cmd || !cmd[0])
		ft_error("missing command");
	path = ft_get_path(cmd[0], envp);
	if (!path)
	{
		free(path);
		ft_error("path not found");
	}
	if (execve(path, cmd, envp) == -1)
		ft_error("execve error");
}

void	ft_close(int *fd, int index, int n_fd)
{
	int	i;

	i = 0;
	while (i < n_fd)
	{
		if (i != ((index - 1) * 2) && i != (index * 2 + 1))
			close(fd[i]);
		i++;
	}
}

void	ft_close_and_wait(int *fd, int n_fd, pid_t *pid, int n_cmd)
{
	int	i;

	i = 0;
	while (i < n_fd)
	{
		close(fd[i]);
		i++;
	}
	i = 0;
	while (i < n_cmd)
	{
		waitpid(pid[i], NULL, 0);
		i++;
	}
}

/*void	ft_pid(t_cmd *cmd_aux, t_mini *mini)
{
	pid_t	pid;

	
}*/

void	executor(t_mini *mini)
{
	t_cmd	*cmd_aux;
	int		n_cmd;
	pid_t	*pid;
	int		*fd;
	int		i;
	int		index;

	if (!mini || !mini->cmds)
		return ;
	cmd_aux = mini->cmds;
	n_cmd = ft_lstsize(mini->cmds);
	if (handle_heredocs(mini))
		return ;
	if (n_cmd == 1 && is_builtin_cmd(cmd_aux->tokens[0]) == 1)
	{
		built_ins(mini, cmd_aux);
		return ;
	}
	pid = malloc(sizeof(pid_t) * n_cmd);
	fd = malloc(sizeof(int) * 2 * (n_cmd - 1));
	if (!fd)
	{
		free(pid);
		return ;
	}
	i = 0;
	index = 0;
	//ft_pipes();
	while (i < (n_cmd - 1))
	{
		if (pipe(&fd[i * 2]) == -1)
		{
			ft_error("pipe error");
			free(fd);
			free(pid);
			return ;
		}
		i++;
	}
	while (cmd_aux)
	{
		
		
		//ft_pid(cmd_aux, mini);
		
		pid[index] = fork();
		if (pid[index] == -1)
		{
			ft_error("fork error");
			free(fd);
			free(pid);
			return ;
		}
		else if (pid[index] == 0)
		{
			ft_close(fd, index, 2 * (n_cmd - 1));
			if (index == 0)//si es el primero
			{
				dup2(fd[1], STDOUT_FILENO);
			}
			else if (cmd_aux->next == NULL)//si es el ultimo
			{
				dup2(fd[(index - 1) * 2], STDIN_FILENO);
			}
			else
			{
				dup2(fd[(index - 1) * 2], STDIN_FILENO);
				dup2(fd[index * 2 + 1], STDOUT_FILENO);
			}
			i = 0;
			while (i < (2 * (n_cmd - 1)))
			{
				close(fd[i]);
				i++;
			}
			if (built_ins(mini, cmd_aux) == false)
				ft_execve(cmd_aux->tokens, mini->env);
			else
				exit(EXIT_SUCCESS);
		}
		cmd_aux = cmd_aux->next;
		index++;
	}
	ft_close_and_wait(fd, 2 * (n_cmd - 1), pid, n_cmd);
	free(fd);
	free(pid);
}
