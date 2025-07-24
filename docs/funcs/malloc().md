## 🧩 `malloc()`  
<small><code>#include &lt;stdlib.h&gt;</code></small>

---

### 🧾 Header
```c
#include <stdlib.h>
```

---

### 🧪 Prototipo
```c
void *malloc(size_t size);
```

---

### 📚 Descripción
Asigna un bloque de memoria del tamaño especificado en bytes. La **memoria asignada no está inicializada** y contiene valores indeterminados. Esta función es esencial para la gestión dinámica de memoria en C.

---

### 🧰 Sintaxis y ejemplo
```c
int *numbers = malloc(10 * sizeof(int));  // Asigna espacio para 10 enteros
```

<summary>🔍 Ejemplo de uso en contexto (Crear un array dinámico)</summary>

```c
char **split_command(char *command)
{
    char **args = malloc((count_words(command) + 1) * sizeof(char *));
    if (!args)
        return (NULL);  // Error de asignación de memoria
        
    // Procesar y dividir el comando
    
    args[count] = NULL;  // Terminar array con NULL
    return (args);
}
```

---

### 💡 Consejos de uso
- **Siempre verifica el valor de retorno** para asegurarte de que la asignación de memoria fue exitosa.
- La memoria asignada por `malloc()` debe ser **liberada con `free()`** cuando ya no se necesite, para evitar fugas de memoria.
- Para asignar memoria para un array, multiplica el tamaño de cada elemento por el número de elementos: `malloc(n * sizeof(tipo))`.
- Si necesitas memoria inicializada a cero, considera usar `calloc()` en lugar de `malloc()`.

---

### 🔙 Retorno
Devuelve un **puntero a la memoria asignada** si la operación fue exitosa.  
Si falla, devuelve un puntero `NULL`.

---

### ⚠️ Errores
- Devuelve `NULL` si **no hay suficiente memoria disponible** en el sistema para realizar la asignación solicitada.
- Si se solicita una **asignación de tamaño cero**, el comportamiento puede variar según la implementación del sistema. Algunos sistemas devuelven `NULL`, mientras que otros pueden devolver un puntero válido a un objeto de tamaño cero.

---

### 🧭 Información adicional

<summary>📎 ¿Por qué se usa en Minishell?</summary>

En Minishell, `malloc()` se usa extensivamente para:

- Asignar memoria para los argumentos del comando después de dividirlos.
- Crear copias de cadenas para procesar.
- Construir estructuras de datos para el análisis sintáctico y la ejecución de comandos.
- Gestionar el historial de comandos y otras estructuras dinámicas.

Ejemplo de uso típico en Minishell:

```c
t_command *new_command(void)
{
    t_command *cmd = malloc(sizeof(t_command));
    if (!cmd)
        return (NULL);
    
    cmd->args = NULL;
    cmd->input_fd = STDIN_FILENO;
    cmd->output_fd = STDOUT_FILENO;
    cmd->next = NULL;
    
    return (cmd);
}
```

Este patrón se utiliza para crear estructuras que representan comandos, tuberías y redirecciones en la implementación del shell.

---