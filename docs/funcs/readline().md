## 🧩 `readline()`  
<small><code>#include &lt;readline/readline.h&gt;</code></small>

---

### 🧾 Header
```c
#include <readline/readline.h>
```

---

### 🧪 Prototipo
```c
char *readline(const char *prompt);
```

---

### 📚 Descripción
Lee una **línea completa de texto** desde la entrada estándar y la devuelve como una cadena de caracteres terminada en nulo.  
Esta función es el núcleo de la biblioteca Readline y proporciona capacidades avanzadas de edición de línea, incluyendo historial, autocompletado, y edición con teclas emacs/vi.

---

### 🧰 Sintaxis y ejemplo
```c
char *input = readline("Ingrese un comando: ");  // Muestra el prompt y espera entrada
```


<summary>🔍 Ejemplo de uso en contexto (Bucle principal de Shell)</summary>

```c
int main(void)
{
    char *line;
    int status = 0;
    
    while (status == 0)
    {
        // Mostrar prompt y obtener entrada
        line = readline("minishell$ ");
        
        // Manejar EOF (Ctrl+D)
        if (!line)
        {
            printf("exit\n");
            break;
        }
        
        // Procesar la línea si no está vacía
        if (*line)
        {
            add_history(line);  // Añadir al historial
            status = execute_command(line);
        }
        
        // Liberar memoria
        free(line);
    }
    
    return (status);
}
```



---

### 💡 Consejos de uso
- **La cadena devuelta debe ser liberada** con `free()` cuando ya no se necesite.
- Si el usuario presiona Ctrl+D (EOF), la función devuelve `NULL`.
- El prompt puede incluir secuencias de escape ANSI para colores y formato.
- Usar en combinación con `add_history()` para permitir recuperar comandos previos.
- Puede configurarse para usar teclas de edición estilo emacs o vi mediante variables globales.

---

### 🔙 Retorno
Devuelve:
- Un **puntero a la línea leída** si tiene éxito (debe liberarse con `free()`).
- `NULL` si se encuentra EOF o si hay un error.

---

### ⚠️ Errores
- Si no hay memoria suficiente para almacenar la línea leída.
- Si se produce una interrupción de señal que no se maneja adecuadamente.
- Si hay problemas con el terminal o con la configuración de Readline.

---

### 🧭 Información adicional

<summary>📎 ¿Por qué se usa en Minishell?</summary>

`readline()` es una de las funciones más importantes en Minishell ya que proporciona una **interfaz de línea de comandos robusta y completa**. Al usarla, Minishell obtiene automáticamente:

1. **Edición de línea avanzada**: El usuario puede:
   - Moverse por la línea con las teclas de flecha
   - Editar en cualquier posición del cursor
   - Usar atajos de teclado (Ctrl+A para inicio, Ctrl+E para fin, etc.)

2. **Gestión de historial**: En combinación con `add_history()`, permite:
   - Navegar por comandos anteriores con las flechas arriba/abajo
   - Buscar en el historial con Ctrl+R
   - Mantener persistencia entre sesiones

3. **Manejo de señales integrado**: Gestiona correctamente Ctrl+C, Ctrl+D, etc.

4. **Customización del prompt**: Permite mostrar información útil como:
   - Directorio actual
   - Estado del último comando
   - Información del usuario y sistema

Ejemplo de uso en el núcleo de Minishell:

```c
void minishell_loop(void)
{
    char *line;
    t_command *commands;
    
    while (g_shell.running)
    {
        // Generar prompt con información dinámica
        char *prompt = create_custom_prompt();
        
        // Leer comando con readline
        line = readline(prompt);
        free(prompt);
        
        // Manejar EOF (Ctrl+D)
        if (!line)
        {
            printf("exit\n");
            g_shell.running = 0;
            continue;
        }
        
        // Procesar y ejecutar si no está vacío
        if (*line)
        {
            add_history(line);  // Añadir al historial
            commands = parse_line(line);
            execute_commands(commands);
            free_commands(commands);
        }
        
        free(line);
    }
}
```

Esta función es esencial para proporcionar una experiencia de usuario familiar y potente, similar a la de shells como bash o zsh.



---