
# EXECUTOR EN MINISHELL - LÓGICA FUNDAMENTAL

---

## VISIÓN GENERAL

### Flujo de Ejecución

```
parse() completo
    ↓
built_ins()?
├─ TRUE: Ya ejecutado, return
└─ FALSE: Continuar ↓

executor()
├─ Verificar comandos válidos
├─ ¿Tiene pipes?
│  ├─ SÍ: Ejecutar pipeline
│  └─ NO: Ejecutar simple
└─ Esperar procesos hijos
```

### Entrada del Executor

```
mini->cmds apunta a lista enlazada:

cmd1 → next → cmd2 → next → cmd3 → NULL

Cada cmd tiene:
├─ tokens = ["comando", "arg1", "arg2", NULL]
├─ redirs → lista de redirecciones
└─ next → siguiente comando
```

---

## DETECCIÓN DE COMANDOS

### Lógica Inicial

```
Antes de ejecutar, verificar:

1. ¿El comando está vacío?
   tokens[0] == NULL o tokens[0][0] == '\0'
   → Salir sin error

2. ¿Hay múltiples comandos?
   Contar nodos de la lista enlazada

3. ¿Hay redirecciones?
   cmd->redirs != NULL
```

### Contar Comandos

```c
int ft_lstsize(t_cmd *lst)
{
    int count = 0;
    t_cmd *curr = lst;
    
    while (curr)
    {
        count++;
        curr = curr->next;
    }
    
    return count;
}
```

**Decisión:**
```
count == 1 → Comando simple
count > 1  → Pipeline
```

---

## COMANDO SIMPLE

### Sin Redirecciones

**Lógica:**
```
1. fork()
2. Hijo: buscar ejecutable y execve()
3. Padre: waitpid()
```

### Con Redirecciones

**Lógica:**
```
1. Procesar redirecciones (abrir archivos)
2. fork()
3. Hijo:
   ├─ Aplicar redirecciones (dup2)
   ├─ Cerrar fds
   └─ execve()
4. Padre: waitpid()
```

### Flujo Detallado

```
pid = fork()

if (pid == 0)  // Hijo
{
    setup_execution_signals()  // SIGINT y SIGQUIT default
    
    ¿Hay redirecciones?
    ├─ SÍ: Aplicar cada una (dup2)
    └─ NO: Continuar
    
    path = buscar_ejecutable(tokens[0], env)
    
    ¿Encontrado?
    ├─ SÍ: execve(path, tokens, env)
    └─ NO: Error "command not found"
               exit(127)
}
else  // Padre
{
    waitpid(pid, &status, 0)
    mini->exit_sts = WEXITSTATUS(status)
}
```

### Buscar Ejecutable

**Lógica:**
```
¿tokens[0] contiene '/' o '.' ?
├─ SÍ: Es ruta (absoluta o relativa)
│      ¿Archivo existe y es ejecutable?
│      ├─ SÍ: return tokens[0]
│      └─ NO: Error, exit(127)
│
└─ NO: Buscar en PATH

Buscar en PATH:
    Obtener PATH de env
    Dividir por ':'
    Para cada directorio:
        full_path = directorio + "/" + tokens[0]
        ¿Existe y es ejecutable?
        ├─ SÍ: return full_path
        └─ NO: Continuar
    
    return NULL (no encontrado)
```

**Ejemplo:**
```bash
# Caso 1: Ruta relativa
./minishell  → buscar "./minishell" directamente

# Caso 2: Ruta absoluta
/bin/ls      → buscar "/bin/ls" directamente

# Caso 3: Buscar en PATH
ls           → buscar en PATH
PATH=/usr/bin:/bin:/usr/local/bin

Probar:
    /usr/bin/ls       → ¿existe? NO
    /bin/ls           → ¿existe? SÍ → return "/bin/ls"
```

---

## REDIRECCIONES

### Tipos y Orden

```
< infile       → in_redir = 1  (O_RDONLY)
<< EOF         → in_redir = 2  (heredoc)
> outfile      → out_redir = 1 (O_WRONLY | O_CREAT | O_TRUNC)
>> outfile     → out_redir = 2 (O_WRONLY | O_CREAT | O_APPEND)
```

### Lógica de Aplicación

**Orden de procesamiento:**
```
Procesar redirecciones en el ORDEN en que aparecen:

cat < file1 < file2
    ↓
1. Abrir file1 para lectura
2. Abrir file2 para lectura  ← Este prevalece

cat > out1 > out2
    ↓
1. Crear/truncar out1
2. Crear/truncar out2  ← Este prevalece
```

