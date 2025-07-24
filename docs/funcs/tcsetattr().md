
## 🧩 `tcsetattr()`  
<small><code>#include &lt;termios.h&gt;</code></small>

---

### 🧾 Header
```c
#include <termios.h>
```

---

### 🧪 Prototipo
```c
int tcsetattr(int fd, int optional_actions, const struct termios *termios_p);
```

---

### 📚 Descripción
Configura los parámetros del terminal referenciado por el descriptor de archivo `fd`, como entrada sin eco, modo canónico, velocidad de transmisión, etc.

---

### 🧰 Sintaxis y ejemplo
```c
tcsetattr(STDIN_FILENO, TCSANOW, &termios);
```

<summary>🔍 Ejemplo de uso en contexto</summary>

```c
#include <termios.h>
#include <unistd.h>

int main(void) {
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
    return 0;
}
```


---

### 💡 Consejos de uso
- `TCSANOW`: aplica los cambios inmediatamente.
- Otras opciones incluyen `TCSADRAIN` y `TCSAFLUSH`.

---

### 🔙 Retorno
Devuelve `0` si tuvo éxito. `-1` en caso de error, y establece `errno`.

---

### ⚠️ Errores
- Fallará si `fd` no es válido o no es un terminal.

---

### 🧭 Información adicional
<details>
<summary>📎 ¿Por qué se usa en Minishell?</summary>

Minishell puede querer configurar el terminal para leer caracteres sin esperar un `Enter`, o para desactivar temporalmente el eco mientras se procesa una línea de entrada.

</details>

---
