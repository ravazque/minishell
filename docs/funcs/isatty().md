
## 🧩 `isatty()`  
<small><code>#include &lt;unistd.h&gt;</code></small>

---

### 🧾 Header
```c
#include <unistd.h>
```

---

### 🧪 Prototipo
```c
int isatty(int fd);
```

---

### 📚 Descripción
Determina si el descriptor de archivo proporcionado hace referencia a un **terminal** (TTY). Es útil para decidir si se debe mostrar un prompt o comportarse de forma interactiva.

---

### 🧰 Sintaxis y ejemplo
```c
if (isatty(STDIN_FILENO)) {
    // Es un terminal
}
```

<summary>🔍 Ejemplo de uso en contexto</summary>

```c
#include <unistd.h>
#include <stdio.h>

int main(void) {
    if (isatty(STDIN_FILENO))
        printf("Entrada estándar es un terminal\n");
    else
        printf("Entrada estándar no es un terminal\n");
    return 0;
}
```


---

### 💡 Consejos de uso
- Es especialmente útil para distinguir entre ejecución interactiva y ejecución desde un script.
- Se suele usar junto con `ttyname()`.

---

### 🔙 Retorno
Devuelve `1` si `fd` es un terminal. Devuelve `0` si no lo es.

---

### ⚠️ Errores
- `errno` se establece si `fd` no es válido.

---

### 🧭 Información adicional
<details>
<summary>📎 ¿Por qué se usa en Minishell?</summary>

Minishell puede verificar si la entrada es un terminal para decidir si mostrar el prompt o comportarse de modo silencioso, por ejemplo, en redirecciones o pipes.

</details>

---
