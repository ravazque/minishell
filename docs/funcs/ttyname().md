## 🧩 `ttyname()`  
<small><code>#include &lt;unistd.h&gt;</code></small>

---

### 🧾 Header
```c
#include <unistd.h>
```

---

### 🧪 Prototipo
```c
char *ttyname(int fd);
```

---

### 📚 Descripción
Devuelve un puntero a una cadena que contiene el nombre de ruta del dispositivo terminal asociado con el descriptor de archivo `fd`. Esta función es útil para determinar el terminal en el que se está ejecutando un programa.

La función solo tiene éxito si `fd` se refiere a un dispositivo terminal abierto; de lo contrario, devuelve `NULL`.

---

### 🧰 Sintaxis y ejemplo
```c
char *terminal_name = ttyname(STDIN_FILENO);
if (terminal_name)
{
    printf("Terminal conectado: %s\n", terminal_name);  // Por ejemplo: "/dev/ttys001"
}
else
{
    printf("No hay terminal conectado a stdin\n");
}
```

<summary>🔍 Ejemplo de uso en contexto (Verificación de terminal interactiva)</summary>

```c
int is_interactive_shell(void)
{
    // Verifica si la entrada estándar está conectada a un terminal
    if (isatty(STDIN_FILENO))
    {
        char *term_name = ttyname(STDIN_FILENO);
        if (term_name)
        {
            printf("Shell interactivo en terminal: %s\n", term_name);
            return (1);
        }
    }
    printf("Shell no interactivo\n");
    return (0);
}
```

---

### 💡 Consejos de uso
- **Verifica el retorno** de `ttyname()` antes de usarlo, ya que puede devolver `NULL`.
- La función solo funciona con descriptores de archivo que representan terminales; úsala junto con `isatty()` para verificar esto primero.
- El valor devuelto por `ttyname()` apunta a una zona de memoria estática, por lo que será sobrescrito en la siguiente llamada a la función.
- Si necesitas preservar el valor, debes copiarlo a tu propio buffer con `strdup()` o una función similar.

---

### 🔙 Retorno
Devuelve un puntero a una cadena que contiene el nombre de ruta del terminal.  
Devuelve `NULL` si `fd` no está asociado con un terminal o si ocurre un error.

---

### ⚠️ Errores
Cuando `ttyname()` falla, `errno` puede establecerse a:
- `EBADF`: El descriptor de archivo `fd` no es válido.
- `ENOTTY`: El descriptor de archivo `fd` no está asociado a un terminal.
- `EINVAL`: Un argumento inválido fue proporcionado.

---

### 🧭 Información adicional

<summary>📎 ¿Por qué se usa en Minishell?</summary>

En Minishell, `ttyname()` puede ser útil para:

1. **Determinar si el shell es interactivo**: Un shell interactivo generalmente está conectado a un terminal, y `ttyname()` puede ayudar a confirmarlo.

2. **Gestión de señales**: Algunos comportamientos de manejo de señales pueden variar dependiendo de si el shell está conectado a un terminal o no.

3. **Información de depuración**: Mostrar información sobre el entorno en el que se ejecuta el shell.

Ejemplo de cómo podría usarse para adaptar el comportamiento del shell:

```c
void configure_shell_mode(void)
{
    if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
    {
        char *term = ttyname(STDIN_FILENO);
        if (term)
        {
            printf("Minishell ejecutándose en terminal: %s\n", term);
            g_shell.interactive = 1;
            // Configurar modo interactivo: historial, prompt, etc.
            setup_signals_interactive();
        }
    }
    else
    {
        g_shell.interactive = 0;
        // Configurar modo no interactivo: sin prompt, manejo diferente de errores
        setup_signals_non_interactive();
    }
}
```

En un shell interactivo, generalmente se desea mostrar un prompt, gestionar señales como SIGINT (Ctrl+C) de manera amigable, y mantener un historial de comandos. En uno no interactivo (por ejemplo, cuando se redirige entrada desde un archivo), estos comportamientos suelen ser diferentes.

---