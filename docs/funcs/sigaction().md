## 🧩 `sigaction()`  
<small><code>#include &lt;signal.h&gt;</code></small>

---

### 🧾 Header
```c
#include <signal.h>
```

---

### 🧪 Prototipo
```c
int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
```

---

### 📚 Descripción
Examina y/o cambia la **acción** asociada a una señal específica. Es una alternativa más potente a `signal()`, ya que permite **mayor control sobre el comportamiento del manejador** de señales y ofrece más portabilidad entre diferentes sistemas UNIX.

La estructura `sigaction` contiene información sobre cómo debe comportarse el proceso cuando recibe una señal, incluyendo la función manejadora, máscaras de señal y flags.

---

### 🧰 Sintaxis y ejemplo
```c
void handle_signal(int sig)
{
    printf("Señal %d recibida\n", sig);
}

struct sigaction sa;
sa.sa_handler = handle_signal;
sigemptyset(&sa.sa_mask);
sa.sa_flags = 0;

sigaction(SIGINT, &sa, NULL);
```

<summary>🔍 Ejemplo de uso en contexto (Configuración completa)</summary>

```c
void	handle_sigint(int sig)
{
    (void)sig;
    write(1, "\n", 1);
    rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay();
}

int	setup_signals(void)
{
    struct sigaction	sa_int;
    struct sigaction	sa_quit;
    
    sa_int.sa_handler = handle_sigint;
    sa_int.sa_flags = SA_RESTART;
    sigemptyset(&sa_int.sa_mask);
    
    sa_quit.sa_handler = SIG_IGN;
    sa_quit.sa_flags = 0;
    sigemptyset(&sa_quit.sa_mask);
    
    if (sigaction(SIGINT, &sa_int, NULL) == -1)
        return (-1);
    if (sigaction(SIGQUIT, &sa_quit, NULL) == -1)
        return (-1);
    
    return (0);
}
```

---

### 💡 Consejos de uso
- La estructura `sigaction` tiene estos campos principales:
  - `sa_handler`: Función que maneja la señal (o `SIG_IGN`/`SIG_DFL`).
  - `sa_mask`: Conjunto de señales que se bloquean durante la ejecución del manejador.
  - `sa_flags`: Modifica el comportamiento del manejador (ej: `SA_RESTART` para reiniciar llamadas interrumpidas).
- Usar `sigemptyset()` para inicializar `sa_mask` antes de configurar `sigaction`.
- `SA_RESTART` es útil para que las funciones bloqueantes (como `read()`) se reanuden automáticamente tras una señal.

---

### 🔙 Retorno
Devuelve `0` si la operación fue exitosa.  
En caso de error, devuelve `-1` y establece `errno`.

---

### ⚠️ Errores
- `EINVAL`: Señal inválida o se intentó capturar una señal que no se puede capturar o ignorar.
- `EFAULT`: Las direcciones de `act` o `oldact` no son válidas.
- No todas las señales pueden ser capturadas o ignoradas. Por ejemplo, `SIGKILL` y `SIGSTOP` no pueden ser manejadas.

---

### 🧭 Información adicional

<summary>📎 ¿Por qué se usa en Minishell?</summary>

En Minishell, `sigaction()` se prefiere sobre `signal()` por varias razones:

1. **Mayor control y fiabilidad**: Permite configurar con precisión cómo se manejan las señales.
2. **Bloqueo de señales durante el manejo**: Evita interrupciones durante la ejecución del manejador.
3. **Comportamiento consistente**: A diferencia de `signal()`, cuyo comportamiento puede variar entre sistemas.

Se utiliza principalmente para:

- **Manejar SIGINT (Ctrl+C)**: Para mostrar un nuevo prompt sin terminar el shell.
- **Ignorar SIGQUIT (Ctrl+\\)**: Para evitar que el shell termine con esta combinación.
- **Restaurar comportamientos por defecto**: En procesos hijo que ejecutan comandos.

```c
// Configuración para el proceso principal
struct sigaction sa;
sa.sa_handler = handle_sigint;
sa.sa_flags = SA_RESTART;  // Para que readline funcione correctamente
sigemptyset(&sa.sa_mask);
sigaction(SIGINT, &sa, NULL);

// Para ignorar SIGQUIT
sa.sa_handler = SIG_IGN;
sigaction(SIGQUIT, &sa, NULL);
```

Esta configuración garantiza un manejo más robusto de señales que con `signal()`.

---