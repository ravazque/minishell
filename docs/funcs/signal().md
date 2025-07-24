## 🧩 `signal()`  
<small><code>#include &lt;signal.h&gt;</code></small>

---

### 🧾 Header
```c
#include <signal.h>
```

---

### 🧪 Prototipo
```c
typedef void (*sighandler_t)(int);
sighandler_t signal(int signum, sighandler_t handler);
```

---

### 📚 Descripción
Establece la **acción** a realizar cuando se recibe una señal específica. Permite **registrar un manejador de señales** que será invocado cuando el proceso reciba la señal indicada por `signum`.

En sistemas modernos, se recomienda usar `sigaction()` en su lugar, ya que ofrece mayor control y es más portable.

---

### 🧰 Sintaxis y ejemplo
```c
void handle_sigint(int sig)
{
    printf("\nSeñal SIGINT recibida (%d)\n", sig);
}

// Registrar el manejador para SIGINT (Ctrl+C)
signal(SIGINT, handle_sigint);
```

<summary>🔍 Ejemplo de uso en contexto (Manejo de Ctrl+C)</summary>

```c
void	handle_sigint(int sig)
{
    (void)sig;
    write(1, "\n", 1);
    rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay();
}

int main(void)
{
    signal(SIGINT, handle_sigint);  // Gestiona Ctrl+C
    signal(SIGQUIT, SIG_IGN);       // Ignora Ctrl+\
    
    // ... resto del código
}
```

---

### 💡 Consejos de uso
- Existen tres valores especiales para el parámetro `handler`:
  - `SIG_DFL`: Restaura el comportamiento por defecto de la señal.
  - `SIG_IGN`: Ignora la señal.
  - Una función que será invocada cuando se reciba la señal.
- El manejador de señales debe ser una función que tome un parámetro de tipo `int` y devuelva `void`.
- En entornos multihilo, es mejor usar `sigaction()`.
- No es seguro utilizar funciones no asíncronas (como `printf`) en los manejadores de señales.

---

### 🔙 Retorno
Devuelve el **manejador anterior** (que puede ser `SIG_DFL`, `SIG_IGN` o un puntero a función).  
En caso de error, devuelve `SIG_ERR` y establece `errno`.

---

### ⚠️ Errores
- `EINVAL`: Señal inválida o se intentó capturar una señal que no se puede capturar o ignorar.
- No todas las señales pueden ser capturadas o ignoradas. Por ejemplo, `SIGKILL` y `SIGSTOP` no pueden ser manejadas.
- Si se recibe una señal mientras se está ejecutando el manejador de otra señal, el comportamiento puede ser impredecible.

---

### 🧭 Información adicional

<summary>📎 ¿Por qué se usa en Minishell?</summary>

En Minishell, la función `signal()` se utiliza principalmente para:

1. **Manejar la señal SIGINT (Ctrl+C)**: Para mostrar un nuevo prompt cuando el usuario presiona Ctrl+C.
2. **Ignorar la señal SIGQUIT (Ctrl+\\)**: Para evitar que el programa termine cuando el usuario presiona Ctrl+\.
3. **Restaurar el comportamiento por defecto en procesos hijo**: Para que los comandos ejecutados por el shell puedan manejar las señales normalmente.

```c
// En el proceso principal
signal(SIGINT, handle_sigint);  // Personaliza Ctrl+C
signal(SIGQUIT, SIG_IGN);       // Ignora Ctrl+\

// Antes de ejecutar un comando en un proceso hijo
signal(SIGINT, SIG_DFL);        // Restaura comportamiento por defecto
signal(SIGQUIT, SIG_DFL);       // Restaura comportamiento por defecto
```

---