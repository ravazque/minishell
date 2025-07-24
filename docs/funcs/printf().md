## 🧩 `printf()`  
<small><code>#include &lt;stdio.h&gt;</code></small>

---

### 🧾 Header
```c
#include <stdio.h>
```

---

### 🧪 Prototipo
```c
int printf(const char *format, ...);
```

---

### 📚 Descripción
Imprime texto formateado en la **salida estándar** (stdout).  
Esta función permite controlar con precisión la forma en que se muestran diferentes tipos de datos, combinando texto fijo con valores variables según especificadores de formato.

---

### 🧰 Sintaxis y ejemplo
```c
int num = 42;
printf("El valor es: %d\n", num);  // Imprime: El valor es: 42
```


<summary>🔍 Ejemplo de uso en contexto (Mostrar información del shell)</summary>

```c
void print_shell_status(t_shell *shell)
{
    printf("==== Estado del Shell ====\n");
    printf("PID: %d\n", getpid());
    printf("Directorio actual: %s\n", shell->current_dir);
    printf("Último código de salida: %d\n", shell->last_exit_code);
    printf("Variables de entorno: %d\n", count_env_vars(shell->env));
    printf("Comandos en historial: %d\n", shell->history_count);
    printf("=========================\n");
}
```



---

### 💡 Consejos de uso
- **Especificadores de formato comunes**:
  - `%d` o `%i`: Entero decimal
  - `%u`: Entero sin signo
  - `%f`: Punto flotante
  - `%c`: Carácter
  - `%s`: Cadena de caracteres
  - `%p`: Puntero
  - `%x` o `%X`: Hexadecimal
  - `%%`: Símbolo de porcentaje literal
- **Modificadores de formato**:
  - Anchura: `%5d` (mínimo 5 caracteres)
  - Precisión: `%.2f` (2 decimales)
  - Relleno: `%05d` (rellenar con ceros)
  - Justificación: `%-10s` (alineado a la izquierda)
- **Verificar siempre** el número y tipo de argumentos para evitar comportamiento indefinido.
- El número de bytes escritos puede ser menor que el retorno si hay caracteres multibyte.

---

### 🔙 Retorno
Devuelve:
- El **número de caracteres impresos** (excluyendo el nulo final) si tiene éxito.
- Un valor **negativo** si ocurre un error.

---

### ⚠️ Errores
- Discordancia entre especificadores de formato y argumentos proporcionados.
- Fallo al intentar escribir a stdout (por ejemplo, si stdout está redirigido a un archivo y hay un error de disco).
- Desbordamiento de buffer interno si la cadena formateada es demasiado larga.

---

### 🧭 Información adicional

<summary>📎 ¿Por qué se usa en Minishell?</summary>

En Minishell, `printf()` es una función esencial para la **comunicación con el usuario** y se utiliza para múltiples propósitos:

1. **Mensajes informativos y de error**:
   ```c
   if (chdir(path) != 0)
       printf("minishell: cd: %s: %s\n", path, strerror(errno));
   ```

2. **Implementación de comandos built-in**:
   ```c
   // Implementación del comando 'echo'
   int cmd_echo(char **args)
   {
       int i = 1;
       int newline = 1;
       
       if (args[1] && strcmp(args[1], "-n") == 0)
       {
           newline = 0;
           i++;
       }
       
       while (args[i])
       {
           printf("%s", args[i]);
           if (args[i + 1])
               printf(" ");
           i++;
       }
       
       if (newline)
           printf("\n");
           
       return (0);
   }
   ```

3. **Debugging y diagnóstico**:
   ```c
   void debug_command(t_command *cmd)
   {
       printf("DEBUG - Comando: '%s'\n", cmd->name);
       printf("DEBUG - Número de argumentos: %d\n", cmd->argc);
       
       for (int i = 0; i < cmd->argc; i++)
           printf("DEBUG - Arg[%d]: '%s'\n", i, cmd->argv[i]);
           
       if (cmd->input_file)
           printf("DEBUG - Redirección de entrada: %s\n", cmd->input_file);
           
       if (cmd->output_file)
           printf("DEBUG - Redirección de salida: %s (append: %d)\n", 
                  cmd->output_file, cmd->append_output);
   }
   ```

4. **Prompt personalizado**:
   ```c
   void display_prompt(void)
   {
       char cwd[PATH_MAX];
       char hostname[256];
       
       if (getcwd(cwd, sizeof(cwd)) == NULL)
           strcpy(cwd, "???");
           
       if (gethostname(hostname, sizeof(hostname)) != 0)
           strcpy(hostname, "unknown");
           
       // Prompt colorido con información útil
       printf("\033[1;32m%s@%s\033[0m:\033[1;34m%s\033[0m$ ", 
              getenv("USER") ? getenv("USER") : "user",
              hostname,
              cwd);
   }
   ```

`printf()` proporciona una forma flexible y potente de presentar información al usuario, lo que es fundamental para la experiencia de usuario en un shell interactivo.



---