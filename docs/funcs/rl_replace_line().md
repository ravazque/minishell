## 🧩 `rl_replace_line()`  
<small><code>#include &lt;readline/readline.h&gt;</code></small>

---

### 🧾 Header
```c
#include <readline/readline.h>
```

---

### 🧪 Prototipo
```c
void rl_replace_line(const char *text, int clear_undo);
```

---

### 📚 Descripción
Reemplaza el contenido de la **línea de entrada actual** de Readline con el texto proporcionado. Esta función modifica el buffer interno que contiene lo que el usuario está editando actualmente.

Es especialmente útil para manipular la línea de entrada en respuesta a eventos como señales o para implementar comandos que modifican la línea actual.

---

### 🧰 Sintaxis y ejemplo
```c
rl_replace_line("nuevo texto", 1);  // Reemplaza la línea actual y borra el historial de deshacer
```

<summary>🔍 Ejemplo de uso en contexto (Manejo de señal SIGINT)</summary>

```c
void	handle_sigint(int sig)
{
    (void)sig;
    write(1, "\n", 1);
    rl_replace_line("", 0);    // Limpia la línea actual
    rl_on_new_line();          // Notifica nueva línea
    rl_redisplay();            // Redibuja el prompt
}

int main(void)
{
    struct sigaction sa;
    
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);
    
    // ... resto del código
}
```

---

### 💡 Consejos de uso
- El parámetro `clear_undo` determina si se debe borrar el historial de operaciones deshacer:
  - `0`: Mantiene el historial de deshacer.
  - `1`: Borra el historial de deshacer.
- **Se usa frecuentemente junto con `rl_on_new_line()` y `rl_redisplay()`** para actualizar completamente la línea.
- Ideal para limpiar la línea después de una señal como `SIGINT` (Ctrl+C).
- También se puede usar para implementar funcionalidades como autocompletado o sugerencias.

---

### 🔙 Retorno
No devuelve ningún valor (void).

---

### ⚠️ Errores
- No reporta errores directamente ya que es de tipo `void`.
- Si se llama cuando Readline no está inicializado o fuera de un contexto interactivo, puede causar comportamiento indefinido.
- Si se proporciona un puntero `NULL` como `text`, puede producir segmentation fault.

---

### 🧭 Información adicional

<summary>📎 ¿Por qué se usa en Minishell?</summary>

En Minishell, `rl_replace_line()` se utiliza principalmente para:

1. **Manejar señales**: Especialmente `SIGINT` (Ctrl+C), para limpiar la línea actual y mostrar un nuevo prompt.

2. **Implementar funcionalidades de edición de línea**: Como autocompletado, sugerencias de comandos o corrección de errores.

3. **Restaurar el estado del prompt**: Después de mostrar mensajes o realizar operaciones que podrían interferir con la entrada del usuario.

Un patrón común en el manejo de señales es:

```c
void handle_sigint(int sig)
{
    (void)sig;
    // Salto de línea para separar del texto actual
    write(1, "\n", 1);
    
    // Limpia la línea actual
    rl_replace_line("", 0);
    
    // Indica a Readline que estamos en una nueva línea
    rl_on_new_line();
    
    // Redibuja el prompt (vacío ahora)
    rl_redisplay();
}
```

Este patrón garantiza que la interfaz de usuario permanezca limpia y coherente incluso después de interrupciones como Ctrl+C.

---