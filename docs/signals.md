
# Señales en Minishell

---

## Introducción

Las señales son interrupciones del sistema operativo que notifican eventos a los procesos. En un shell como minishell, debemos manejar señales para replicar el comportamiento de bash: permitir que el usuario interrumpa comandos sin cerrar el shell, manejar cambios de tamaño de ventana, etc.

**Señales principales en minishell**:
- **SIGINT** (señal 2): Generada con Ctrl+C
- **SIGQUIT** (señal 3): Generada con Ctrl+\
- **SIGWINCH**: Generada al redimensionar la terminal

---

## Conceptos Fundamentales

### Qué es una señal

Una señal es una interrupción asíncrona enviada a un proceso. El sistema operativo suspende momentáneamente el proceso, ejecuta un "signal handler" (manejador) y luego continúa.

**Comportamiento por defecto**:
- SIGINT: Termina el proceso
- SIGQUIT: Termina el proceso con core dump
- SIGWINCH: Ignorada por defecto

### Comportamiento en Bash

Minishell debe replicar cómo bash maneja las señales en diferentes contextos:

**En modo interactivo (esperando input)**:
- Ctrl+C → Nueva línea, nuevo prompt, el shell NO termina
- Ctrl+\ → Ignorado completamente

**Durante heredoc**:
- Ctrl+C → Interrumpe el heredoc, vuelve al prompt
- Ctrl+\ → Ignorado

**Durante ejecución de comandos externos**:
- Ctrl+C → Termina el comando hijo, NO el shell padre
- Ctrl+\ → Termina el comando hijo mostrando "Quit", NO el shell padre

### Signal Handlers

Un signal handler es una función que se ejecuta cuando llega una señal. Podemos:
- **SIG_DFL**: Usar el comportamiento por defecto
- **SIG_IGN**: Ignorar la señal completamente
- **Función personalizada**: Ejecutar nuestro código

**Instalación de handlers**:
Se usa `sigaction()` para instalar handlers de forma robusta.

---

## Variable Global

### g_signal_received

```c
volatile sig_atomic_t g_signal_received = 0;
```

**Propósito**: Es la única variable global permitida. Sirve para comunicar entre el signal handler y el código principal del shell.

**Por qué es necesaria**: Los signal handlers tienen restricciones severas sobre qué pueden hacer (no pueden usar malloc, printf, etc.). Necesitamos una forma segura de informar al código principal que llegó una señal.

**Tipo `volatile sig_atomic_t`**:
- `volatile`: Indica al compilador que el valor puede cambiar en cualquier momento (por un handler), evita optimizaciones incorrectas
- `sig_atomic_t`: Tipo garantizado de ser modificable atómicamente, seguro para usar en signal handlers

**Valores que toma**:
- `0`: No ha llegado ninguna señal
- `SIGINT` (2): Se recibió Ctrl+C
- Otros: Según la señal recibida

**Flujo de uso**:
1. Handler recibe señal → Asigna el número de señal a `g_signal_received`
2. Código principal detecta valor != 0 → Procesa según la señal
3. Código principal resetea a 0

---

## Configuraciones de Señales

### setup_interactive_signals

**Cuándo se usa**: Cuando el shell está en modo interactivo esperando que el usuario escriba un comando.

**Configuración de SIGINT**:
```c
sa_int.sa_handler = handle_sigint;
sa_int.sa_flags = SA_RESTART;
sigaction(SIGINT, &sa_int, NULL);
```

**Handler handle_sigint**:
```c
static void handle_sigint(int sig)
{
    g_signal_received = SIGINT;
    write(STDOUT_FILENO, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}
```

**Qué hace**:
1. Marca la señal en la variable global
2. Escribe un salto de línea (para que aparezca en nueva línea)
3. `rl_on_new_line()`: Informa a readline que estamos en una línea nueva
4. `rl_replace_line("", 0)`: Limpia el buffer de readline
5. `rl_redisplay()`: Redibuja el prompt

**Resultado visual**:
```
[usuario@host ~]$ escribiendo algo^C
[usuario@host ~]$ 
```

El usuario ve un prompt nuevo y limpio, listo para escribir.

**Flag SA_RESTART**: Hace que llamadas de sistema interrumpidas por SIGINT se reinicien automáticamente. Sin esto, `readline()` podría fallar.

**Configuración de SIGQUIT**:
```c
signal(SIGQUIT, SIG_IGN);
```

Se ignora completamente. Ctrl+\ no hace nada.

**Configuración de SIGWINCH**:
```c
sa_winch.sa_handler = handle_sigwinch;
sigaction(SIGWINCH, &sa_winch, NULL);
```

