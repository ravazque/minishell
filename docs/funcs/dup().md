## 🧩 `dup()`  
<small><code>#include &lt;unistd.h&gt;</code></small>

---

### 🧾 Header
```c
#include <unistd.h>
```

---

### 🧪 Prototipo
```c
int dup(int oldfd);
```

---

### 📚 Descripción
La función `dup()` **crea una copia** (duplica) del descriptor de archivo especificado por `oldfd`. El nuevo descriptor retornado utiliza el número disponible más bajo en la tabla de descriptores de archivos del proceso.

El descriptor original (`oldfd`) y el duplicado (`newfd`) comparten la misma posición de lectura/escritura y el mismo conjunto de flags. Esto significa que si se cambia la posición del archivo mediante `lseek()` en uno de ellos, también se actualiza en el otro.

Ambos descriptores son independientes en cuanto a su ciclo de vida (se pueden cerrar por separado) pero apuntan al mismo archivo o recurso.

---

### 🧰 Sintaxis y ejemplo
```c
int fd = open("file.txt", O_RDWR);
int new_fd;

if (fd != -1)
{
    new_fd = dup(fd);
    if (new_fd != -1)
        printf("Descriptor duplicado: %d -> %d\n", fd, new_fd);
    else
        perror("dup error");
}
```


<summary>🔍 Ejemplo de uso en contexto (Redireccionamiento de salida)</summary>

```c
void redirect_stdout_to_file(const char *filename)
{
    int fd;
    int stdout_copy;
    
    // Primero guardamos una copia del descriptor de stdout
    stdout_copy = dup(STDOUT_FILENO);
    if (stdout_copy == -1)
    {
        perror("dup");
        return;
    }
    
    // Abrimos el archivo de destino
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("open");
        close(stdout_copy);
        return;
    }
    
    // Redirigimos stdout al archivo
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        perror("dup2");
        close(fd);
        close(stdout_copy);
        return;
    }
    
    // Ya no necesitamos el descriptor original del archivo
    close(fd);
    
    // Ahora cualquier printf() escribirá al archivo
    printf("Esta línea va al archivo, no a la terminal\n");
    
    // Restauramos stdout a su estado original
    if (dup2(stdout_copy, STDOUT_FILENO) == -1)
        perror("dup2 restore");
    
    close(stdout_copy);
    
    // Esta línea irá a la terminal nuevamente
    printf("Esta línea va a la terminal\n");
}
```



---

### 💡 Consejos de uso
- **Guardar descriptores estándar**: Antes de redirigir stdin/stdout/stderr, guarda una copia con `dup()` para poder restaurarlos después.
- **Diferencia con `dup2()`**: `dup()` elige automáticamente el descriptor disponible de menor número, mientras que `dup2()` permite especificar el número de descriptor que se va a utilizar.
- **Cierre de descriptores**: Cada descriptor creado con `dup()` debe cerrarse independientemente con `close()` cuando ya no se necesite.
- **Heredar descriptores**: En un proceso hijo creado con `fork()`, los descriptores duplicados se heredan y mantienen sus propiedades.
- **Reiniciar descriptores**: Si necesitas "reiniciar" un descriptor para que apunte al principio del archivo, usa `lseek()` en cualquiera de los duplicados.

---

### 🔙 Retorno
Devuelve el **nuevo descriptor** de archivo en caso de éxito.  
En caso de error, devuelve `-1` y establece la variable `errno`.

---

### ⚠️ Errores
- `EBADF`: `oldfd` no es un descriptor válido y abierto.
- `EMFILE`: El proceso ya tiene el número máximo de descriptores de archivo abiertos o se ha alcanzado el límite del sistema.
- `EINTR`: La llamada fue interrumpida por una señal antes de que se completara.

---

### 🧭 Información adicional

<summary>📎 ¿Por qué se usa en Minishell?</summary>

En Minishell, `dup()` es una función crítica para implementar redireccionamientos y tuberías (pipes), que son características fundamentales de un shell:

1. **Implementación de redireccionamientos** (`>`, `<`, `>>`): Cuando un usuario escribe comandos como `ls > archivo.txt`, el shell debe redirigir la salida estándar al archivo:

