## 🧩 `rl_clear_history()`  
<small><code>#include &lt;readline/history.h&gt;</code></small>

---

### 🧾 Header
```c
#include <readline/history.h>
```

---

### 🧪 Prototipo
```c
void rl_clear_history(void);
```

---

### 📚 Descripción
Elimina **todas las entradas** del historial de comandos de Readline. Esta función borra completamente el historial en memoria, liberando todos los recursos asociados a las entradas almacenadas.

Es útil cuando se necesita limpiar el historial por completo, por ejemplo, al finalizar un programa o al implementar un comando `clear-history`.

---

### 🧰 Sintaxis y ejemplo
```c
rl_clear_history();  // Elimina todo el historial de comandos
```

<summary>🔍 Ejemplo de uso en contexto (Comando clear-history)</summary>

```c
int	cmd_clear_history(void)
{
    rl_clear_history();
    printf("Historial borrado correctamente\n");
    return (0);
}

// En el procesamiento de comandos
if (strcmp(line, "clear-history") == 0)
    cmd_clear_history();
```

---

### 💡 Consejos de uso
- **Libera toda la memoria** asociada al historial, por lo que no es necesario hacer free() adicionales.
- **No afecta a los archivos** de historial en disco (como `.bash_history`). Solo borra el historial en memoria.
- Si deseas conservar parte del historial, debes usar funciones específicas como `remove_history()` para eliminar entradas individuales.
- Es una operación que **no se puede deshacer**. Una vez borrado el historial, no hay forma de recuperarlo.

---

### 🔙 Retorno
No devuelve ningún valor (void).

---

### ⚠️ Errores
- No reporta errores directamente ya que es de tipo `void`.
- Sin embargo, si el historial no ha sido inicializado correctamente, su comportamiento puede ser impredecible.
- Si llamas a esta función y luego intentas acceder al historial sin añadir nuevas entradas, podrías encontrar problemas en el programa.

---

### 🧭 Información adicional

<summary>📎 ¿Por qué se usa en Minishell?</summary>

En Minishell, `rl_clear_history()` puede utilizarse para:

1. **Implementar un comando interno** que permita al usuario limpiar su historial de comandos (similar a `history -c` en bash).
2. **Liberar memoria correctamente** al finalizar la ejecución del shell, como parte de las rutinas de limpieza.
3. **Reiniciar el estado del historial** tras cambiar de usuario o al implementar funcionalidades como "modo privado".

Ejemplo de cómo podría implementarse un comando para borrar el historial:

```c
// En el intérprete de comandos
if (strcmp(cmd, "clear-history") == 0)
{
    rl_clear_history();
    ft_putendl_fd("Historial borrado", STDOUT_FILENO);
    return (0);
}
```

También es importante llamarlo durante la limpieza final del programa:

```c
void cleanup_shell(void)
{
    // Liberar otras estructuras...
    
    rl_clear_history();  // Liberar memoria del historial
    // Otras operaciones de limpieza...
}
```

---