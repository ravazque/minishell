## 🧩 `read()`  
<small><code>#include &lt;unistd.h&gt;</code></small>

---

### 🧾 Header
```c
#include <unistd.h>
```

---

### 🧪 Prototipo
```c
ssize_t read(int fd, void *buf, size_t count);
```

---

### 📚 Descripción
Lee hasta `count` bytes desde un descriptor de archivo `fd` en el buffer apuntado por `buf`. El descriptor debe estar abierto para lectura.

La función intenta leer la cantidad solicitada de bytes, pero puede devolver menos si hay menos datos disponibles o si se alcanza el final del archivo.

---

### 🧰 Sintaxis y ejemplo
```c
char buffer[1024];
ssize_t bytes_read = read(fd, buffer, sizeof(buffer));
```

<summary>🔍 Ejemplo de uso en contexto (Lectura de un archivo completo)</summary>

```c
char	*read_file(const char *filename)
{
    int		fd;
    char	buffer[1024];
    char	*content = NULL;
    char	*temp;
    ssize_t	bytes_read;
    size_t	total_size = 0;
    
    fd = open(filename, O_RDONLY);
    if (fd == -1)
        return (NULL);
    
    while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0)
    {
        buffer[bytes_read] = '\0';
        
        // Reservar/ampliar espacio para el contenido
        temp = malloc(total_size + bytes_read + 1);
        if (!temp)
        {
            free(content);
            close(fd);
            return (NULL);
        }
        
        // Copiar contenido anterior + nuevo
        if (content)
        {
            memcpy(temp, content, total_size);
            free(content);
        }
        memcpy(temp + total_size, buffer, bytes_read + 1);
        
        content = temp;
        total_size += bytes_read;
    }
    
    close(fd);
    
    if (bytes_read == -1)
    {
        free(content);
        return (NULL);
    }
    
    return (content);
}
```

---

### 💡 Consejos de uso
- **No asume terminación nula**: `read()` no añade un carácter nulo (`'\0'`) al final del buffer. Si estás leyendo texto, debes añadirlo manualmente.
- **Puede devolver menos bytes** de los solicitados, especialmente cuando se lee de terminales o pipes.
- Para archivos, una lectura de 0 bytes indica **fin de archivo** (EOF).
- Para **lectura bloqueante** (por defecto), la función espera hasta que haya datos disponibles.
- Para **lectura no bloqueante**, el descriptor debe abrirse con `O_NONBLOCK`, y `read()` devolverá inmediatamente con `-1` y `errno` establecido a `EAGAIN` si no hay datos.
- Es buena práctica comprobar errores después de cada llamada a `read()`.

---

### 🔙 Retorno
Devuelve el **número de bytes leídos** (mayor o igual a 0).  
Devuelve `0` al alcanzar el fin de archivo.  
En caso de error, devuelve `-1` y establece `errno`.

---

### ⚠️ Errores
- `EAGAIN`: No hay datos disponibles y el descriptor es no bloqueante.
- `EBADF`: El descriptor de archivo no es válido o no está abierto para lectura.
- `EFAULT`: El buffer apunta fuera del espacio de direcciones accesible.
- `EINTR`: La llamada fue interrumpida por una señal antes de leer datos.
- `EIO`: Error de E/S.
- `EISDIR`: El descriptor de archivo se refiere a un directorio.

---

### 🧭 Información adicional

<summary>📎 ¿Por qué se usa en Minishell?</summary>

En Minishell, `read()` se utiliza en múltiples contextos:

1. **Lectura de pipes**: Para comunicación entre procesos en comandos con pipes (`|`).

```c
void execute_piped_commands(t_command *cmd1, t_command *cmd2)
{
    int     pipe_fd[2];
    pid_t   pid1, pid2;
    
    if (pipe(pipe_fd) == -1)
        return (perror("minishell: pipe"));
    
    // Primer proceso escribe en pipe_fd[1]
    // ...
    
    // Segundo proceso lee de pipe_fd[0]
    pid2 = fork();
    if (pid2 == 0)
    {
        close(pipe_fd[1]);
        dup2(pipe_fd[0], STDIN_FILENO);
        close(pipe_fd[0]);
        
        // Ahora read() en stdin leerá del pipe
        execute_command(cmd2);
        exit(EXIT_SUCCESS);
    }
    
    // ...
}
```

2. **Implementación de heredoc**: Leyendo entrada hasta encontrar un delimitador.

```c
char *read_until_delimiter(int fd, char *delimiter)
{
    char    buffer[1024];
    ssize_t bytes_read;
    char    *result = NULL;
    size_t  total = 0;
    
    while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0)
    {
        buffer[bytes_read] = '\0';
        
        // Procesamiento del buffer buscando el delimitador
        // ...
        
        // Si se encuentra el delimitador, terminar
        if (strstr(buffer, delimiter))
            break;
            
        // Añadir buffer a result...
    }
    
    return (result);
}
```

3. **Lectura de archivos de configuración**: Para cargar archivos como `.minishellrc`.

4. **Redirecciones de entrada**: Para implementar el operador `<` que redirige la entrada estándar desde un archivo.

Es fundamental usar `read()` correctamente, comprobando errores y manejando adecuadamente el fin de archivo y las lecturas parciales para garantizar la robustez del shell.

---