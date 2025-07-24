## 🧩 `fork()`  
<small><code>#include &lt;unistd.h&gt;</code></small>

---

### 🧾 Header
```c
#include <unistd.h>
```

---

### 🧪 Prototipo
```c
pid_t fork(void);
```

---

### 📚 Descripción
Crea un nuevo proceso duplicando el proceso que realiza la llamada. El nuevo proceso (proceso hijo) es una copia exacta del proceso original (proceso padre), excepto por ciertos detalles. Después de `fork()`, **ambos procesos ejecutan el mismo código**, pero pueden tomar diferentes caminos de ejecución basados en el valor de retorno de la función.

---

### 🧰 Sintaxis y ejemplo
```c
pid_t pid = fork();
if (pid == 0)
    printf("Soy el proceso hijo\n");
else if (pid > 0)
    printf("Soy el proceso padre, mi hijo tiene PID %d\n", pid);
```

<summary>🔍 Ejemplo de uso en contexto (Ejecución de comandos en Minishell)</summary>

```c
void execute_command(t_command *cmd, char **env)
{
    pid_t pid;
    
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return;
    }
    else if (pid == 0)
    {
        // Código ejecutado por el proceso hijo
        if (cmd->input_fd != STDIN_FILENO)
        {
            dup2(cmd->input_fd, STDIN_FILENO);
            close(cmd->input_fd);
        }
        if (cmd->output_fd != STDOUT_FILENO)
        {
            dup2(cmd->output_fd, STDOUT_FILENO);
            close(cmd->output_fd);
        }
        
        execve(cmd->path, cmd->args, env);
        perror("execve");  // Si execve falla
        exit(EXIT_FAILURE);
    }
    else
    {
        // Código ejecutado por el proceso padre
        wait_for_process(pid);
    }
}
```

---

### 💡 Consejos de uso
- La función **devuelve valores diferentes** en el proceso padre y en el hijo, lo que permite identificar en qué proceso nos encontramos.
- El proceso hijo hereda copias de los **descriptores de archivo** del padre, por lo que pueden comunicarse a través de tuberías.
- Para evitar **procesos zombies**, el proceso padre debe esperar a sus hijos usando `wait()` o `waitpid()`.
- Los programas concurrentes que usan `fork()` deben ser cuidadosos con **variables compartidas** y condiciones de carrera.
- El proceso hijo hereda la mayor parte del contexto del padre, incluyendo **variables de entorno**, **directorio de trabajo** y **espacio de direcciones**.

---

### 🔙 Retorno
- En el **proceso padre**: Devuelve el **PID del proceso hijo** (valor positivo).
- En el **proceso hijo**: Devuelve **0**.
- En caso de **error**: Devuelve **-1** y establece `errno` adecuadamente.

---

### ⚠️ Errores
- `EAGAIN`: El sistema no tiene recursos suficientes para crear un nuevo proceso o se ha alcanzado el límite de procesos del usuario.
- `ENOMEM`: No hay suficiente memoria para crear un nuevo proceso.
- `ENOSYS`: La función `fork()` no está implementada en este sistema.

---

### 🧭 Información adicional

<summary>📎 ¿Por qué se usa en Minishell?</summary>

En Minishell, `fork()` es fundamental para la ejecución de comandos externos, ya que permite:

1. **Ejecutar comandos sin bloquear el shell principal**:
   El shell principal (proceso padre) puede seguir funcionando mientras espera que los comandos terminen su ejecución en procesos separados.

2. **Gestionar múltiples procesos para pipes**:
```c
void execute_pipeline(t_command *cmd_list, char **env)
{
    int pipefd[2];
    pid_t pid;
    t_command *current = cmd_list;
    
    while (current && current->next)
    {
        pipe(pipefd);
        pid = fork();
        
        if (pid == 0)
        {
            // Configurar salida al pipe
            close(pipefd[0]);
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[1]);
            
            // Ejecutar comando
            execve(current->path, current->args, env);
            exit(EXIT_FAILURE);
        }
        
        // Configurar entrada desde pipe para el siguiente comando
        close(pipefd[1]);
        current->next->input_fd = pipefd[0];
        current = current->next;
    }
    
    // Ejecutar el último comando
    execute_command(current, env);
}
```

3. **Evitar que señales como SIGINT terminen todo el shell**:
   Al ejecutar comandos en procesos hijo, señales como Ctrl+C (SIGINT) terminan solo el comando en ejecución, no el shell completo.

El uso correcto de `fork()` junto con `waitpid()` y `execve()` es esencial para implementar un shell funcional que pueda ejecutar comandos y gestionar procesos correctamente.

---