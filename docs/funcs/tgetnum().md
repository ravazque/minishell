## 🧩 `tgetnum()`  
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
int tgetnum(const char *id);
```

---

### 📚 Descripción
Obtiene el valor de una **capacidad numérica** de la terminal desde la base de datos termcap.  
Esta función consulta valores numéricos específicos (identificados por `id`) de las características del terminal actual.

---

### 🧰 Sintaxis y ejemplo
```c
int columns = tgetnum("co");  // Obtiene el número de columnas de la terminal
int lines = tgetnum("li");    // Obtiene el número de líneas de la terminal
```


<summary>🔍 Ejemplo de uso en contexto (Dimensiones de la terminal)</summary>

```c
void get_terminal_size(int *width, int *height)
{
    char buffer[2048];
    
    // Inicializar la base de datos termcap
    if (tgetent(buffer, getenv("TERM")) <= 0)
    {
        *width = 80;   // Valores por defecto
        *height = 24;
        return;
    }
    
    // Obtener dimensiones
    *width = tgetnum("co");
    if (*width <= 0)
        *width = 80;   // Valor por defecto
        
    *height = tgetnum("li");
    if (*height <= 0)
        *height = 24;  // Valor por defecto
}
```



---

### 💡 Consejos de uso
- **Debe llamarse después de `tgetent()`**, ya que requiere que la base de datos termcap esté inicializada.
- Las capacidades numéricas se identifican con códigos de dos letras definidos en la base de datos termcap/terminfo.
- Ejemplos comunes de identificadores: `co` (columnas), `li` (líneas), `sg` (magic cookie glitch), `pa` (padding).
- Verifica siempre el valor de retorno ya que un valor negativo indica error.

---

### 🔙 Retorno
Devuelve:
- El **valor numérico** de la capacidad si existe.
- `-1` si la capacidad no existe o no está disponible.

---

### ⚠️ Errores
- Si `tgetent()` no se llamó antes o falló, el comportamiento es indefinido.
- Si el identificador `id` no es válido o no existe en la base de datos, devuelve `-1`.
- No establece `errno`, por lo que no hay manera estándar de obtener más detalles sobre el error.

---

### 🧭 Información adicional

<summary>📎 ¿Por qué se usa en Minishell?</summary>

En Minishell, `tgetnum()` se utiliza principalmente para **obtener información sobre las dimensiones y características de la terminal**, lo que permite adaptar la presentación del prompt y la salida del comando al espacio disponible.

Ejemplo de uso para ajustar el prompt:

```c
void update_prompt_based_on_terminal(void)
{
    int cols;
    
    // Asumimos que tgetent() ya fue llamado con éxito
    cols = tgetnum("co");
    
    if (cols <= 0)
        cols = 80;  // Valor predeterminado
        
    // Ajustar el prompt según el ancho de la terminal
    if (cols < 40)
    {
        // Terminal estrecho, usar prompt compacto
        g_shell.prompt = ft_strdup("$ ");
    }
    else
    {
        // Terminal más ancho, usar prompt descriptivo
        g_shell.prompt = ft_strdup("minishell $ ");
    }
}
```

Esto permite que Minishell ajuste su comportamiento y presentación según las características específicas de la terminal en la que se está ejecutando.



---