**Proceso:**
```
Para cada redirección en orden:

    IN (< o <<):
        fd = open(target, O_RDONLY)
        Si falla: error, exit(1)
        dup2(fd, STDIN_FILENO)
        close(fd)
    
    OUT (> o >>):
        flags = O_WRONLY | O_CREAT
        Si out_redir == 1: flags |= O_TRUNC
        Si out_redir == 2: flags |= O_APPEND
        
        fd = open(target, flags, 0644)
        Si falla: error, exit(1)
        dup2(fd, STDOUT_FILENO)
        close(fd)
```

### Momento de Aplicación

**Timing:**
```
1. Procesar heredocs ANTES de fork (en padre)
2. Abrir archivos DESPUÉS de fork (en hijo)
3. Aplicar dup2 ANTES de execve (en hijo)
```

**¿Por qué?**
```
Heredoc → Necesita leer de stdin del shell
          Si se hace en hijo, bloquearía hijo

Archivos → Si se abren en padre, afectarían padre
           Se abren en hijo para no modificar padre
```

### Errores en Redirecciones

```
¿Archivo no existe (input)?
├─ Error: "No such file or directory"
└─ exit(1), NO ejecutar comando

¿Sin permisos (input/output)?
├─ Error: "Permission denied"
└─ exit(1), NO ejecutar comando

¿Directorio en vez de archivo?
├─ Error: "Is a directory"
└─ exit(1), NO ejecutar comando
```

---

## PIPES

### Lógica General

```
cmd1 | cmd2 | cmd3

Se necesitan:
    n comandos = 3
    n-1 pipes = 2

pipes[0]  → cmd1 escribe, cmd2 lee
pipes[1]  → cmd2 escribe, cmd3 lee
```

### Estructura de Pipes

```
pipe[0] = fd de lectura
pipe[1] = fd de escritura

cmd1 → | → cmd2 → | → cmd3
       ↑         ↑
    pipe[0]  pipe[1]
```

### Creación de Pipes

```
n_pipes = num_comandos - 1

pipes = malloc(n_pipes * 2 * sizeof(int))

Para i = 0 hasta n_pipes-1:
    pipe(pipes + i*2)
    
Resultado:
pipes[0] = pipe0_lectura
pipes[1] = pipe0_escritura
pipes[2] = pipe1_lectura
pipes[3] = pipe1_escritura
...
```

### Ejecución de Pipeline

**Lógica:**
```
Para cada comando (índice i):

    pid = fork()
    
    if (pid == 0)  // Hijo
    {
        ¿Es primer comando (i == 0)?
        └─ NO: dup2(pipes[(i-1)*2], STDIN_FILENO)
               (Leer del pipe anterior)
        
        ¿Es último comando (i == n-1)?
        └─ NO: dup2(pipes[i*2 + 1], STDOUT_FILENO)
               (Escribir al pipe siguiente)
        
        Cerrar TODOS los pipes en hijo
        
        Aplicar redirecciones propias del comando
        
        execve(...)
    }

Cerrar TODOS los pipes en padre

Para cada hijo:
    waitpid(pid, &status, 0)

exit_sts = status del ÚLTIMO comando
```

### Ejemplo Visual

```bash
Input: cat file.txt | grep test | wc -l

Comandos:
├─ cmd0: ["cat", "file.txt"]
├─ cmd1: ["grep", "test"]
└─ cmd2: ["wc", "-l"]

Pipes:
pipe[0] → [r0, w0]
pipe[1] → [r1, w1]

Hijo 0 (cat):
├─ stdin = STDIN original (leer file.txt)
├─ stdout = w0  ← Escribir a pipe[0]
└─ Cerrar: r0, w0, r1, w1

Hijo 1 (grep):
├─ stdin = r0   ← Leer de pipe[0]
├─ stdout = w1  ← Escribir a pipe[1]
└─ Cerrar: r0, w0, r1, w1

Hijo 2 (wc):
├─ stdin = r1   ← Leer de pipe[1]
├─ stdout = STDOUT original
└─ Cerrar: r0, w0, r1, w1

Padre:
└─ Cerrar: r0, w0, r1, w1
   Esperar los 3 hijos
```

### Cerrar FDs Correctamente

**CRÍTICO:**
```
El padre debe cerrar TODOS los pipes
└─ Si no, los hijos esperarán indefinidamente

Cada hijo debe cerrar TODOS los pipes
├─ Incluso los que usa (después de dup2)
└─ Si no, el programa no terminará
```

**Código:**
```c
// En cada hijo, después de dup2
for (int j = 0; j < n_pipes * 2; j++)
    close(pipes[j]);

// En el padre, antes de waitpid
for (int j = 0; j < n_pipes * 2; j++)
    close(pipes[j]);
```

---

## HEREDOC

### Lógica General

