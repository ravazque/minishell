## 🧩 `perror()`  
<small><code>#include &lt;stdio.h&gt;</code></small>

---

### 🧾 Header
```c
#include <stdio.h>
```

---

### 🧪 Prototipo
```c
void perror(const char *s);
```

---

### 📚 Descripción
Imprime un mensaje de error descriptivo en el flujo de error estándar (`stderr`). Imprime primero la cadena `s` seguida de dos puntos y un espacio, y luego un mensaje de error correspondiente al valor actual de la variable global `errno`.

Es muy útil para mostrar descripciones legibles de errores del sistema después de que una llamada al sistema ha fallado.

---

### 🧰 Sintaxis y ejemplo
```c
FILE *file = fopen("archivo_inexistente.txt", "r");
if (file == NULL)
{
    perror("Error al abrir archivo");  // Muestra: "Error al abrir archivo: No such file or directory"
}
```

<summary>🔍 Ejemplo de uso en contexto (Manejo de errores en comandos del shell)</summary>

```c
int execute_command(const char *cmd, char **args)
{
    pid_t pid = fork();
    
    if (pid < 0)
    {
        perror("fork");  // Muestra: "fork: Resource temporarily unavailable"
        return (1);
    }
    else if (pid == 0)
    {
        // Proceso hijo
        if (execvp(cmd, args) == -1)
        {
            perror(cmd);  // Muestra: "[comando]: command not found"
            exit(EXIT_FAILURE);
        }
    }
    // Proceso padre
    wait(NULL);
    return (0);
}
```

---

### 💡 Consejos de uso
- **Proporciona mensajes contextuales** como primer argumento para que el error sea más fácil de entender.
- Usa `perror()` inmediatamente después de una llamada al sistema que falla, ya que otras llamadas podrían modificar `errno`.
- Si `s` es una cadena vacía (`""`), solo se imprime el mensaje de error sin el prefijo de dos puntos.
- Si `s` es `NULL`, no se imprime ningún prefijo, solo el mensaje de error.

---

### 🔙 Retorno
No devuelve ningún valor (tipo `void`).

---

### ⚠️ Errores
- No hay errores específicos para `perror()` ya que simplemente imprime información.
- Si `errno` no corresponde a un error conocido, el mensaje puede ser genérico o impreciso.

---

### 🧭 Información adicional

<summary>📎 ¿Por qué se usa en Minishell?</summary>

En Minishell, `perror()` es esencial para proporcionar retroalimentación útil al usuario cuando ocurren errores del sistema. Es especialmente útil para:

1. **Comandos que fallan**: Cuando una llamada a `execve` falla, `perror()` puede mostrar por qué.
2. **Operaciones de archivo**: Al abrir, leer o escribir archivos.
3. **Operaciones de proceso**: Al crear procesos con `fork()` o controlarlos.

Un shell debe proporcionar mensajes de error informativos, y `perror()` facilita esto mostrando automáticamente la causa del error en un formato estándar.

Ejemplo de uso en el manejo de redirecciones:

```c
int redirect_input(const char *filename)
{
    int fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
        perror(filename);  // Muestra: "archivo.txt: No such file or directory"
        return (1);
    }
    
    if (dup2(fd, STDIN_FILENO) < 0)
    {
        perror("dup2");
        close(fd);
        return (1);
    }
    
    close(fd);
    return (0);
}
```

En este ejemplo, si el archivo no existe, `perror()` mostrará un mensaje claro indicando el problema con el nombre del archivo específico, lo que ayuda al usuario a entender qué salió mal.

---