**Handler handle_sigwinch**:
```c
static void handle_sigwinch(int sig)
{
    rl_resize_terminal();
}
```

Cuando el usuario redimensiona la ventana, readline recalcula las dimensiones y el prompt se redibuja correctamente.

---

### setup_heredoc_signals

**Cuándo se usa**: Durante la lectura de un heredoc (operador `<<`).

**Configuración de SIGINT**:
```c
sa_int.sa_handler = handle_heredoc_sigint;
sa_int.sa_flags = 0;  // SIN SA_RESTART
sigaction(SIGINT, &sa_int, NULL);
```

**Handler handle_heredoc_sigint**:
```c
static void handle_heredoc_sigint(int sig)
{
    g_signal_received = SIGINT;
    rl_done = 1;
    write(STDOUT_FILENO, "\n", 1);
    close(STDIN_FILENO);
}
```

**Qué hace**:
1. Marca la señal
2. `rl_done = 1`: Variable de readline que indica "terminar ahora"
3. Escribe salto de línea
4. Cierra stdin para forzar que `readline()` termine

**Por qué cerrar stdin**: Hace que `readline()` retorne NULL inmediatamente, permitiendo salir del loop del heredoc.

**Diferencias con modo interactivo**:
- NO usa SA_RESTART: Queremos que la interrupción sea inmediata
- NO llama a `rl_replace_line()`: No queremos limpiar, solo salir
- Cierra stdin: Fuerza salida abrupta

**Configuración de SIGQUIT**:
También ignorado durante heredoc.

---

### setup_execution_signals

**Cuándo se usa**: En procesos hijos justo antes de ejecutar comandos externos.

**Configuración**:
```c
signal(SIGINT, SIG_DFL);
signal(SIGQUIT, SIG_DFL);
```

**Propósito**: Restaurar el comportamiento por defecto de las señales.

**Por qué**: El proceso hijo hereda los signal handlers del padre. Necesitamos que el comando externo responda normalmente a Ctrl+C y Ctrl+\.

**Resultado**: Si el usuario presiona Ctrl+C mientras un comando se ejecuta, ese comando termina, pero el shell padre continúa.

---

### restore_default_signals

Idéntica a `setup_execution_signals`. Se puede usar en cualquier momento para restaurar comportamiento por defecto.

---

## Flujos de Manejo

### Flujo en Modo Interactivo

**Secuencia completa**:

```
1. Shell configura señales interactivas
   ↓
2. Shell llama a readline(), espera input
   ↓
3. Usuario presiona Ctrl+C
   ↓
4. Sistema envía SIGINT al proceso
   ↓
5. Se ejecuta handle_sigint:
   - g_signal_received = SIGINT
   - Escribe "\n"
   - Limpia y redibuja readline
   ↓
6. readline() continúa esperando
   ↓
7. Usuario escribe comando y presiona Enter
   ↓
8. readline() retorna el input
   ↓
9. Función ft_signal() verifica g_signal_received
   - Si es SIGINT: mini->exit_sts = 130
   - Resetea g_signal_received = 0
   ↓
10. Shell procesa el comando
```

**Dónde se verifica la señal**:
```c
void ft_signal(t_mini *mini)
{
    if (!mini->input)
    {
        // EOF (Ctrl+D)
        cleanup_mini(mini);
        write(STDOUT_FILENO, "exit\n", 5);
        exit(0);
    }
    if (g_signal_received == SIGINT)
    {
        mini->exit_sts = 130;
        g_signal_received = 0;
    }
}
```

Se llama después de `readline()`, antes de procesar el comando.

---

### Flujo Durante Heredoc

**Secuencia**:

```
1. Shell detecta operador <<
   ↓
2. Llama a setup_heredoc_signals()
   ↓
3. Loop lee líneas con readline("> ")
   ↓
4. Usuario presiona Ctrl+C
   ↓
5. Se ejecuta handle_heredoc_sigint:
   - g_signal_received = SIGINT
   - rl_done = 1
   - Cierra stdin
   ↓
6. readline() retorna NULL (por stdin cerrado)
   ↓
7. Loop detecta NULL o g_signal_received
   ↓
8. Se sale del loop de heredoc
   ↓
9. Verifica g_signal_received:
   - Si es SIGINT: mini->exit_sts = 130, aborta comando
   ↓
10. Llama a setup_interactive_signals()
    ↓
11. Vuelve al prompt normal
```

**Backup de stdin**:
```c
int stdin_backup = dup(STDIN_FILENO);
// ... proceso heredoc ...
dup2(stdin_backup, STDIN_FILENO);  // Restaura stdin si se cerró
close(stdin_backup);
```

Si se cierra stdin para interrumpir, se restaura después para que el shell pueda seguir leyendo.