```
<< EOF
línea1
línea2
EOF

1. Leer líneas hasta encontrar "EOF"
2. Guardar en archivo temporal o pipe
3. Usar como stdin del comando
```

### Procesamiento

**¿Cuándo procesar?**
```
ANTES de fork()
└─ El padre lee de su stdin
   El hijo recibirá el contenido
```

**¿Cómo guardar?**
```
Opción 1: Pipe anónimo
├─ pipe(fd)
├─ Leer líneas y escribir a fd[1]
├─ En hijo: dup2(fd[0], STDIN_FILENO)
└─ Cerrar ambos extremos después

Opción 2: Archivo temporal
├─ fd = open(".heredoc_tmp", O_WRONLY|O_CREAT|O_TRUNC)
├─ Escribir líneas
├─ close(fd)
├─ En hijo: fd = open(".heredoc_tmp", O_RDONLY)
│           dup2(fd, STDIN_FILENO)
│           close(fd)
└─ unlink(".heredoc_tmp")
```

### Expansión de Variables

**Lógica:**
```
¿redir->hd_expand == 1?
├─ SÍ: Expandir $VAR en cada línea
└─ NO: Dejar todo literal

Ejemplos:
<< EOF
$USER    → Si hd_expand=1: expandir a "john"
         → Si hd_expand=0: dejar "$USER"
EOF
```

### Flujo Completo

```
Para cada comando:
    Para cada redirección:
        ¿Es heredoc (in_redir == 2)?
        ├─ SÍ:
        │   pipe(hd_fd)
        │   
        │   while (1):
        │       línea = readline("> ")
        │       
        │       Si línea == delimitador:
        │           break
        │       
        │       Si hd_expand:
        │           expandir_variables(línea)
        │       
        │       write(hd_fd[1], línea)
        │       write(hd_fd[1], "\n")
        │   
        │   close(hd_fd[1])
        │   Guardar hd_fd[0] para usar en hijo
        │
        └─ NO: Continuar

En el hijo:
    Si hay heredoc:
        dup2(hd_fd[0], STDIN_FILENO)
        close(hd_fd[0])
```

### Múltiples Heredocs

```bash
cat << EOF1 << EOF2
contenido1
EOF1
contenido2
EOF2

Regla: El ÚLTIMO prevalece

Procesamiento:
1. Leer hasta EOF1, guardar
2. Leer hasta EOF2, guardar  ← Este se usa
3. En hijo: stdin = contenido de EOF2
```

---

## SEÑALES

### Estados del Shell

```
INTERACTIVO (esperando input):
    SIGINT (Ctrl+C)  → Imprimir \n, nueva línea
    SIGQUIT (Ctrl+\) → Ignorar

EJECUTANDO:
    SIGINT  → Terminar hijo, padre continúa
    SIGQUIT → Terminar hijo con core dump
```

### Configuración

**En el padre (shell):**
```c
struct sigaction sa;

sa.sa_handler = handle_sigint;
sa.sa_flags = SA_RESTART;
sigaction(SIGINT, &sa, NULL);

signal(SIGQUIT, SIG_IGN);
```

**En el hijo (comando):**
```c
signal(SIGINT, SIG_DFL);
signal(SIGQUIT, SIG_DFL);
```

### Manejo de SIGINT

```
Usuario presiona Ctrl+C:

¿En qué estado?
├─ INTERACTIVO:
│   handle_sigint():
│       write(1, "\n", 1)
│       rl_on_new_line()
│       rl_replace_line("", 0)
│       rl_redisplay()
│       g_signal_received = SIGINT
│   
│   En el loop principal:
│       Si g_signal_received:
│           exit_sts = 130
│
└─ EJECUTANDO:
    Hijo recibe SIGINT y termina
    Padre en waitpid continúa
    WTERMSIG(status) == SIGINT
    exit_sts = 128 + SIGINT = 130
```

---

## VERIFICACIONES NECESARIAS

### Antes de Ejecutar

```
1. ¿Comando vacío?
   └─ Salir sin error

2. ¿Es built-in?
   └─ Ya ejecutado, return

3. ¿Tokens válidos?
   └─ tokens != NULL && tokens[0] != NULL

4. ¿Existen archivos de redirección?
   └─ Verificar antes de fork

5. ¿Permisos correctos?
   └─ access() para verificar
```

### Durante Ejecución

```
1. ¿fork() falló?
   └─ Error crítico, salir

2. ¿pipe() falló?
   └─ Error, liberar recursos

3. ¿open() falló?
   └─ Error, mensaje, exit(1)

4. ¿execve() falló?
   └─ Error "command not found", exit(127)
```

### Después de Ejecutar

