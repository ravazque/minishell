
## 🧩 `kill()`  
<small><code>#include &lt;signal.h&gt;</code></small>

---

### 🧾 Header
```c
#include <signal.h>
```

---

### 🧪 Prototipo
```c
int kill(pid_t pid, int sig);
```

---

### 📚 Descripción
Envía una señal al proceso o grupo de procesos indicado por `pid`. Es la forma estándar de comunicar o terminar procesos mediante señales, como `SIGKILL`, `SIGTERM`, o señales personalizadas.

---

### 🧰 Sintaxis y ejemplo
```c
kill(pid, SIGINT);  // Envía SIGINT al proceso con PID especificado
```


<summary>🔍 Ejemplo de uso en contexto</summary>

```c
#include <signal.h>
#include <unistd.h>

int main(void) {
    pid_t pid = fork();
    if (pid == 0) {
        while (1) {}  // Hijo esperando indefinidamente
    } else {
        kill(pid, SIGTERM);  // Termina el hijo
    }
    return 0;
}
```


---

### 💡 Consejos de uso
- Se puede usar `kill(getpid(), sig)` para enviarse una señal a uno mismo.
- Si `pid` es negativo, se puede enviar una señal a un grupo de procesos.

---

### 🔙 Retorno
Devuelve `0` si la señal fue enviada correctamente. Devuelve `-1` si ocurre un error.

---

### ⚠️ Errores
- `ESRCH`: el proceso no existe.
- `EPERM`: no tienes permisos para enviar señales a ese proceso.

---

### 🧭 Información adicional
<details>
<summary>📎 ¿Por qué se usa en Minishell?</summary>

Minishell puede usar `kill()` para implementar comandos como `kill` o para manejar la finalización de procesos hijos, como cuando se implementa el manejo de `Ctrl+C` o `Ctrl+\` en procesos en foreground.

</details>

---
