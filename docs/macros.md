# Referencia de Macros Usadas en Minishell

Este documento lista todas las macros utilizadas en el proyecto que no están explícitamente definidas en el subject de 42.

## Macros del Sistema (sys/wait.h)

Estas macros son parte de la biblioteca estándar de POSIX y se utilizan para el manejo de procesos:

### WIFEXITED(status)
- **Descripción**: Devuelve true si el proceso hijo terminó normalmente (con exit() o return desde main)
- **Uso**: Verificar si un proceso terminó correctamente
- **Ejemplo**: `if (WIFEXITED(status)) ...`

### WEXITSTATUS(status)
- **Descripción**: Extrae el código de salida del proceso hijo (0-255)
- **Uso**: Obtener el exit code cuando WIFEXITED es true
- **Ejemplo**: `exit_code = WEXITSTATUS(status);`

### WIFSIGNALED(status)
- **Descripción**: Devuelve true si el proceso hijo terminó por una señal
- **Uso**: Verificar si el proceso fue terminado por una señal (SIGINT, SIGQUIT, etc.)
- **Ejemplo**: `if (WIFSIGNALED(status)) ...`

### WTERMSIG(status)
- **Descripción**: Extrae el número de la señal que terminó el proceso
- **Uso**: Obtener qué señal mató al proceso cuando WIFSIGNALED es true
- **Ejemplo**: `signal_num = WTERMSIG(status);`

### WNOHANG (no usada actualmente)
- **Descripción**: Opción para waitpid() que hace que no bloquee si no hay hijos terminados
- **Uso**: Polling de procesos sin bloquear
- **Ejemplo**: `waitpid(pid, &status, WNOHANG);`

## Macros de File Descriptors (unistd.h)

### STDIN_FILENO
- **Valor**: 0
- **Descripción**: File descriptor estándar para la entrada (stdin)
- **Uso**: Operaciones con entrada estándar

### STDOUT_FILENO
- **Valor**: 1
- **Descripción**: File descriptor estándar para la salida (stdout)
- **Uso**: Operaciones con salida estándar

### STDERR_FILENO
- **Valor**: 2
- **Descripción**: File descriptor estándar para errores (stderr)
- **Uso**: Operaciones con salida de errores

## Macros Definidas en el Proyecto

### Mensajes de Error Generales

```c
ERR_C           "minishell: error: Argument is not -c.\n"
ERR_QUO         "minishell: error: syntax error\n"
ERR_C_ARG       "minishell: error: -c: option requires an argument\n"
ERR_VAR         "minishell: expansion error\n"
ERR_RDI         "minishell: syntax error near unexpected token `newline'\n"
ERR_OLDPWD      "minishell: cd: OLDPWD not set\n"
ERR_HOME        "minishell: cd: HOME not set\n"
ERR_HEREDOC     "here-document delimited by end-of-file (wanted `"
```

### Mensajes de Error de Inicialización

```c
INIT_ERR_VARS       "failed to allocate local_vars\n"
INIT_ERR_ENV        "minishell: fatal error: failed to copy environment\n"
INIT_ERR_HOME       "minishell: fatal error: failed to allocate cd_home\n"
INIT_ERR_ARG_COPY   "minishell: fatal error: failed to copy arguments\n"
INIT_ERR_ALLOC      "minishell: fatal error: failed to alloc arguments\n"
INIT_ERR_MSHLVL     "minishell: fatal error: failed to allocate MSHLVL\n"
```

### Mensajes de Fork Bomb

```c
ERR_FORKBOMB    "FORK BOMB DETECTED!\n"
MSG_APOLOGY     "You must apologize by typing: Sorry!\n"
MSG_CONFIRM     "Now confirm by typing: I'm really sorry!\n"
MSG_FORGIVEN    "\nApology accepted. Please be more careful.\n\n"
MSG_WRONG       "That's not a proper apology. Try again.\n"
```

## Macros de Colores para Readline (colors.h)

Estas macros utilizan secuencias de escape ANSI envueltas en `\001` y `\002` para que readline las ignore al calcular el ancho del prompt:

### Colores Básicos

```c
RL_RST          "\001\033[0m\002"       // Reset
RL_BLD          "\001\033[1m\002"       // Bold
RL_RED          "\001\033[31m\002"      // Rojo
RL_GRN          "\001\033[32m\002"      // Verde
RL_YEL          "\001\033[33m\002"      // Amarillo
RL_BLU          "\001\033[34m\002"      // Azul
RL_MAG          "\001\033[35m\002"      // Magenta
RL_CYN          "\001\033[36m\002"      // Cian
RL_WHT          "\001\033[37m\002"      // Blanco
```

### Colores Especiales

```c
RL_FRKBMB       "\001\033[1;31;5m\002"  // Rojo parpadeante y bold (fork bomb)
```

## Constantes del Sistema

### INT_MAX (limits.h)
- **Valor**: 2147483647 (en sistemas de 32-bit)
- **Descripción**: Valor máximo de un int
- **Uso**: Validación de límites numéricos

### errno (errno.h)
- **Descripción**: Variable global que contiene el último código de error
- **Uso**: Diagnóstico de errores en syscalls

### EACCES (errno.h)
- **Descripción**: Código de error para "Permission denied"
- **Valor**: 13
- **Uso**: Detectar errores de permisos en execve()

## Señales (signal.h)

### SIGINT
- **Valor**: 2
- **Descripción**: Señal de interrupción (Ctrl+C)

### SIGQUIT
- **Valor**: 3
- **Descripción**: Señal de quit (Ctrl+\)

### SIGTERM
- **Valor**: 15
- **Descripción**: Señal de terminación

## Notas de Implementación

1. **Escrituras Atómicas**: Los mensajes de error se construyen completamente en memoria antes de escribirse con una sola llamada a `write()` para evitar race conditions.

2. **Sincronización de Procesos**: Se usa un pipe dummy en `wait_for_stderr_flush()` para asegurar que los mensajes de stderr de procesos hijos se muestren antes de retornar al prompt.

3. **Colores en Readline**: Las secuencias `\001` y `\002` son especiales de readline e indican que el texto entre ellas no debe contarse para el ancho del prompt.

4. **Una Variable Global**: Solo se usa `g_signal_received` (tipo `volatile sig_atomic_t`) para cumplir con el requisito del subject.
