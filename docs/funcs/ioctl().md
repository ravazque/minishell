## 🧩 `ioctl()`  
<small><code>#include &lt;sys/ioctl.h&gt;</code></small>

---

### 🧾 Header
```c
#include <sys/ioctl.h>
```

---

### 🧪 Prototipo
```c
int ioctl(int fd, unsigned long request, ...);
```

---

### 📚 Descripción
Realiza operaciones de control de entrada/salida (I/O Control) en un dispositivo o archivo representado por el descriptor `fd`. La función `ioctl()` proporciona una interfaz de bajo nivel para comunicarse directamente con controladores de dispositivos.

El argumento `request` especifica el comando de control solicitado. Dependiendo del comando específico, `ioctl()` puede aceptar un tercer argumento variable, generalmente un puntero a una estructura de datos o un valor entero.

Esta función es extremadamente versátil y se utiliza para operaciones específicas del dispositivo que no pueden realizarse con las operaciones estándar de lectura/escritura.

---

### 🧰 Sintaxis y ejemplo
```c
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>

// Obtener dimensiones del terminal
struct winsize ws;
if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) != -1)
{
    printf("Terminal: %d filas x %d columnas\n", ws.ws_row, ws.ws_col);
}
```

<summary>🔍 Ejemplo de uso en contexto (Obtener dimensiones de un terminal)</summary>

```c
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void print_centered_text(const char *text)
{
    struct winsize ws;
    
    // Obtener dimensiones del terminal
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1)
    {
        perror("ioctl");
        printf("%s\n", text);  // Fallback: imprimir sin centrar
        return;
    }
    
    int text_len = strlen(text);
    int padding = (ws.ws_col - text_len) / 2;
    
    if (padding > 0)
    {
        printf("%*s%s\n", padding, "", text);
    }
    else
    {
        printf("%s\n", text);
    }
}
```

---

### 💡 Consejos de uso
- **Cada dispositivo tiene sus propios comandos** de `ioctl()`. Consulta la documentación del sistema para conocer los comandos específicos disponibles.
- **Verifica siempre el valor de retorno** para detectar errores, ya que muchas llamadas a `ioctl()` pueden fallar de manera silenciosa.
- Los comandos `ioctl()` para terminales son particularmente útiles en shells y programas interactivos para obtener información como dimensiones del terminal.
- Ten cuidado con la **portabilidad**, ya que los comandos disponibles y su comportamiento pueden variar entre sistemas operativos.

---

### 🔙 Retorno
Devuelve `-1` en caso de error y establece `errno` apropiadamente.  
En caso de éxito, el valor devuelto depende del comando específico de `ioctl()` utilizado.

---

### ⚠️ Errores
- `EBADF`: `fd` no es un descriptor de archivo válido.
- `ENOTTY`: `fd` no está asociado a un dispositivo que acepte el comando `request`.
- `EINVAL`: El comando `request` o el argumento no son válidos.
- Otros códigos de error pueden establecerse dependiendo del dispositivo específico y el comando utilizado.

---

### 🧭 Información adicional

<summary>📎 ¿Por qué se usa en Minishell?</summary>

En Minishell, `ioctl()` es especialmente útil para:

1. **Obtener dimensiones del terminal**: Esto permite adaptar la salida para que se vea bien en el terminal actual:

```c
void update_terminal_size(void)
{
    struct winsize ws;
    
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) != -1)
    {
        g_shell.term_width = ws.ws_col;
        g_shell.term_height = ws.ws_row;
    }
    else
    {
        // Valores por defecto si no se puede obtener el tamaño
        g_shell.term_width = 80;
        g_shell.term_height = 24;
    }
}
```

2. **Implementar características interactivas**: Como autocompletado, menús desplegables o editores de línea de comando avanzados:

```c
void handle_window_resize(int signo)
{
    (void)signo;  // Evitar advertencia de no uso
    
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) != -1)
    {
        // Actualizar dimensiones almacenadas
        g_shell.term_width = ws.ws_col;
        g_shell.term_height = ws.ws_row;
        
        // Redibujar interfaz si es necesario
        redraw_prompt_and_line();
    }
}
```

3. **Control de terminal**: Para operaciones avanzadas de terminal como cambiar modos de entrada, controlar flujo, etc.:

```c
void enable_raw_mode(void)
{
    struct termios raw;
    
    tcgetattr(STDIN_FILENO, &g_shell.orig_termios);
    raw = g_shell.orig_termios;
    
    // Desactivar eco, modo canónico, señales, etc.
    raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
    raw.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
    raw.c_cflag |= (CS8);
    raw.c_oflag &= ~(OPOST);
    
    // Establecer tiempo de espera mínimo para leer
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;
    
    // Aplicar nuevos ajustes
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}
```

`ioctl()` es esencial para cualquier shell que necesite interactuar con las características del terminal de manera avanzada, más allá de simple lectura/escritura.

---