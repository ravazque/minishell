
## 🧩 `tputs()`  
<small><code>#include &lt;term.h&gt;</code></small>

---

### 🧾 Header
```c
#include <term.h>
```

---

### 🧪 Prototipo
```c
int tputs(const char *str, int affcnt, int (*putc)(int));
```

---

### 📚 Descripción
La función `tputs()` se utiliza para imprimir cadenas de control de terminal, procesando posibles retrasos embebidos en ellas. Estas cadenas suelen obtenerse mediante `tgetstr()` desde la base de datos termcap.

---

### 🧰 Sintaxis y ejemplo
```c
tputs(tgetstr("cl", NULL), 1, putchar);  // Limpia la pantalla
```


<summary>🔍 Ejemplo de uso en contexto</summary>

```c
#include <term.h>
#include <unistd.h>

int main(void) {
    setupterm(NULL, STDOUT_FILENO, NULL);
    tputs(tgetstr("cl", NULL), 1, putchar);
    return 0;
}
```


---

### 💡 Consejos de uso
- `affcnt` representa la cantidad de líneas afectadas (usualmente 1).
- El tercer argumento es una función que imprime caracteres (frecuentemente `putchar`).
- Requiere llamar previamente a `setupterm()`.

---

### 🔙 Retorno
Devuelve un valor negativo si hay error, `0` o positivo en caso contrario.

---

### ⚠️ Errores
- Si no se ha inicializado el terminal con `setupterm()`, el comportamiento puede ser indefinido.
- Las cadenas `str` deben provenir de `tgetstr()` para ser válidas.

---

### 🧭 Información adicional
<details>
<summary>📎 ¿Por qué se usa en Minishell?</summary>

`Minishell` puede usar `tputs()` para manipular la pantalla de forma más controlada, como limpiar el prompt o mover el cursor cuando no se depende únicamente de Readline.

</details>

---
