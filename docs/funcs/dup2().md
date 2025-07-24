## 🧩 `dup2()`  
<small><code>#include &lt;unistd.h&gt;</code></small>

---

### 🧾 Header
```c
#include <unistd.h>
```

---

### 🧪 Prototipo
```c
int dup2(int oldfd, int newfd);
```

---

### 📚 Descripción
Duplica un descriptor de archivo existente (`oldfd`) para que apunte al mismo archivo o recurso que `newfd`. Si `newfd` ya está abierto, primero se cierra automáticamente. Después de `dup2()`, ambos descriptores pueden usarse indistintamente, ya que apuntan al mismo recurso con las mismas propiedades (modo de apertura, posición del cursor, etc.).

---

### 🧰 Sintaxis y ejemplo
```c
int fd = open("archivo.txt", O_RDONLY);  // Abre un archivo
dup2(fd, STDIN_FILENO);                  // Redirige la entrada estándar al archivo
```


<summary>🔍 Ejemplo de uso en contexto (Redirección de entrada/salida)</summary>

```c
// Redirige la salida de un comando a un archivo
int fd_out = open("salida.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
if (fd_out < 0)
    return (error_handler("open error"));

// Guarda el stdout original para restaurarlo después
int saved_stdout = dup(STDOUT_FILENO);

// Redirige stdout al archivo
dup2(fd_out, STDOUT_FILENO);
close(fd_out);

// El comando ahora escribirá en el archivo en lugar de en la terminal
execute_command();

// Restaura stdout a su valor original
dup2(saved_stdout, STDOUT_FILENO);
close(saved_stdout);
```



---

### 💡 Consejos de uso
- **Fundamental para implementar redirecciones** en un shell (<, >, >>, etc.).
- Siempre **verifica el valor de retorno** para asegurarte de que la operación fue exitosa.
- Es común **guardar el descriptor original** antes de redirigirlo, para poder restaurarlo después.
- Recuerda **cerrar los descriptores** que ya no necesites para evitar fugas de recursos.

---

### 🔙 Retorno
Devuelve el nuevo descriptor de archivo (`newfd`) si tiene éxito.  
En caso de error, devuelve `-1` y establece `errno` con el código de error correspondiente.

---

### ⚠️ Errores
- `EBADF`: `oldfd` no es un descriptor de archivo válido y abierto.
- `EINVAL`: `oldfd` es negativo o el descriptor es inválido.
- `EMFILE`: Se ha alcanzado el límite del proceso para descriptores de archivo abiertos.

---

### 🧭 Información adicional

<summary>📎 ¿Por qué se usa en Minishell?</summary>

En Minishell, `dup2()` es esencial para implementar:

1. **Redirecciones de entrada/salida**: Cuando el usuario escribe comandos como `ls > archivo.txt` o `cat < entrada.txt`.

2. **Tuberías (pipes)**: Para conectar la salida de un comando con la entrada del siguiente en expresiones como `cmd1 | cmd2`.

Ejemplo de implementación de redirección de salida:

```c
void redirect_output(char *filename)
{
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
    {
        perror("minishell: open");
        exit(EXIT_FAILURE);
    }
    
    // Redirige stdout al archivo
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        perror("minishell: dup2");
        exit(EXIT_FAILURE);
    }
    close(fd);  // Ya no necesitamos este descriptor
}
```

Esta función sería llamada antes de ejecutar un comando cuya salida necesita ser redirigida a un archivo.



---