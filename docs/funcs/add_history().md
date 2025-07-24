
## 🧩 `add_history()`  
<small><code>#include &lt;readline/readline.h&gt;</code></small>

---

### 🧾 Header
```c
#include <readline/readline.h>
```

---

### 🧪 Prototipo
```c
int add_history(const char *line);
```

---

### 📚 Descripción
Agrega una línea de texto al **historial de comandos** de Readline. Este historial es el que se almacena y puede ser recorrido por el usuario con las teclas de flecha hacia arriba y hacia abajo, permitiéndole recuperar comandos previos.

---

### 🧰 Sintaxis y ejemplo
```c
add_history("echo Hello World");  // Agrega la línea al historial de comandos
```


<summary>🔍 Ejemplo de uso en contexto (Comando del Shell)</summary>

```c
char *line = readline("Minishell$ ");
if (line && *line)
{
    add_history(line);  // Agrega el comando al historial
}
free(line);
```



---

### 💡 Consejos de uso
- **Se usa para guardar comandos** que el usuario ha ingresado para que pueda acceder a ellos más tarde.
- La función **devuelve 0 si tiene éxito**, y un valor negativo si ocurre un error al agregar la línea al historial.
- **No almacena líneas vacías**, por lo que es mejor asegurarse de que la línea no esté vacía antes de agregarla.

---

### 🔙 Retorno
Devuelve `0` si la operación fue exitosa.  
En caso de error, devuelve un valor negativo.

---

### ⚠️ Errores
- Si **el historial no se ha inicializado** correctamente (por ejemplo, si no se ha configurado el almacenamiento adecuado del historial), puede devolver un error.
- Puede no funcionar correctamente si se le pasa una **línea nula o vacía**.

---

### 🧭 Información adicional

<summary>📎 ¿Por qué se usa en Minishell?</summary>

Minishell necesita mantener un historial de comandos ingresados por el usuario para ofrecer la funcionalidad de navegar a través de comandos anteriores con las teclas de flecha. Esto se logra mediante `add_history()`, que almacena los comandos en un buffer para que el usuario pueda recuperarlos más tarde.

Ejemplo de cómo podría usarse dentro del bucle principal:

```c
char *line = readline("Minishell$ ");
if (line && *line)
{
    add_history(line);  // Guarda la línea en el historial
}
```

Esto permite que el usuario presione las teclas de flecha hacia arriba o hacia abajo para navegar a través de los comandos previos sin tener que escribirlos nuevamente.



---