---

### Flujo Durante Ejecución de Comandos

**En el padre (shell)**:

```
1. Shell mantiene señales interactivas
   ↓
2. fork() crea proceso hijo
   ↓
3. Shell llama a waitpid(pid)
   ↓
4. Si llega SIGINT durante waitpid:
   - Handler se ejecuta
   - waitpid puede ser interrumpido
   - Shell NO termina
   ↓
5. Hijo termina (por la señal o normalmente)
   ↓
6. waitpid() retorna
   ↓
7. Shell obtiene exit status del hijo
   ↓
8. Shell continúa normalmente
```

**En el hijo (comando)**:

```
1. Después del fork(), antes del execve()
   ↓
2. Llama a setup_execution_signals()
   ↓
3. Señales en modo SIG_DFL
   ↓
4. Ejecuta execve()
   ↓
5. Si llega SIGINT o SIGQUIT:
   - Comportamiento por defecto: termina
   - NO afecta al padre
```

**Resultado**: El usuario puede interrumpir comandos largos sin cerrar el shell.

---

## Gestión del Exit Status

### Valores Especiales

El exit status es un número entre 0-255 que indica cómo terminó el último comando.

**Convención para señales**:
```
Exit Status = 128 + Número de señal
```

**Valores importantes**:
- `0`: Éxito
- `1`: Error genérico
- `2`: Error de sintaxis
- `130`: Terminado por SIGINT (128 + 2)
- `131`: Terminado por SIGQUIT (128 + 3)

### Actualización en Diferentes Contextos

**Después de readline (SIGINT)**:
```c
if (g_signal_received == SIGINT)
{
    mini->exit_sts = 130;
    g_signal_received = 0;
}
```

**Después de heredoc interrumpido**:
```c
if (g_signal_received == SIGINT)
{
    mini->exit_sts = 130;
    return 1;  // Aborta el comando
}
```

**Después de comando ejecutado**:
```c
waitpid(pid, &status, 0);

if (WIFEXITED(status))
{
    // Terminó normalmente
    mini->exit_sts = WEXITSTATUS(status);
}
else if (WIFSIGNALED(status))
{
    // Terminó por señal
    mini->exit_sts = 128 + WTERMSIG(status);
}
```

**En builtins**:
Cada builtin actualiza directamente:
```c
void builtin_echo(t_mini *mini)
{
    // ... código ...
    mini->exit_sts = 0;  // Éxito
}
```

### Preservación del Exit Status

El exit status solo cambia cuando:
- Se ejecuta un comando (builtin o externo)
- Se recibe SIGINT
- Hay error de sintaxis

**Se mantiene cuando**:
- Se escribe línea vacía
- Se escribe solo espacios
- Se presiona Enter sin escribir nada

---

## Casos Especiales

### Interacción con Readline

Readline tiene su propio sistema de señales que puede interferir. Por eso usamos funciones específicas:

**rl_on_new_line()**:
Informa a readline que el cursor está en una nueva línea. Necesario después de escribir manualmente con `write()`.

**rl_replace_line(string, clear)**:
Reemplaza el contenido del buffer de readline. Usado para limpiar la línea después de Ctrl+C.

**rl_redisplay()**:
Fuerza a readline a redibujar el prompt y la línea. Hace que el usuario vea el prompt limpio.

**rl_done**:
Variable interna de readline. Al setearla a 1, el próximo chequeo del loop interno hace que `readline()` retorne inmediatamente.

**rl_resize_terminal()**:
Recalcula las dimensiones de la terminal. Llama internamente a funciones que obtienen filas y columnas.

### EOF (Ctrl+D)

Aunque no es una señal, se maneja en el mismo contexto:

```c
char *input = readline(prompt);

if (!input)  // EOF
{
    cleanup_mini(mini);
    write(STDOUT_FILENO, "exit\n", 5);
    exit(mini->exit_sts);
}
```

**Comportamiento**: Ctrl+D cierra el input, `readline()` retorna NULL, el shell termina limpiamente.

**Diferencia con SIGINT**: SIGINT limpia la línea y continúa, EOF termina el shell.

---

### Fork Bomb Detection

Caso especial cuando se detecta patrón de fork bomb:

```c
void handle_fork_bomb(t_mini *mini)
{
    signal(SIGINT, SIG_IGN);  // Ignora Ctrl+C temporalmente
    
    // ... modo de disculpa forzada ...
    
    setup_interactive_signals();  // Restaura señales
}
```

**Durante el modo disculpa**:
- SIGINT está en SIG_IGN
- Usuario no puede interrumpir con Ctrl+C
- Debe escribir las disculpas correctas
- Después se restauran señales normales

