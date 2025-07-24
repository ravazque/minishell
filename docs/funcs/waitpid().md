## 🧩 `waitpid()`  
<small><code>#include &lt;sys/wait.h&gt;</code></small>

---

### 🧾 Header
```c
#include <sys/wait.h>
```

---

### 🧪 Prototipo
```c
pid_t waitpid(pid_t pid, int *status, int options);
```

---

### 📚 Descripción
Suspende la ejecución del proceso que realiza la llamada hasta que un hijo específico cambia de estado. Permite **esperar a que finalice un proceso hijo concreto** y obtener información sobre cómo terminó (normalmente, por señal, etc.). A diferencia de `wait()`, `waitpid()` ofrece más control sobre qué proceso hijo esperar y cómo comportarse durante la espera.

---

### 🧰 Sintaxis y ejemplo
```c
int status;
pid_t child_pid = fork();

if (child_pid == 0) {
    // Código del proceso hijo
    exit(42);  // Terminar con código 42
} else {
    // Código del proceso padre
    waitpid(child_pid, &status, 0);  // Esperar a que el hijo termine
    
    if (WIFEXITED(status))
        printf("El hijo terminó con código: %d\n", WEXITSTATUS(status));
}
```

<summary>🔍 Ejemplo de uso en contexto (Ejecución de comandos en Minishell)</summary>

```c
int execute_command(t_command *cmd, char **env)
{
    pid_t pid;
    int status;
    
    pid = fork();
    if (pid == -1)
    {
        perror("minishell: fork");
        return (1);
    }
    else if (pid == 0)
    {
        // Código del proceso hijo
        if (redirect_io(cmd) != 0)
            exit(1);
            
        execve(cmd->path, cmd->args, env);
        perror("minishell");
        exit(127);  // Comando no encontrado
    }
    else
    {
        // Código del proceso padre
        waitpid(pid, &status, 0);
        
        // Determinar y devolver el código de salida
        if (WIFEXITED(status))
            return (WEXITSTATUS(status));
        else if (WIFSIGNALED(status))
            return (128 + WTERMSIG(status));  // Convención de shell
            
        return (1);
    }
}
```

---

### 💡 Consejos de uso
- El parámetro `pid` permite especificar qué proceso hijo esperar:
  - `pid > 0`: Espera al hijo con PID igual a `pid`.
  - `pid = 0`: Espera a cualquier hijo en el mismo grupo de procesos.
  - `pid = -1`: Espera a cualquier hijo (equivalente a `wait()`).
  - `pid < -1`: Espera a cualquier hijo cuyo grupo de procesos sea igual al valor absoluto de `pid`.

- El parámetro `options` puede incluir:
  - `WNOHANG`: No bloquea si ningún hijo ha terminado.
  - `WUNTRACED`: Retorna también si los hijos están parados, no solo si terminan.
  - `WCONTINUED`: Retorna también si un hijo parado ha sido continuado.

- Para determinar cómo terminó el proceso hijo:
  - `WIFEXITED(status)`: True si el hijo terminó normalmente.
  - `WEXITSTATUS(status)`: Devuelve el código de salida del hijo (0-255).
  - `WIFSIGNALED(status)`: True si el hijo terminó por una señal.
  - `WTERMSIG(status)`: Devuelve el número de la señal que causó la terminación.
  - `WIFSTOPPED(status)`: True si el hijo está actualmente parado.
  - `WSTOPSIG(status)`: Devuelve el número de la señal que paró al proceso.
  - `WIFCONTINUED(status)`: True si el hijo ha sido continuado.

---

### 🔙 Retorno
- Si `options` incluye `WNOHANG` y no hay hijos disponibles: Devuelve **0**.
- Si tiene éxito: Devuelve el **PID del hijo** que cambió de estado.
- En caso de error: Devuelve **-1** y establece `errno` adecuadamente.

---

### ⚠️ Errores
- `ECHILD`: No existe ningún hijo no esperado con el PID especificado o el valor de `pid` es inválido.
- `EINTR`: La función fue interrumpida por una señal.
- `EINVAL`: El valor de `options` no es válido.

---

### 🧭 Información adicional

<summary>📎 ¿Por qué se usa en Minishell?</summary>

En Minishell, `waitpid()` es crucial por varias razones:

1. **Evitar procesos zombies**:
   Cuando un proceso hijo termina, mantiene una entrada en la tabla de procesos hasta que el padre "recoge" su estado con `waitpid()`. Si no se hace, se convierte en un "zombie".

2. **Obtener el código de salida de los comandos**:
```c
int g_exit_status = 0;  // Variable global para el código de salida

void handle_command(t_command *cmd)
{
    // Ejecutar comando...
    
    // Esperar y actualizar el código de salida
    waitpid(pid, &status, 0);
    
    if (WIFEXITED(status))
        g_exit_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        g_exit_status = 128 + WTERMSIG(status);
}
```

3. **Gestionar la ejecución de pipelines**:
   Para pipelines como `cmd1 | cmd2 | cmd3`, es necesario esperar a que todos los procesos terminen:
```c
void wait_for_pipeline(pid_t *pids, int count)
{
    int i;
    int status;
    
    for (i = 0; i < count; i++)
    {
        waitpid(pids[i], &status, 0);
        
        // Guardar el estado del último comando del pipeline
        if (i == count - 1)
        {
            if (WIFEXITED(status))
                g_exit_status = WEXITSTATUS(status);
            else if (WIFSIGNALED(status))
                g_exit_status = 128 + WTERMSIG(status);
        }
    }
}
```

4. **Manejo de señales**:
   `waitpid()` permite detectar si un proceso hijo fue terminado por una señal (como Ctrl+C):
```c
if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
{
    // El proceso fue terminado por Ctrl+C
    write(1, "\n", 1);  // Nueva línea después de ^C
}
```

El uso correcto de `waitpid()` asegura que los recursos del sistema se liberen adecuadamente y que el shell pueda manejar correctamente los códigos de salida y señales.

---