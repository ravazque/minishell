## 🧩 `tgetflag()`  
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
int tgetflag(const char *id);
```

---

### 📚 Descripción
Obtiene el valor de una **capacidad booleana** de la terminal desde la base de datos termcap.  
Esta función consulta si una capacidad específica (identificada por `id`) está disponible en el terminal actual.

---

### 🧰 Sintaxis y ejemplo
```c
int has_color = tgetflag("Co");  // Verifica si la terminal soporta colores
```


<summary>🔍 Ejemplo de uso en contexto (Verificación de capacidades)</summary>

```c
void check_terminal_capabilities(void)
{
    // Primero inicializamos la base de datos termcap
    char buffer[2048];
    if (tgetent(buffer, getenv("TERM")) <= 0)
        return;
    
    // Verificamos capacidades booleanas
    if (tgetflag("am"))
        printf("Terminal supports automatic margins\n");
    else
        printf("Terminal does not support automatic margins\n");
        
    if (tgetflag("km"))
        printf("Terminal has a meta key\n");
    else
        printf("Terminal does not have a meta key\n");
}
```



---

### 💡 Consejos de uso
- **Debe llamarse después de `tgetent()`**, ya que requiere que la base de datos termcap esté inicializada.
- Las capacidades booleanas se identifican con códigos de dos letras definidos en la base de datos termcap/terminfo.
- Ejemplos comunes de identificadores: `am` (automatic margins), `km` (has meta key), `xn` (newline ignored after 80 cols).

---

### 🔙 Retorno
Devuelve:
- `1` si la capacidad está disponible.
- `0` si la capacidad no está disponible o no existe.

---

### ⚠️ Errores
- Si `tgetent()` no se llamó antes o falló, el comportamiento es indefinido.
- Si el identificador `id` no es válido o no existe en la base de datos, devuelve `0`.
- No establece `errno`, por lo que no hay manera estándar de distinguir entre "capacidad no disponible" y "capacidad no existe".

---

### 🧭 Información adicional

<summary>📎 ¿Por qué se usa en Minishell?</summary>

En Minishell, `tgetflag()` se utiliza para **detectar capacidades específicas de la terminal** que pueden afectar la forma en que se muestran los comandos, el prompt, o cómo se manejan las entradas del usuario.

Ejemplo de uso para adaptar el comportamiento del shell:

```c
void configure_terminal_display(void)
{
    // Asumimos que tgetent() ya fue llamado con éxito
    
    // Verificar si podemos usar colores para mejorar la visualización
    if (tgetflag("Co"))
        g_shell.use_colors = 1;
    else
        g_shell.use_colors = 0;
        
    // Verificar si la terminal soporta teclas de función
    if (tgetflag("kf"))
        g_shell.function_keys_enabled = 1;
    else
        g_shell.function_keys_enabled = 0;
}
```

Esto permite que Minishell se adapte a las capacidades de la terminal en la que se está ejecutando, mejorando la experiencia del usuario.



---