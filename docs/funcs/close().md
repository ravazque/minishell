## 🧩 `close()`  
<small><code>#include &lt;unistd.h&gt;</code></small>

---

### 🧾 Header
```c
#include <unistd.h>
```

---

### 🧪 Prototipo
```c
int close(int fd);
```

---

### 📚 Descripción
Cierra un **descriptor de archivo**, liberando los recursos asociados y haciendo que el descriptor no esté disponible para futuras llamadas al sistema.

Es una operación fundamental de mantenimiento de recursos, ya que cada proceso tiene un límite en el número de descriptores de archivo que puede tener abiertos simultáneamente.

---

### 🧰 Sintaxis y ejemplo
```c
int fd = open("archivo.txt", O_RDONLY);
// Operaciones con el archivo...
close(fd);  // Cierra el descriptor cuando ya no es necesario
```

<summary>🔍 Ejemplo de uso en contexto (Redirecciones)</summary>

```c
int	redirect_output(char *filename, int append)
{
    int	fd;
    int	stdout_copy;
    
    // Guarda una copia del stdout original para restaurarlo después
    stdout_copy = dup(STDOUT_FILENO);
    
    // Abre el archivo con los flags adecuados según sea append o no
    if (append)
        fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    else
        fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    
    if (fd == -1)
    {
        perror("minishell");
        return (-1);
    }
    
    // Redirige stdout al archivo
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        perror("minishell");
        close(fd);
        return (-1);
    }
    
    // Cierra el descriptor original ya que ya no es necesario
    close(fd);
    
    return (stdout_copy);  // Para restaurarlo después
}
```

---

### 💡 Consejos de uso
- **Siempre cierra** los descriptores cuando ya no son necesarios para evitar agotar recursos.
- Un **descriptor cerrado** no puede ser utilizado en operaciones posteriores de E/S.
- Cuando un proceso **termina**, el sistema operativo cierra automáticamente todos sus descriptores, pero es mejor práctica cerrarlos explícitamente.
- Si varios descriptores apuntan al mismo archivo abierto (por ejemplo, después de `dup()`), el archivo no se cierra realmente hasta que todos los descriptores asociados se hayan cerrado.
- Es especialmente importante cerrar los descriptores en código que maneja **señales o errores** para evitar fugas de recursos.

---

### 🔙 Retorno
Devuelve `0` si la operación fue exitosa.  
En caso de error, devuelve `-1` y establece `errno`.

---

### ⚠️ Errores
- `EBADF`: El descriptor de archivo no es válido.
- `EINTR`: La llamada fue interrumpida por una señal.
- `EIO`: Error de E/S.

---

### 🧭 Información adicional

<summary>📎 ¿Por qué se usa en Minishell?</summary>

En Minishell, `close()` es esencial para un manejo adecuado de los recursos. Se utiliza principalmente en:

1. **Redirecciones y pipes**: Después de redirigir entrada/salida con `dup2()`, los descriptores originales deben cerrarse.

```c
// En una redirección de entrada (<)
int fd = open(filename, O_RDONLY);
dup2(fd, STDIN_FILENO);  // STDIN ahora lee del archivo
close(fd);               // El descriptor original ya no es necesario
```

2. **Manejo de pipes**: Al crear un pipe con la función `pipe()`, se obtienen dos descriptores. Los extremos no utilizados deben cerrarse en cada proceso.

```c
int pipe_fd[2];
pipe(pipe_fd);  // pipe_fd[0] para lectura, pipe_fd[1] para escritura

if (fork() == 0)
{
    // Proceso hijo (lee del pipe)
    close(pipe_fd[1]);  // Cierra extremo de escritura que no usa
    dup2(pipe_fd[0], STDIN_FILENO);
    close(pipe_fd[0]);  // Ya no necesita el descriptor original
    
    // Ejecuta comando que lee de stdin...
}
else
{
    // Proceso padre (escribe en el pipe)
    close(pipe_fd[0]);  // Cierra extremo de lectura que no usa
    dup2(pipe_fd[1], STDOUT_FILENO);
    close(pipe_fd[1]);  // Ya no necesita el descriptor original
    
    // Ejecuta comando que escribe a stdout...
}
```

3. **Limpieza de recursos**: Al finalizar el shell o cuando ocurre un error, se deben cerrar todos los descriptores abiertos.

```c
void cleanup_resources(t_shell *shell)
{
    // Cierra descriptores abiertos
    if (shell->input_fd != STDIN_FILENO)
        close(shell->input_fd);
    if (shell->output_fd != STDOUT_FILENO)
        close(shell->output_fd);
        
    // Libera memoria, etc...
}
```

No cerrar los descriptores adecuadamente puede provocar fugas de recursos, comportamiento indefinido en redirecciones, y en el peor caso, alcanzar el límite de descriptores abiertos del sistema, lo que impediría abrir nuevos archivos.

---