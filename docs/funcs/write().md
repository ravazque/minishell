## 🧩 `write()`  
<small><code>#include &lt;unistd.h&gt;</code></small>

---

### 🧾 Header
```c
#include <unistd.h>
```

---

### 🧪 Prototipo
```c
ssize_t write(int fd, const void *buf, size_t count);
```

---

### 📚 Descripción
Escribe datos desde un buffer a un descriptor de archivo. Esta función **intenta escribir `count` bytes** desde el buffer apuntado por `buf` al archivo asociado al descriptor de archivo `fd`. El contador de posición del archivo aumenta según el número de bytes escritos.

---

### 🧰 Sintaxis y ejemplo
```c
write(1, "Hello, World!\n", 14);  // Escribe a stdout (1)
```

<summary>🔍 Ejemplo de uso en contexto (Mensaje de error en Minishell)</summary>

```c
void print_error(char *cmd, char *error_msg)
{
    write(2, "minishell: ", 11);
    write(2, cmd, ft_strlen(cmd));
    write(2, ": ", 2);
    write(2, error_msg, ft_strlen(error_msg));
    write(2, "\n", 1);
}
```

---

### 💡 Consejos de uso
- El descriptor `1` corresponde a la **salida estándar** (stdout), `2` a la **salida de error** (stderr) y `0` a la **entrada estándar** (stdin).
- `write()` **no agrega automáticamente un carácter de nueva línea** al final, debes incluirlo explícitamente si lo necesitas.
- Para los descriptores de archivo que refieren a archivos regulares, la operación de escritura comienza en la posición actual del archivo y desplaza este puntero por la cantidad de bytes escritos.
- No asumas que `write()` siempre escribe todos los bytes solicitados; verifica el valor de retorno.

---

### 🔙 Retorno
Devuelve el **número de bytes escritos** si la operación fue exitosa.  
En caso de error, devuelve `-1` y establece `errno` adecuadamente.

---

### ⚠️ Errores
- `EBADF`: El descriptor de archivo no es válido o no está abierto para escritura.
- `EFAULT`: El buffer apunta fuera del espacio de direcciones accesible.
- `EINTR`: La llamada fue interrumpida por una señal antes de que se escribiera algún dato.
- `EINVAL`: El descriptor de archivo está asociado a un objeto que no permite escritura.
- `EIO`: Error de entrada/salida.
- `ENOSPC`: No hay espacio suficiente en el dispositivo para la escritura.
- `EPIPE`: El descriptor de archivo está conectado a una tubería o socket cuyo otro extremo está cerrado.

---

### 🧭 Información adicional

<summary>📎 ¿Por qué se usa en Minishell?</summary>

En Minishell, `write()` se utiliza para diferentes propósitos:

1. **Impresión de mensajes de error** cuando fallan los comandos:
```c
if (execve(path, args, env) == -1)
{
    write(2, "minishell: ", 11);
    write(2, args[0], ft_strlen(args[0]));
    write(2, ": ", 2);
    write(2, strerror(errno), ft_strlen(strerror(errno)));
    write(2, "\n", 1);
}
```

2. **Manejo de señales**, especialmente para limpiar la línea y mostrar un nuevo prompt:
```c
void handle_signal(int sig)
{
    if (sig == SIGINT)  // Ctrl+C
    {
        write(1, "\n", 1);  // Nueva línea
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
}
```

3. **Implementación de builtins** como `echo`:
```c
void ft_echo(char **args)
{
    int i;
    int newline;
    
    newline = 1;
    i = 1;
    
    if (args[1] && ft_strcmp(args[1], "-n") == 0)
    {
        newline = 0;
        i++;
    }
    
    while (args[i])
    {
        write(1, args[i], ft_strlen(args[i]));
        if (args[i + 1])
            write(1, " ", 1);
        i++;
    }
    
    if (newline)
        write(1, "\n", 1);
}
```

`write()` es preferido sobre `printf()` en situaciones donde se necesita control preciso sobre lo que se escribe y a qué descriptor de archivo, especialmente al manejar señales y errores.

---