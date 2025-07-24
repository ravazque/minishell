## 🧩 `free()`  
<small><code>#include &lt;stdlib.h&gt;</code></small>

---

### 🧾 Header
```c
#include <stdlib.h>
```

---

### 🧪 Prototipo
```c
void free(void *ptr);
```

---

### 📚 Descripción
Libera un bloque de memoria previamente asignado por `malloc()`, `calloc()` o `realloc()`. Esta función **devuelve la memoria al sistema** para que pueda ser reutilizada, evitando así fugas de memoria en programas que asignan memoria dinámicamente.

---

### 🧰 Sintaxis y ejemplo
```c
char *str = malloc(100);
// Usar str...
free(str);  // Liberar la memoria cuando ya no se necesita
```

<summary>🔍 Ejemplo de uso en contexto (Limpieza de memoria en Minishell)</summary>

```c
void clean_command(t_command *cmd)
{
    int i;
    
    if (!cmd)
        return;
        
    i = 0;
    if (cmd->args)
    {
        while (cmd->args[i])
            free(cmd->args[i++]);  // Liberar cada argumento
        free(cmd->args);           // Liberar el array de argumentos
    }
    
    if (cmd->input_file)
        free(cmd->input_file);
        
    if (cmd->output_file)
        free(cmd->output_file);
        
    free(cmd);                     // Liberar la estructura principal
}
```

---

### 💡 Consejos de uso
- **Nunca liberes memoria más de una vez** (double free), ya que esto puede causar comportamientos indefinidos o errores graves.
- **No accedas a la memoria después de liberarla** (uso después de liberación), esto puede causar comportamientos indefinidos.
- **No liberes un puntero NULL** (aunque es seguro, no tiene efecto).
- Es recomendable establecer los punteros a `NULL` después de liberarlos para evitar el uso accidental después de liberación.

---

### 🔙 Retorno
`free()` **no devuelve ningún valor** (tipo `void`).

---

### ⚠️ Errores
- Intentar liberar memoria que no fue asignada con `malloc()`, `calloc()` o `realloc()` puede causar **comportamientos indefinidos** o un error de segmentación.
- Liberar un bloque de memoria **más de una vez** (double free) causa comportamientos indefinidos.
- **Fugas de memoria** se producen cuando no se libera memoria que ya no se necesita. No es un error de la función, sino un error de programación.

---

### 🧭 Información adicional

<summary>📎 ¿Por qué se usa en Minishell?</summary>

En Minishell, la gestión adecuada de la memoria es crucial para evitar fugas de memoria, ya que el programa ejecuta continuamente comandos y procesa entrada del usuario.

Patrones comunes de uso en Minishell incluyen:

1. **Limpieza de comandos después de su ejecución**:
```c
void execute_line(char *line)
{
    t_command *cmd = parse_command(line);
    
    // Ejecutar el comando
    run_command(cmd);
    
    // Liberar toda la memoria asignada
    clean_command(cmd);
}
```

2. **Manejo de señales y terminación del programa**:
```c
void cleanup_and_exit(t_shell *shell)
{
    // Liberar todas las estructuras y memoria antes de salir
    free_history(shell->history);
    free_environment(shell->env);
    // Liberar otras estructuras...
    
    exit(shell->exit_status);
}
```

La correcta liberación de memoria evita que el shell consuma cada vez más recursos durante su ejecución.

---