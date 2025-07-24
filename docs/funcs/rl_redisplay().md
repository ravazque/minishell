## 🧩 `rl_redisplay()`  
<small><code>#include &lt;readline/readline.h&gt;</code></small>

---

### 🧾 Header
```c
#include <readline/readline.h>
```

---

### 🧪 Prototipo
```c
void rl_redisplay(void);
```

---

### 📚 Descripción
Actualiza la **visualización de la línea** actual en la terminal según el estado interno de Readline. Esta función redibuja la línea de comandos, mostrando los cambios que se hayan realizado en el buffer de entrada.

Es esencial cuando se modifica la línea de entrada programáticamente (con funciones como `rl_replace_line()`) o cuando se necesita actualizar el prompt después de eventos como señales.

---

### 🧰 Sintaxis y ejemplo
```c
rl_redisplay();  // Redibuja la línea actual
```

<summary>🔍 Ejemplo de uso en contexto (Restauración después de SIGINT)</summary>

```c
void	handle_sigint(int sig)
{
    (void)sig;
    write(1, "\n", 1);          // Nueva línea
    rl_replace_line("", 0);     // Limpia el buffer
    rl_on_new_line();           // Notifica a Readline que estamos en una nueva línea
    rl_redisplay();             // Redibuja el prompt limpio
}

int main(void)
{
    signal(SIGINT, handle_sigint);
    
    while (1)
    {
        char *line = readline("minishell$ ");
        // Procesar la entrada...
    }
    
    return (0);
}
```

---

### 💡 Consejos de uso
- **Debe llamarse después de modificar** el contenido de la línea con funciones como `rl_replace_line()`.
- Forma parte del **trío de funciones** que se usan juntas para manejar señales: `rl_replace_line()`, `rl_on_new_line()` y `rl_redisplay()`.
- No realizar llamadas innecesarias a esta función, ya que puede causar parpadeo en la terminal.
- Es particularmente útil en manejadores de señales y para implementar funcionalidades interactivas como autocompletado.

---

### 🔙 Retorno
No devuelve ningún valor (void).

---

### ⚠️ Errores
- No reporta errores directamente ya que es de tipo `void`.
- Si se llama cuando Readline no está inicializado o fuera de un contexto interactivo, puede causar comportamiento indefinido.
- En entornos con restricciones de E/S, puede no funcionar como se espera si la terminal no está configurada correctamente.

---

### 🧭 Información adicional

<summary>📎 ¿Por qué se usa en Minishell?</summary>

En Minishell, `rl_redisplay()` es crucial para mantener una interfaz de usuario coherente y receptiva:

1. **Manejo de señales**: Después de recibir señales como `SIGINT` (Ctrl+C), redibuja el prompt para que el usuario pueda seguir interactuando con el shell.

```c
void handle_sigint(int sig)
{
    // Código para manejar la señal...
    rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay();  // Muestra el prompt limpio nuevamente
}
```

2. **Funcionalidades interactivas**: Para implementar características como autocompletado, historial interactivo o sugerencias en tiempo real.

```c
void suggest_command(const char *suggestion)
{
    // Guarda la entrada actual del usuario
    char *current_input = strdup(rl_line_buffer);
    
    // Reemplaza con la sugerencia
    rl_replace_line(suggestion, 0);
    rl_redisplay();  // Muestra la sugerencia
    
    // Espera confirmación del usuario...
    
    // Si el usuario rechaza, restaura la entrada original
    rl_replace_line(current_input, 0);
    rl_redisplay();  // Actualiza la pantalla
    
    free(current_input);
}
```

3. **Actualizaciones dinámicas**: Para mostrar información en tiempo real sin interferir con la entrada del usuario.

La función forma parte esencial del flujo de trabajo para mantener la experiencia de línea de comandos funcionando correctamente, especialmente después de interrupciones o modificaciones programáticas de la entrada.

---