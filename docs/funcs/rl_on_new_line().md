
## 🧩 `rl_on_new_line()`  
<small><code>#include &lt;readline/readline.h&gt;</code></small>

---

### 🧾 Header
```c
#include <readline/readline.h>
```

---

### 🧪 Prototipo
```c
int rl_on_new_line(void);
```

---

### 📚 Descripción
Activa el reconocimiento de una **nueva línea de entrada** en la línea de comandos de Readline.  
Es **necesaria** cuando se ha modificado el contenido del prompt o la línea de entrada de forma manual, y se desea que Readline actualice correctamente su estado interno antes de redibujar la línea.

---

### 🧰 Sintaxis y ejemplo
```c
rl_replace_line("", 0);      // Limpia la línea actual
rl_on_new_line();            // Notifica que estamos en una nueva línea
rl_redisplay();              // Redibuja el prompt limpio
```


<summary>🔍 Ejemplo de uso en contexto (Manejo de señal SIGINT)</summary>

```c
void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "
", 1);
	rl_replace_line("", 0);    // Borra línea actual
	rl_on_new_line();          // Indica que inicia una nueva línea
	rl_redisplay();            // Muestra el prompt limpio
}
```



---

### 💡 Consejos de uso
- Se **debe usar junto con `rl_replace_line()` y `rl_redisplay()`** si estás manejando señales como `SIGINT` (`Ctrl+C`), para evitar que el prompt quede mal alineado o con texto residual.
- No imprime nada por sí sola, simplemente actualiza el estado interno de Readline.
- No usarla **sin necesidad**, ya que puede interferir con el flujo normal de la línea de entrada.

---

### 🔙 Retorno
Devuelve `0` si la operación fue exitosa.  
En caso de error, devuelve un valor negativo.

---

### ⚠️ Errores
- Puede fallar si **Readline no ha sido inicializado** correctamente o si se llama en un contexto no interactivo.
- Si se usa sin coordinar con `rl_replace_line()` y `rl_redisplay()`, el comportamiento del prompt puede volverse inconsistente.

---

### 🧭 Información adicional

<summary>📎 ¿Por qué se usa en Minishell?</summary>

Readline mantiene un **buffer interno de entrada**. Cuando se recibe una señal (como `Ctrl+C`), ese buffer puede quedar en estado inconsistente.  
Para restaurarlo y redibujar el prompt de manera limpia, se usa el siguiente patrón:

```c
write(1, " ", 1);
rl_replace_line("", 0);
rl_on_new_line();
rl_redisplay();
```

Esto garantiza que el usuario vea el prompt limpio, sin residuos de texto, y pueda seguir escribiendo comandos sin problemas.



---