```c
int handle_output_redirection(const char *file, int append_mode)
{
    int fd;
    int stdout_backup;
    
    // Guardar una copia del descriptor de salida estándar
    stdout_backup = dup(STDOUT_FILENO);
    if (stdout_backup == -1)
    {
        perror("dup");
        return -1;
    }
    
    // Abrir el archivo para escribir
    int flags = O_WRONLY | O_CREAT;
    flags |= (append_mode ? O_APPEND : O_TRUNC);
    
    fd = open(file, flags, 0644);
    if (fd == -1)
    {
        perror("open");
        close(stdout_backup);
        return -1;
    }
    
    // Redirigir stdout al archivo
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        perror("dup2");
        close(fd);
        close(stdout_backup);
        return -1;
    }
    
    close(fd);  // Ya no necesitamos este descriptor
    
    return stdout_backup;  // Devolvemos la copia para restaurar después
}

// Función para restaurar el descriptor estándar
void restore_stdout(int backup_fd)
{
    if (backup_fd >= 0)
    {
        dup2(backup_fd, STDOUT_FILENO);
        close(backup_fd);
    }
}
```

2. **Implementación de tuberías** (`|`): Para que un comando pueda enviar su salida como entrada de otro:

```c
int execute_pipe(char **cmd1_args, char **cmd2_args)
{
    int pipe_fds[2];
    pid_t pid1, pid2;
    
    // Crear una tubería
    if (pipe(pipe_fds) == -1)
    {
        perror("pipe");
        return EXIT_FAILURE;
    }
    
    // Crear el primer proceso hijo
    pid1 = fork();
    if (pid1 == -1)
    {
        perror("fork");
        close(pipe_fds[0]);
        close(pipe_fds[1]);
        return EXIT_FAILURE;
    }
    
    if (pid1 == 0)
    {
        // Proceso hijo 1 (productor)
        close(pipe_fds[0]);  // Cerrar el extremo de lectura no usado
        
        // Redirigir stdout a la tubería
        if (dup2(pipe_fds[1], STDOUT_FILENO) == -1)
        {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        
        close(pipe_fds[1]);  // Ya no necesitamos el descriptor original
        
        // Ejecutar el primer comando
        execvp(cmd1_args[0], cmd1_args);
        perror("execvp");
        exit(EXIT_FAILURE);
    }
    
    // Crear el segundo proceso hijo
    pid2 = fork();
    if (pid2 == -1)
    {
        perror("fork");
        close(pipe_fds[0]);
        close(pipe_fds[1]);
        return EXIT_FAILURE;
    }
    
    if (pid2 == 0)
    {
        // Proceso hijo 2 (consumidor)
        close(pipe_fds[1]);  // Cerrar el extremo de escritura no usado
        
        // Redirigir stdin desde la tubería
        if (dup2(pipe_fds[0], STDIN_FILENO) == -1)
        {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        
        close(pipe_fds[0]);  // Ya no necesitamos el descriptor original
        
        // Ejecutar el segundo comando
        execvp(cmd2_args[0], cmd2_args);
        perror("execvp");
        exit(EXIT_FAILURE);
    }
    
    // Proceso padre: cerrar ambos extremos de la tubería
    close(pipe_fds[0]);
    close(pipe_fds[1]);
    
    // Esperar a que ambos hijos terminen
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    
    return EXIT_SUCCESS;
}
```

3. **Restauración de descriptores estándar**: Después de ejecutar comandos con redireccionamiento, es necesario restaurar los descriptores estándar a su estado original:

```c
void execute_command_with_redirections(char **args, char *input_file, char *output_file)
{
    int stdin_backup = -1;
    int stdout_backup = -1;
    
    // Configurar redirección de entrada si es necesario
    if (input_file)
    {
        stdin_backup = dup(STDIN_FILENO);
        int fd = open(input_file, O_RDONLY);
        if (fd == -1)
        {
            perror(input_file);
            return;
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
    }
    
    // Configurar redirección de salida si es necesario
    if (output_file)
    {
        stdout_backup = dup(STDOUT_FILENO);
        int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1)
        {
            perror(output_file);
            if (stdin_backup != -1)
            {
                dup2(stdin_backup, STDIN_FILENO);
                close(stdin_backup);
            }
            return;
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
    
    // Ejecutar el comando
    execute_command(args);
    
    // Restaurar descriptores estándar
    if (stdin_backup != -1)
    {
        dup2(stdin_backup, STDIN_FILENO);
        close(stdin_backup);
    }
    
    if (stdout_backup != -1)
    {
        dup2(stdout_backup, STDOUT_FILENO);
        close(stdout_backup);
    }
}
```

La función `dup()` y su variante `dup2()` son fundamentales para el funcionamiento correcto de un shell que debe manipular la entrada y salida estándar para implementar las características esperadas por los usuarios.