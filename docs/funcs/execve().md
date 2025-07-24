## 🧩 `execve()`  
<small><code>#include &lt;unistd.h&gt;</code></small>

---

### 🧾 Header
```c
#include <unistd.h>
```

---

### 🧪 Prototipo
```c
int execve(const char *pathname, char *const argv[], char *const envp[]);
```

---

### 📚 Descripción
Ejecuta el programa referido por `pathname`. Esta función **reemplaza completamente el proceso actual** por el nuevo programa, incluyendo el código, los datos y la pila. Si tiene éxito, no regresa a la función que la llama. Los argumentos para el nuevo programa se pasan en `argv`, y el entorno del nuevo programa se especifica a través de `envp`.

---

### 🧰 Sintaxis y ejemplo
```c
char *args[] = {"/bin/ls", "-l", NULL};
char *env[] = {"PATH=/bin:/usr/bin", NULL};

execve("/bin/ls", args, env);
perror("execve");  // Solo se ejecuta si execve falla
```

<summary>🔍 Ejemplo de uso en contexto (Ejecución de comandos en Minishell)</summary>

```c
void execute_external_command(t_command *cmd, char **env)
{
    pid_t pid;
    
    pid = fork();
    if (pid == -1)
    {
        perror("minishell: fork");
        return;
    }
    else if (pid == 0)  // Proceso hijo
    {
        // Configurar redirecciones
        setup_redirections(cmd);
        
        // Buscar el path completo del comando
        char *command_path = find_command_path(cmd->args[0], env);
        if (!command_path)
        {
            ft_putstr_fd("minishell: ", 2);
            ft_putstr_fd(cmd->args[0], 2);
            ft_putstr_fd(": command not found\n", 2);
            exit(127);
        }
        
        // Ejecutar el comando
        execve(command_path, cmd->args, env);
        
        // Si execve falla
        perror("minishell");
        free(command_path);
        exit(126);  // Comando encontrado pero no ejecutable
    }
    else  // Proceso padre
    {
        int status;
        
        waitpid(pid, &status, 0);
        update_exit_status(status);
    }
}
```

---

### 💡 Consejos de uso
- `pathname` debe ser una **ruta completa** al ejecutable, no solo el nombre del comando.
- `argv` debe ser un array de strings que termina con `NULL`. Por convención, `argv[0]` es el nombre del programa.
- `envp` debe ser un array de strings en formato `"VARIABLE=valor"` que termina con `NULL`.
- Después de una llamada exitosa a `execve()`, **todo el proceso es reemplazado**, por lo que cualquier código después de la llamada nunca se ejecutará.
- Los **descriptores de archivo** permanecen abiertos a través de `execve()` a menos que tengan establecido el flag `FD_CLOEXEC`.
- Para ejecutar un comando por su nombre (ej. "ls") en lugar de su ruta completa, es necesario buscar en los directorios del `PATH`.

---

### 🔙 Retorno
Si tiene éxito, `execve()` **no retorna**.  
En caso de error, devuelve **-1** y establece `errno` adecuadamente.

---

### ⚠️ Errores
- `E2BIG`: La lista de argumentos es demasiado larga.
- `EACCES`: Permiso denegado (el archivo no es ejecutable o no se puede acceder a él).
- `EFAULT`: `pathname`, `argv` o `envp` apuntan fuera del espacio de direcciones accesible.
- `EIO`: Error de entrada/salida.
- `ELOOP`: Demasiados enlaces simbólicos al resolver `pathname`.
- `ENAMETOOLONG`: `pathname` es demasiado largo.
- `ENOENT`: El archivo no existe o `pathname` es un puntero nulo.
- `ENOEXEC`: El archivo tiene un formato ejecutable inválido.
- `ENOMEM`: No hay suficiente memoria disponible.
- `ENOTDIR`: Un componente de `pathname` no es un directorio.

---

### 🧭 Información adicional

<summary>📎 ¿Por qué se usa en Minishell?</summary>

En Minishell, `execve()` es fundamental para ejecutar programas externos. Algunos aspectos específicos de su uso incluyen:

1. **Ejecución de comandos externos**:
   Cada vez que el usuario introduce un comando que no es un builtin (como `cd`, `echo`, etc.), se utiliza `execve()` para ejecutarlo:

```c
char *find_command_path(char *cmd, char **env)
{
    char *path_env;
    char **paths;
    char *full_path;
    int i;
    
    // Si el comando incluye una ruta, no buscar en PATH
    if (cmd[0] == '/' || cmd[0] == '.')
    {
        if (access(cmd, F_OK | X_OK) == 0)
            return (ft_strdup(cmd));
        return (NULL);
    }
    
    // Obtener la variable PATH del entorno
    path_env = get_env_value("PATH", env);
    if (!path_env)
        return (NULL);
        
    // Dividir PATH en directorios individuales
    paths = ft_split(path_env, ':');
    
    // Buscar el comando en cada directorio
    i = 0;
    while (paths[i])
    {
        full_path = ft_strjoin3(paths[i], "/", cmd);
        if (access(full_path, F_OK | X_OK) == 0)
        {
            free_array(paths);
            return (full_path);
        }
        free(full_path);
        i++;
    }
    
    free_array(paths);
    return (NULL);  // Comando no encontrado
}
```

2. **Manejo de pipes y redirecciones**:
   Antes de llamar a `execve()`, es común configurar los descriptores de archivo para implementar tuberías y redirecciones:

```c
void setup_pipe(int pipefd[2], int is_input)
{
    if (is_input)
    {
        close(pipefd[1]);  // Cerrar extremo de escritura
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
    }
    else
    {
        close(pipefd[0]);  // Cerrar extremo de lectura
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
    }
}
```

3. **Propagación de entorno**:
   El shell debe propagar un entorno adecuado a los programas que ejecuta:

```c
char **create_env_array(t_env *env_list)
{
    int count = env_list_size(env_list);
    char **env_array = malloc((count + 1) * sizeof(char *));
    int i = 0;
    
    while (env_list)
    {
        env_array[i++] = ft_strjoin3(env_list->key, "=", env_list->value);
        env_list = env_list->next;
    }
    env_array[i] = NULL;
    
    return (env_array);
}
```

La combinación de `fork()`, `execve()` y `waitpid()` es lo que permite a un shell ejecutar comandos sin terminar su propia ejecución.

---