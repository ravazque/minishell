## 🧩 `tgetstr()`  
<small><code>#include &lt;term.h&gt;</code> o <code>#include &lt;curses.h&gt;</code></small>

---

### 🧾 Header
```c
#include <term.h>
```
o
```c
#include <curses.h>
```

---

### 🧪 Prototipo
```c
char *tgetstr(const char *id, char **area);
```

---

### 📚 Descripción
Obtiene una **secuencia de escape de control** específica de la terminal desde la base de datos termcap.  
Esta función recupera secuencias de caracteres que permiten controlar diferentes aspectos de la terminal, como mover el cursor, borrar la pantalla, cambiar colores, etc.

---

### 🧰 Sintaxis y ejemplo
```c
char buffer[1024];
char *buffer_ptr = buffer;
char *clear_screen = tgetstr("cl", &buffer_ptr);  // Obtiene secuencia para limpiar pantalla
```


<summary>🔍 Ejemplo de uso en contexto (Controlando la terminal)</summary>

```c
void setup_terminal_controls(void)
{
    char buffer[1024];
    char *buffer_ptr = buffer;
    char *clear_screen;
    char *move_cursor;
    
    // Asumimos que tgetent() ya fue llamado con éxito
    
    // Obtener secuencias de control
    clear_screen = tgetstr("cl", &buffer_ptr);
    move_cursor = tgetstr("cm", &buffer_ptr);
    
    if (clear_screen)
    {
        tputs(clear_screen, 1, putchar);  // Limpiar la pantalla
    }
    
    if (move_cursor)
    {
        // Mover cursor a posición (10, 5)
        char *positioned = tgoto(move_cursor, 10, 5);
        tputs(positioned, 1, putchar);
    }
}
```



---

### 💡 Consejos de uso
- **Debe llamarse después de `tgetent()`**, ya que requiere que la base de datos termcap esté inicializada.
- El parámetro `area` es un puntero a un puntero de caracteres donde se copiará la secuencia. Debe apuntar a un buffer con suficiente espacio.
- Después de cada llamada a `tgetstr()`, el puntero `*area` se actualiza para apuntar justo después de la secuencia copiada.
- Para usar las secuencias obtenidas, generalmente se utiliza la función `tputs()`.
- Ejemplos comunes de identificadores: `cl` (clear screen), `cm` (cursor move), `us` (underline start), `ue` (underline end).

---

### 🔙 Retorno
Devuelve:
- Un **puntero a la secuencia de caracteres** si la capacidad existe.
- `NULL` si la capacidad no existe o no está disponible.

---

### ⚠️ Errores
- Si `tgetent()` no se llamó antes o falló, el comportamiento es indefinido.
- Si el buffer apuntado por `*area` no tiene suficiente espacio, puede ocurrir un desbordamiento.
- Si el identificador `id` no es válido o no existe en la base de datos, devuelve `NULL`.

---

### 🧭 Información adicional

<summary>📎 ¿Por qué se usa en Minishell?</summary>

En Minishell, `tgetstr()` se utiliza para **obtener secuencias que controlan la apariencia y comportamiento de la terminal**. Esto permite implementar funcionalidades como:

- Limpiar la pantalla (`cl`)
- Cambiar colores (`AF`, `AB` para colores de frente y fondo)
- Mover el cursor (`cm`)
- Borrar hasta el final de la línea (`ce`)
- Resaltar texto (combinando `so`/`se` o `us`/`ue`)

Ejemplo de uso para implementar un prompt colorido:

```c
void setup_colorful_prompt(void)
{
    char buffer[1024];
    char *buf_ptr = buffer;
    char *set_bold, *set_color, *reset_all;
    
    // Asumimos que tgetent() ya fue llamado con éxito
    
    set_bold = tgetstr("md", &buf_ptr);    // Secuencia para texto en negrita
    set_color = tgetstr("AF", &buf_ptr);   // Secuencia para color de texto
    reset_all = tgetstr("me", &buf_ptr);   // Secuencia para restablecer atributos
    
    // Guardar secuencias para usarlas al mostrar el prompt
    if (set_bold && set_color && reset_all)
    {
        g_shell.prompt_start = ft_strjoin(set_bold, tparm(set_color, COLOR_GREEN));
        g_shell.prompt_end = ft_strdup(reset_all);
    }
    else
    {
        g_shell.prompt_start = ft_strdup("");
        g_shell.prompt_end = ft_strdup("");
    }
}
```

Esto proporciona a Minishell la capacidad de presentar una interfaz más rica y amigable para el usuario.



---