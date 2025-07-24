## 🧩 `tgoto()`  
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
char *tgoto(const char *cap, int col, int row);
```

---

### 📚 Descripción
Aplica parámetros de columna y fila a una **secuencia de posicionamiento del cursor** obtenida previamente con `tgetstr()`.  
Esta función formatea la secuencia de control para mover el cursor a una posición específica en la terminal.

---

### 🧰 Sintaxis y ejemplo
```c
char buffer[1024];
char *buffer_ptr = buffer;
char *cursor_move = tgetstr("cm", &buffer_ptr);  // Secuencia de movimiento de cursor
char *positioned = tgoto(cursor_move, 10, 5);    // Posicionar en columna 10, fila 5
```


<summary>🔍 Ejemplo de uso en contexto (Dibujar un marco)</summary>

```c
void draw_box(int x, int y, int width, int height)
{
    char buffer[1024];
    char *buffer_ptr = buffer;
    char *cursor_move;
    int i;
    
    // Asumimos que tgetent() ya fue llamado con éxito
    cursor_move = tgetstr("cm", &buffer_ptr);
    if (!cursor_move)
        return;
    
    // Dibujar la parte superior del marco
    tputs(tgoto(cursor_move, x, y), 1, putchar);
    putchar('+');
    for (i = 0; i < width - 2; i++)
        putchar('-');
    putchar('+');
    
    // Dibujar los lados
    for (i = 1; i < height - 1; i++)
    {
        tputs(tgoto(cursor_move, x, y + i), 1, putchar);
        putchar('|');
        tputs(tgoto(cursor_move, x + width - 1, y + i), 1, putchar);
        putchar('|');
    }
    
    // Dibujar la parte inferior del marco
    tputs(tgoto(cursor_move, x, y + height - 1), 1, putchar);
    putchar('+');
    for (i = 0; i < width - 2; i++)
        putchar('-');
    putchar('+');
}
```



---

### 💡 Consejos de uso
- **Requiere una secuencia de posicionamiento del cursor** obtenida con `tgetstr("cm", ...)`.
- El orden de los parámetros es **columna, fila** (¡no fila, columna!).
- La secuencia resultante debe aplicarse con `tputs()` para enviarla a la terminal.
- Las coordenadas generalmente comienzan en (0,0) para la esquina superior izquierda de la terminal.
- Verifica siempre que el valor de `cap` no sea `NULL` antes de usar `tgoto()`.

---

### 🔙 Retorno
Devuelve:
- Un **puntero a la secuencia formateada** si tiene éxito.
- `NULL` si la secuencia de posicionamiento es inválida o si hay un error.

---

### ⚠️ Errores
- Si `cap` es `NULL` o no es una secuencia válida de posicionamiento del cursor.
- Si las coordenadas están fuera del rango soportado por la terminal.
- En algunas implementaciones, puede devolver un puntero a un buffer estático que se sobrescribe en cada llamada.

---

### 🧭 Información adicional

<summary>📎 ¿Por qué se usa en Minishell?</summary>

En Minishell, `tgoto()` se utiliza para **posicionar el cursor en lugares específicos de la terminal**, lo que es útil para implementar:

- Interfaces de usuario avanzadas (menús, cuadros de diálogo)
- Edición de línea con movimiento arbitrario del cursor
- Sugerencias y autocompletado en ubicaciones específicas
- Barra de estado en la parte inferior de la pantalla

Ejemplo de uso para posicionar un mensaje de estado:

```c
void display_status_message(const char *message)
{
    char buffer[1024];
    char *buffer_ptr = buffer;
    char *cursor_move, *clear_to_eol;
    int term_width, term_height;
    
    // Asumimos que tgetent() ya fue llamado con éxito
    
    // Obtener dimensiones de la terminal
    term_width = tgetnum("co");
    term_height = tgetnum("li");
    if (term_width <= 0 || term_height <= 0)
        return;
    
    // Obtener secuencias necesarias
    cursor_move = tgetstr("cm", &buffer_ptr);
    clear_to_eol = tgetstr("ce", &buffer_ptr);
    if (!cursor_move || !clear_to_eol)
        return;
    
    // Posicionar en la última línea y mostrar mensaje
    tputs(tgoto(cursor_move, 0, term_height - 1), 1, putchar);
    tputs(clear_to_eol, 1, putchar);  // Limpiar la línea
    ft_putstr_fd(message, STDOUT_FILENO);
    
    // Devolver el cursor al prompt
    tputs(tgoto(cursor_move, 0, term_height - 2), 1, putchar);
}
```

Esto permite a Minishell proporcionar información visual al usuario sin interrumpir la entrada de comandos.



---