```
1. Cerrar todos los FDs abiertos
   └─ Pipes, archivos de redirección

2. Esperar todos los hijos
   └─ waitpid() para cada pid

3. Liberar memoria
   └─ Paths, arrays, estructuras temporales

4. Actualizar exit_sts
   └─ Status del último comando en pipeline
```

### Casos Especiales

**Comando no encontrado:**
```
./noexiste    → "No such file or directory", exit(127)
noexiste      → "command not found", exit(127)
```

**Sin permisos:**
```
./file.txt    → "Permission denied", exit(126)
```

**Es directorio:**
```
./carpeta     → "Is a directory", exit(126)
```

**Pipe vacío:**
```
cat | | grep   → Syntax error (detectar en lexer)
```

**Redirección sin archivo:**
```
cat <          → Syntax error (detectar en lexer)
cat > > file   → Syntax error
```

---

## RESUMEN FLUJO COMPLETO

### Comando Simple

```
1. Verificar que no es built-in
2. Procesar heredocs (si existen)
3. fork()
4. Hijo:
   ├─ Restaurar señales default
   ├─ Aplicar redirecciones (dup2)
   ├─ Cerrar FDs
   ├─ Buscar ejecutable
   └─ execve()
5. Padre:
   ├─ Cerrar FDs de heredocs
   ├─ waitpid()
   └─ Actualizar exit_sts
```

### Pipeline

```
1. Verificar comandos
2. Procesar heredocs de todos los comandos
3. Crear pipes (n-1)
4. Para cada comando:
   ├─ fork()
   ├─ Hijo:
   │  ├─ Configurar pipes según posición
   │  ├─ Cerrar todos los pipes
   │  ├─ Aplicar redirecciones propias
   │  └─ execve()
   └─ Guardar pid
5. Padre:
   ├─ Cerrar todos los pipes
   ├─ waitpid() para cada hijo
   └─ exit_sts = status del último
```

### Prioridad de FDs

```
1. Redirecciones de input/output
   ↓
2. Pipes (si existen)
   ↓
3. Redirecciones propias del comando

Regla: La última redirección prevalece
```

---

## ERRORES COMUNES A EVITAR

### 1. No cerrar pipes

```
✗ MALO:
    fork()
    // No cerrar pipes
    → Los hijos esperan indefinidamente

✓ BUENO:
    fork()
    // Cerrar TODOS los pipes en padre e hijo
```

### 2. Cerrar FDs antes de dup2

```
✗ MALO:
    close(fd)
    dup2(fd, STDIN_FILENO)  // fd ya cerrado!

✓ BUENO:
    dup2(fd, STDIN_FILENO)
    close(fd)
```

### 3. No esperar hijos

```
✗ MALO:
    fork()
    // No waitpid()
    → Procesos zombies

✓ BUENO:
    fork()
    waitpid(pid, &status, 0)
```

### 4. Aplicar redirecciones en orden incorrecto

```
✗ MALO:
    Aplicar pipes después de redirecciones propias
    → Las redirecciones no funcionan

✓ BUENO:
    1. Configurar pipes
    2. Aplicar redirecciones del comando
       (prevalecen sobre pipes)
```

### 5. Procesar heredocs después de fork

```
✗ MALO:
    fork()
    if (hijo)
        leer heredoc  // Bloquea hijo

✓ BUENO:
    leer heredoc  // En padre
    fork()
    if (hijo)
        usar heredoc
```

---

## FUNCIONES CLAVE

### Funciones de Sistema Necesarias

```c
// Proceso
fork()      → Crear proceso hijo
execve()    → Ejecutar programa
exit()      → Terminar proceso
wait()      → Esperar hijo (cualquiera)
waitpid()   → Esperar hijo específico

// File descriptors
pipe()      → Crear pipe
dup2()      → Duplicar FD
close()     → Cerrar FD
open()      → Abrir archivo

// Sistema de archivos
access()    → Verificar permisos
stat()      → Info de archivo
getcwd()    → Directorio actual
chdir()     → Cambiar directorio

// Señales
signal()    → Configurar handler simple
sigaction() → Configurar handler avanzado
kill()      → Enviar señal

// Entorno
getenv()    → Obtener variable
```

### Comprobaciones con access()

```c
// Verificar que existe
if (access(path, F_OK) != 0)
    // No existe

// Verificar que es ejecutable
if (access(path, X_OK) != 0)
    // No es ejecutable

// Verificar permisos de lectura
if (access(path, R_OK) != 0)
    // No se puede leer
```

### Manejo de status

```c
int status;
waitpid(pid, &status, 0);

if (WIFEXITED(status))
    exit_code = WEXITSTATUS(status);
else if (WIFSIGNALED(status))
    exit_code = 128 + WTERMSIG(status);
```
