## 🧩 `tcgetattr()`  
<small><code>#include &lt;termios.h&gt;</code></small>

---

### 🧾 Header
```c
#include <termios.h>
```

---

### 🧪 Prototipo
```c
int tcgetattr(int fd, struct termios *termios_p);
```

---

### 📚 Descripción
Obtiene los **atributos actuales de la terminal** asociada al descriptor de archivo `fd` y los almacena en la estructura `termios_p`.  
Esta función es fundamental para la **configuración de terminales** y permite consultar configuraciones como la velocidad de transmisión, manejo de caracteres especiales, y otros modos de control.

---

### 🧰 Sintaxis y ejemplo
```c
struct termios term;
tcgetattr(STDIN_FILENO, &term);  // Obtiene los atributos actuales de stdin
```


<summary>🔍 Ejemplo de uso en contexto (Guardando y restaurando la configuración)</summary>

```c
struct termios original;
struct termios raw;

// Guardar la configuración original
tcgetattr(STDIN_FILENO, &original);

// Copiar la configuración para modificarla
tcgetattr(STDIN_FILENO, &raw);

// Modificar para modo raw
raw.c_lflag &= ~(ECHO | ICANON);  // Desactivar eco y modo canónico
tcsetattr(STDIN_FILENO, TCSANOW, &raw);

// ...código que requiere modo raw...

// Restaurar configuración original al terminar
tcsetattr(STDIN_FILENO, TCSANOW, &original);
```



---

### 💡 Consejos de uso
- **Guardar siempre la configuración original** antes de realizar cambios, para poder restaurarla después.
- Se usa generalmente en conjunto con `tcsetattr()` para realizar cambios en la configuración de la terminal.
- Verifica siempre si el descriptor de archivo es válido y corresponde a una terminal antes de llamar a esta función.

---

### 🔙 Retorno
Devuelve `0` si la operación fue exitosa.  
En caso de error, devuelve `-1` y establece la variable `errno` para indicar el error.

---

### ⚠️ Errores
- `EBADF`: El descriptor de archivo `fd` no es válido.
- `ENOTTY`: El descriptor de archivo `fd` no se refiere a una terminal.
- `EINVAL`: El valor en `termios_p` no es válido o contiene valores inválidos.

---

### 🧭 Información adicional

<summary>📎 ¿Por qué se usa en Minishell?</summary>

En Minishell, `tcgetattr()` se utiliza para **manipular el comportamiento de la terminal** y controlar cómo se manejan los caracteres especiales como Ctrl+C, Ctrl+D, etc. 

Ejemplo de uso en configuración de la terminal:

```c
void setup_terminal(void)
{
    struct termios term;
    
    tcgetattr(STDIN_FILENO, &term);  // Obtener configuración actual
    
    // Guardar configuración original para restaurarla después
    g_original_config = term;
    
    // Modificar configuración para manejo personalizado de señales
    term.c_lflag &= ~ECHOCTL;  // Evitar que se muestre ^C al presionar Ctrl+C
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}
```

Esto permite que Minishell tenga un control más preciso sobre la interacción del usuario y el manejo de señales del sistema.



---