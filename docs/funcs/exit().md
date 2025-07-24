## 🧩 `exit()`  
<small><code>#include &lt;stdlib.h&gt;</code></small>

---

### 🧾 Header
```c
#include <stdlib.h>
```

---

### 🧪 Prototipo
```c
void exit(int status);
```

---

### 📚 Descripción
Termina inmediatamente el programa en ejecución y **devuelve el control al sistema operativo**. El valor de `status` es devuelto al proceso padre y puede ser recuperado mediante `waitpid()`. Antes de finalizar, `exit()` llama a todas las funciones registradas con `atexit()` y `on_exit()`, cierra todos los flujos abiertos, elimina archivos temporales y realiza otras tareas de limpieza.

---

### 🧰 Sintaxis y ejemplo
```c
if (error_occurred)
    exit(EXIT_FAILURE);  // Terminar con código de error (1)
// ...
exit(EXIT_SUCCESS);      // Terminar con éxito (0)
```

<summary>🔍 Ejemplo de uso en contexto (Manejo de errores en Minishell)</summary>

```c
void execute_child_process(t_command *cmd, char **env)
{
    // Configurar redirecciones
    if (cmd->input_fd != STDIN_FILENO)
    {
        if (dup2(cmd->input_fd, STDIN_FILENO) == -1)
        {
            perror("minishell: dup2");
            exit(1);
        }
        close(cmd->input_fd);
    }
    
    if (cmd->output_fd != STDOUT_FILENO)
    {
        if (dup2(cmd->output_fd, STDOUT_FILENO) == -1)
        {
            perror("minishell: dup2");
            exit(1);
        }
        close(cmd->output_fd);
    }
    
    // Ejecutar el comando
    if (execve(cmd->path, cmd->args, env) == -1)
    {
        perror("minishell");
        if (errno == ENOENT)
            exit(127);  // Comando no encontrado
        else if (errno == EACCES)
            exit(126);  // Permiso denegado
        exit(1);        // Otro error
    }
}
```

---

### 💡 Consejos de uso
- Las constantes `EXIT_SUCCESS` (0) y `EXIT_FAILURE` (1) están definidas en `<stdlib.h>` y son recomendadas para mayor portabilidad.
- Por convención en los shells Unix:
  - **0**: Éxito
  - **1-125**: Errores genéricos
  - **126**: Comando encontrado pero no ejecutable
  - **127**: Comando no encontrado
  - **128+N**: Terminado por señal N (ej. 130 = SIGINT)
- `exit()` **no cierra descriptores de archivo heredados** por otros procesos, lo que puede causar comportamientos inesperados en tuberías.
- Usar `_exit()` (de `<unistd.h>`) en lugar de `exit()` después de `fork()` evita llamar a manejadores de terminación registrados, lo cual puede ser deseable en ciertos casos.

---

### 🔙 Retorno
`exit()` **nunca retorna** a la función que la llama.

---

### ⚠️ Errores
`exit()` no tiene errores ya que nunca retorna. Sin embargo, comportamientos a tener en cuenta:

- Si se llama desde **constructores/destructores globales**, puede causar bucles infinitos.
- Si las funciones registradas con `atexit()` nunca retornan, el programa nunca terminará.
- Si los **recursos no son liberados adecuadamente** antes de llamar a `exit()`, puede causar fugas de memoria o recursos.

---

### 🧭 Información adicional

<summary>📎 ¿Por qué se usa en Minishell?</summary>

En Minishell, `exit()` se utiliza principalmente en tres contextos:

1. **Como comando incorporado (builtin)** para finalizar el shell:
```c
int ft_exit(char **args, t_shell *shell)
{
    int exit_code;
    
    write(1, "exit\n", 5);
    
    // Si hay argumentos, usar el primero como código de salida
    if (args[1])
    {
        if (!is_numeric(args[1]))
        {
            ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
            exit(255);
        }
        else if (args[2])
        {
            ft_putstr_fd("minishell: exit: too many arguments\n", 2);
            return (1);  // Error pero no salir
        }
        exit_code = ft_atoi(args[1]) % 256;
    }
    else
        exit_code = shell->exit_status;  // Usar el último código de salida
        
    // Liberar recursos antes de salir
    clean_shell(shell);
    exit(exit_code);
}
```

2. **En procesos hijos** después de ejecutar comandos o cuando ocurren errores:
```c
if (pid == 0)  // Proceso hijo
{
    // Configurar redirecciones...
    
    if (execve(cmd->path, cmd->args, env) == -1)
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(cmd->args[0], 2);
        ft_putstr_fd(": ", 2);
        ft_putstr_fd(strerror(errno), 2);
        ft_putstr_fd("\n", 2);
        exit(127);  // Comando no encontrado
    }
}
```

3. **Para manejar errores fatales** que impiden continuar la ejecución:
```c
void *ft_malloc(size_t size)
{
    void *ptr;
    
    ptr = malloc(size);
    if (!ptr)
    {
        ft_putstr_fd("minishell: fatal error: memory allocation failed\n", 2);
        exit(1);
    }
    return (ptr);
}
```

El uso correcto de los códigos de salida es esencial para que los scripts de shell y otras herramientas puedan interpretar correctamente el resultado de la ejecución de comandos.

---