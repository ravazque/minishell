## 🧩 `open()`  
<small><code>#include &lt;fcntl.h&gt;</code></small>

---

### 🧾 Header
```c
#include <fcntl.h>
```

---

### 🧪 Prototipo
```c
int open(const char *pathname, int flags);
int open(const char *pathname, int flags, mode_t mode);
```

---

### 📚 Descripción
Abre un **archivo o dispositivo** y devuelve un descriptor de archivo que puede usarse en llamadas posteriores al sistema como `read()`, `write()` o `close()`.

El parámetro `flags` determina el modo de acceso (lectura, escritura, etc.) y otras opciones. Si se crea un nuevo archivo, `mode` especifica los permisos que tendrá.

---

### 🧰 Sintaxis y ejemplo
```c
// Abrir para lectura
int fd = open("archivo.txt", O_RDONLY);

// Abrir para escritura, creándolo si no existe
int fd = open("nuevo.txt", O_WRONLY | O_CREAT, 0644);
```

<summary>🔍 Ejemplo de uso en contexto (Redirecciones en Minishell)</summary>

```c
int	redirect_input(char *filename)
{
    int	fd;
    int	stdin_copy;
    
    // Guarda una copia del stdin original
    stdin_copy = dup(STDIN_FILENO);
    
    // Abre el archivo para lectura
    fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        perror("minishell");
        return (-1);
    }
    
    // Redirige stdin para que lea del archivo
    if (dup2(fd, STDIN_FILENO) == -1)
    {
        perror("minishell");
        close(fd);
        return (-1);
    }
    
    // Cierra el descriptor original ya que ya no es necesario
    close(fd);
    
    return (stdin_copy);  // Devuelve el descriptor original para restaurarlo después
}
```

---

### 💡 Consejos de uso
- Los **flags más comunes** son:
  - `O_RDONLY`: Abre para lectura.
  - `O_WRONLY`: Abre para escritura.
  - `O_RDWR`: Abre para lectura y escritura.
  - `O_CREAT`: Crea el archivo si no existe.
  - `O_APPEND`: Los datos se escriben al final del archivo.
  - `O_TRUNC`: Trunca el archivo a longitud cero si existe.
- Cuando se usa `O_CREAT`, el tercer parámetro `mode` es **obligatorio** para especificar los permisos.
- Los permisos se especifican en octal: `0644` significa `-rw-r--r--`.
- Cada proceso tiene un **límite de descriptores de archivo** abiertos simultáneamente.
- Siempre se debe cerrar el descriptor con `close()` cuando ya no se necesite.

---

### 🔙 Retorno
Devuelve un **entero no negativo** que es el descriptor de archivo.  
En caso de error, devuelve `-1` y establece `errno`.

---

### ⚠️ Errores
- `EACCES`: Permiso denegado.
- `EEXIST`: El archivo existe y se especificó O_CREAT y O_EXCL.
- `EINTR`: La llamada fue interrumpida por una señal.
- `EISDIR`: El archivo es un directorio y se intentó abrir para escritura.
- `EMFILE`: Se alcanzó el límite de archivos abiertos para el proceso.
- `ENFILE`: Se alcanzó el límite de archivos abiertos para el sistema.
- `ENOENT`: El archivo no existe y no se especificó O_CREAT.
- `EROFS`: El sistema de archivos es de solo lectura.

---

### 🧭 Información adicional

<summary>📎 ¿Por qué se usa en Minishell?</summary>

En Minishell, `open()` es fundamental para implementar:

1. **Redirecciones de entrada/salida**: Permite redirigir la entrada estándar (`<`), la salida estándar (`>`), y la salida para anexar (`>>`).

```c
// Redirección de salida (>)
int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
dup2(fd, STDOUT_FILENO);
close(fd);

// Redirección para anexar (>>)
int fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
dup2(fd, STDOUT_FILENO);
close(fd);

// Redirección de entrada (<)
int fd = open(filename, O_RDONLY);
dup2(fd, STDIN_FILENO);
close(fd);
```

2. **Heredocs**: Para implementar redirecciones de documento heredado (`<<`).

```c
void	handle_heredoc(char *delimiter)
{
    int		pipe_fd[2];
    char	*line;
    
    pipe(pipe_fd);
    
    while (1)
    {
        line = readline("> ");
        if (!line || !strcmp(line, delimiter))
        {
            free(line);
            break;
        }
        
        write(pipe_fd[1], line, strlen(line));
        write(pipe_fd[1], "\n", 1);
        free(line);
    }
    
    close(pipe_fd[1]);
    dup2(pipe_fd[0], STDIN_FILENO);
    close(pipe_fd[0]);
}
```

3. **Ejecución de comandos con archivos**: Para leer configuraciones, scripts o para implementar comandos como `source`.

La gestión adecuada de los descriptores de archivo es esencial para evitar fugas de recursos y asegurar un comportamiento correcto del shell con redirecciones y pipes.

---