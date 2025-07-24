## 🧩 `fstat()`  
<small><code>#include &lt;sys/stat.h&gt;</code></small>

---

### 🧾 Header
```c
#include <sys/stat.h>
#include <unistd.h>
```

---

### 🧪 Prototipo
```c
int fstat(int fd, struct stat *statbuf);
```

---

### 📚 Descripción
La función `fstat()` **obtiene información** sobre un archivo abierto a partir de su **descriptor de archivo** (`fd`). Es similar a `stat()` y `lstat()`, pero en lugar de utilizar una ruta de archivo, trabaja con un descriptor de archivo ya abierto.

La ventaja principal de `fstat()` es que funciona incluso con archivos que han sido abiertos pero posteriormente renombrados o eliminados del sistema de archivos. También evita las condiciones de carrera que pueden ocurrir entre verificar un archivo con `stat()` y luego abrirlo.

---

### 🧰 Sintaxis y ejemplo
```c
int fd = open("/path/to/file", O_RDONLY);
struct stat file_info;

if (fd != -1)
{
    if (fstat(fd, &file_info) == 0)
    {
        printf("Tamaño: %ld bytes\n", file_info.st_size);
        printf("Permisos: %o\n", file_info.st_mode & 0777);
    }
    else
        perror("fstat error");
        
    close(fd);
}
```


<summary>🔍 Ejemplo de uso en contexto (Verificación de tipo de archivo)</summary>

```c
int is_regular_and_readable(int fd)
{
    struct stat file_stat;
    
    // Obtener información del archivo usando su descriptor
    if (fstat(fd, &file_stat) == -1)
    {
        perror("fstat");
        return 0;
    }
    
    // Verificar si es un archivo regular
    if (!S_ISREG(file_stat.st_mode))
    {
        fprintf(stderr, "Error: No es un archivo regular\n");
        return 0;
    }
    
    // Verificar permisos de lectura (simplificado)
    if (!(file_stat.st_mode & S_IRUSR))
    {
        fprintf(stderr, "Error: No se tienen permisos de lectura\n");
        return 0;
    }
    
    return 1;
}
```



---

### 💡 Consejos de uso
- **Uso con archivos ya abiertos**: A diferencia de `stat()` y `lstat()`, que toman rutas, `fstat()` es ideal para archivos que ya están abiertos.
- **Acceso a información completa**: La estructura `stat` proporciona información detallada como:
  - `st_dev`: ID del dispositivo que contiene el archivo
  - `st_ino`: Número de inodo del archivo
  - `st_mode`: Tipo de archivo y permisos
  - `st_nlink`: Número de enlaces duros al archivo
  - `st_uid`, `st_gid`: ID de usuario y grupo propietarios
  - `st_size`: Tamaño en bytes
  - `st_blocks`: Número de bloques asignados
  - `st_atime`, `st_mtime`, `st_ctime`: Tiempos de acceso, modificación y cambio de estado
- **Verificación de descriptores especiales**: Útil para determinar si un descriptor de archivo apunta a un archivo regular, un socket, una pipe, etc.
- **Se puede usar con stdin/stdout/stderr**: Los descriptores estándar (0, 1, 2) también pueden ser usados con `fstat()`.

---

### 🔙 Retorno
Devuelve `0` en caso de éxito.  
En caso de error, devuelve `-1` y establece la variable `errno`.

---

### ⚠️ Errores
- `EBADF`: El descriptor de archivo `fd` no es válido.
- `EFAULT`: El puntero `statbuf` apunta a una dirección de memoria inválida.
- `ENOMEM`: Memoria insuficiente para completar la operación.
- `EIO`: Error de entrada/salida.

---

### 🧭 Información adicional

<summary>📎 ¿Por qué se usa en Minishell?</summary>

En Minishell, `fstat()` puede ser útil para varias funcionalidades importantes:

1. **Verificación de redirecciones**: Cuando los usuarios redirigen entrada/salida con `<`, `>` o `>>`, el shell debe verificar que los descriptores están correctamente configurados:

```c
int validate_fd_for_redirection(int fd, int for_input)
{
    struct stat fd_stat;
    
    if (fstat(fd, &fd_stat) == -1)
    {
        perror("fstat");
        return 0;
    }
    
    // Para entrada, verificar que sea un archivo regular o FIFO
    if (for_input && !S_ISREG(fd_stat.st_mode) && !S_ISFIFO(fd_stat.st_mode))
    {
        fprintf(stderr, "Error: La entrada debe ser un archivo regular o una tubería\n");
        return 0;
    }
    
    // Para salida, verificar que sea un archivo regular o un dispositivo
    if (!for_input && !S_ISREG(fd_stat.st_mode) && !S_ISCHR(fd_stat.st_mode))
    {
        fprintf(stderr, "Error: La salida debe ser un archivo regular o un dispositivo\n");
        return 0;
    }
    
    return 1;
}
```

2. **Implementación de heredocs**: Al usar redirecciones de tipo "heredoc" (`<<`), es útil verificar el tipo de descriptor temporal creado:

```c
int setup_heredoc(const char *delimiter)
{
    int pipe_fds[2];
    
    // Crear una tubería
    if (pipe(pipe_fds) == -1)
    {
        perror("pipe");
        return -1;
    }
    
    // Leer líneas de entrada hasta encontrar el delimitador
    char *line;
    while ((line = readline("> ")) != NULL)
    {
        if (strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        
        write(pipe_fds[1], line, strlen(line));
        write(pipe_fds[1], "\n", 1);
        free(line);
    }
    
    // Cerrar el extremo de escritura
    close(pipe_fds[1]);
    
    // Verificar que la tubería esté configurada correctamente
    struct stat pipe_stat;
    if (fstat(pipe_fds[0], &pipe_stat) == -1 || !S_ISFIFO(pipe_stat.st_mode))
    {
        fprintf(stderr, "Error al configurar heredoc\n");
        close(pipe_fds[0]);
        return -1;
    }
    
    return pipe_fds[0];  // Devolver el extremo de lectura
}
```

3. **Detección de tipos de descriptores estándar**: A veces es necesario saber si stdin/stdout/stderr están conectados a una terminal o a un archivo:

```c
int is_terminal_input(void)
{
    struct stat stdin_stat;
    
    if (fstat(STDIN_FILENO, &stdin_stat) == -1)
        return 0;
    
    // S_ISCHR verifica si es un device de caracteres (como una terminal)
    return S_ISCHR(stdin_stat.st_mode);
}

// Esta función puede usarse para cambiar el comportamiento del prompt:
void display_prompt(void)
{
    if (is_terminal_input())
        printf("minishell$ ");  // Solo muestra el prompt en modo interactivo
}
```

4. **Optimización de operaciones de archivo**: Al trabajar con archivos grandes, `fstat()` permite conocer el tamaño exacto antes de leer:

```c
char *read_entire_file(int fd)
{
    struct stat file_stat;
    char *buffer;
    
    // Obtener el tamaño del archivo
    if (fstat(fd, &file_stat) == -1)
        return NULL;
    
    // Asignar memoria exacta (más un byte para el terminador nulo)
    buffer = malloc(file_stat.st_size + 1);
    if (!buffer)
        return NULL;
    
    // Leer todo el archivo de una vez
    if (read(fd, buffer, file_stat.st_size) != file_stat.st_size)
    {
        free(buffer);
        return NULL;
    }
    
    buffer[file_stat.st_size] = '\0';
    return buffer;
}
```

Estas aplicaciones hacen que `fstat()` sea una herramienta valiosa para la implementación correcta y robusta de un shell.