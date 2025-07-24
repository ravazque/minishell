## 🧩 `strerror()`  
<small><code>#include &lt;string.h&gt;</code></small>

---

### 🧾 Header
```c
#include <string.h>
```

---

### 🧪 Prototipo
```c
char *strerror(int errnum);
```

---

### 📚 Descripción
Devuelve un puntero a una cadena de caracteres que describe el código de error especificado en `errnum`, que típicamente es el valor de la variable global `errno`. La cadena no debe ser modificada por el programa, pero puede ser sobrescrita por llamadas subsecuentes a `strerror()`.

A diferencia de `perror()`, que imprime automáticamente el mensaje de error, `strerror()` devuelve la cadena para que el programa la use como considere necesario.

---

### 🧰 Sintaxis y ejemplo
```c
int fd = open("archivo_inexistente.txt", O_RDONLY);
if (fd == -1)
{
    printf("Error: %s\n", strerror(errno));  // Muestra: "Error: No such file or directory"
}
```

<summary>🔍 Ejemplo de uso en contexto (Mensajes de error personalizados)</summary>

```c
int create_backup(const char *filename)
{
    char backup_name[256];
    int source_fd, dest_fd;
    
    snprintf(backup_name, sizeof(backup_name), "%s.bak", filename);
    
    source_fd = open(filename, O_RDONLY);
    if (source_fd == -1)
    {
        fprintf(stderr, "No se pudo abrir '%s' para backup: %s\n", 
                filename, strerror(errno));
        return (1);
    }
    
    dest_fd = open(backup_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd == -1)
    {
        fprintf(stderr, "No se pudo crear archivo backup '%s': %s\n", 
                backup_name, strerror(errno));
        close(source_fd);
        return (1);
    }
    
    // Código para copiar datos...
    
    close(source_fd);
    close(dest_fd);
    return (0);
}
```

---

### 💡 Consejos de uso
- Útil cuando necesitas **personalizar el formato** del mensaje de error, o guardarlo para uso posterior.
- La cadena devuelta por `strerror()` es estática y puede ser sobrescrita en la siguiente llamada a `strerror()`.
- Combina bien con funciones como `fprintf()` o `snprintf()` para crear mensajes de error detallados.
- Si necesitas mostrar el error inmediatamente en `stderr`, considera usar `perror()` en su lugar.

---

### 🔙 Retorno
Devuelve un puntero a una cadena de caracteres que contiene el mensaje de error correspondiente al código `errnum`.

---

### ⚠️ Errores
- Si `errnum` no corresponde a un código de error válido, el comportamiento es específico de la implementación. Algunas implementaciones pueden devolver un mensaje genérico de error desconocido.
- No debe confiarse en la persistencia de la cadena devuelta más allá de futuras llamadas a `strerror()`.

---

### 🧭 Información adicional

<summary>📎 ¿Por qué se usa en Minishell?</summary>

En Minishell, `strerror()` es particularmente útil cuando:

1. **Necesitas personalizar mensajes de error**: Cuando quieres incluir contexto adicional o formatear el mensaje de manera específica.
2. **Guardas mensajes de error para uso posterior**: Por ejemplo, si quieres registrar errores o mostrarlos en un momento diferente.
3. **Implementas comandos internos**: Al crear versiones propias de comandos del shell, puedes querer imitar el formato exacto de sus mensajes de error.

Ejemplo de implementación de un comando `cd` interno que usa `strerror()`:

```c
int builtin_cd(char **args)
{
    char *dir = args[1];
    
    if (!dir)
        dir = getenv("HOME");
    
    if (chdir(dir) != 0)
    {
        // Formato similar al del comando cd real
        fprintf(stderr, "cd: %s: %s\n", dir, strerror(errno));
        return (1);
    }
    return (0);
}
```

`strerror()` también te permite almacenar mensajes de error para situaciones más complejas, como cuando necesitas realizar operaciones adicionales antes de informar un error:

```c
int handle_command_error(const char *cmd, int error_code)
{
    char *error_msg = strerror(error_code);
    
    // Registrar el error en un archivo de log
    log_error(cmd, error_msg);
    
    // Mostrar mensaje personalizado al usuario
    fprintf(stderr, "Error ejecutando '%s': %s\n", cmd, error_msg);
    
    return (error_code);
}
```

---