**Si el usuario hace Ctrl+D durante disculpa**:
```c
if (!input)  // EOF durante disculpa
{
    cleanup_mini(mini);
    write(STDOUT_FILENO, "exit... COWARD!\n", ...);
    exit(42);
}
```

Mensaje especial y exit status 42.

---

### Pipes y Señales

En un pipeline, cada comando es un proceso independiente:

**Configuración**:
```
Padre (shell):
  - Señales interactivas
  - NO termina con SIGINT

Hijo 1 (cmd1):
  - setup_execution_signals() antes de execve()
  - Puede terminar con SIGINT

Hijo 2 (cmd2):
  - setup_execution_signals() antes de execve()
  - Puede terminar con SIGINT
```

**Si el usuario presiona Ctrl+C**:
- SIGINT se envía a todo el grupo de procesos
- Todos los hijos reciben la señal
- Todos terminan
- El padre detecta que terminaron por señal
- El exit status del último hijo se guarda

**Ejemplo**:
```bash
sleep 10 | sleep 10 | sleep 10
^C
```

Los tres `sleep` reciben SIGINT y terminan. El shell obtiene exit status 130 del último.

---

### Heredoc con Pipes

Caso especial: heredoc seguido de pipe:

```bash
cat << EOF | grep hello
```

**Secuencia**:
1. Se procesa heredoc primero (antes de hacer fork)
2. Si se interrumpe con Ctrl+C:
   - Heredoc se aborta
   - TODO el comando se aborta (no se ejecuta el pipe)
3. Si se completa:
   - El contenido se guarda
   - Se ejecuta el pipeline usando ese contenido

**Por qué antes del fork**: Los heredocs se procesan en el padre porque necesitan stdin del terminal. Si se hiciera en el hijo, los pipes interferirían.

---

### Señales Durante Builtins

Los builtins se ejecutan en el proceso padre, por lo que:

**Sin pipes**:
```bash
cd /some/path
```

Si el builtin tarda (por ejemplo, esperando un lock), el usuario puede:
- Presionar Ctrl+C → Handler se ejecuta, pero el builtin debe chequear
- En la práctica, la mayoría de builtins son instantáneos

**En pipes**:
```bash
echo hello | cd /tmp | grep hello
```

El `cd` se ejecuta en un hijo (por el pipe), por lo que puede recibir señales normalmente.

---

### Restauración de stdin

Situación especial cuando se cierra stdin en heredoc:

```c
int stdin_backup = dup(STDIN_FILENO);
setup_heredoc_signals();

// ... lectura de heredoc ...

if (g_signal_received == SIGINT)
{
    dup2(stdin_backup, STDIN_FILENO);  // Restaura
    g_signal_received = 0;
}

close(stdin_backup);
setup_interactive_signals();
```

**Por qué**: El handler de heredoc cierra stdin para interrumpir. Pero el shell necesita stdin para continuar leyendo comandos. Se hace backup antes, se restaura después.

---

## Resumen Visual

```
CONFIGURACIONES:

┌─────────────────────────────────────────────┐
│        setup_interactive_signals()          │
├─────────────────────────────────────────────┤
│ SIGINT  → handle_sigint (limpia, redibuja) │
│ SIGQUIT → SIG_IGN (ignorar)                 │
│ SIGWINCH → handle_sigwinch (redimensiona)   │
│                                             │
│ Usado: Modo interactivo, esperando input   │
└─────────────────────────────────────────────┘

┌─────────────────────────────────────────────┐
│         setup_heredoc_signals()             │
├─────────────────────────────────────────────┤
│ SIGINT  → handle_heredoc_sigint (aborta)   │
│ SIGQUIT → SIG_IGN (ignorar)                 │
│                                             │
│ Usado: Durante lectura de heredoc          │
└─────────────────────────────────────────────┘

┌─────────────────────────────────────────────┐
│        setup_execution_signals()            │
├─────────────────────────────────────────────┤
│ SIGINT  → SIG_DFL (default: terminar)      │
│ SIGQUIT → SIG_DFL (default: terminar+core) │
│                                             │
│ Usado: En hijos antes de execve()          │
└─────────────────────────────────────────────┘


FLUJO DE g_signal_received:

Handler recibe señal
      ↓
g_signal_received = SIGINT
      ↓
Código principal detecta
      ↓
Procesa según contexto
      ↓
g_signal_received = 0 (reset)


EXIT STATUS:

Comando normal → 0-255 (valor de retorno)
Ctrl+C durante comando → 130 (128 + 2)
Ctrl+\ durante comando → 131 (128 + 3)
Ctrl+C en prompt → 130 (sin ejecutar comando)
```
