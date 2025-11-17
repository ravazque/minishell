
# Arquitectura de Minishell

---

## Estructuras de Datos

### t_mini (Contexto Principal)
La estructura principal que mantiene todo el estado del shell:

```c
typedef struct s_mini
{
    char    *prompt;        // Prompt dinámico mostrado al usuario
    char    *input;         // Línea de input del usuario
    char    *pwd;           // Directorio de trabajo actual
    int     argc;           // Número de argumentos al iniciar
    char    **argv;         // Argumentos al iniciar
    char    **env;          // Variables de entorno
    char    **local_vars;   // Variables locales (assignments)
    int     exit_sts;       // Código de salida del último comando
    int     apology_mode;   // Flag para detección de fork bomb
    char    *cd_home;       // Cache del directorio HOME
    t_cmd   *cmds;          // Lista enlazada de comandos
}   t_mini;
```

**Puntos clave:**
- `env` y `local_vars` están separados: las variables locales solo existen en el proceso actual
- `exit_sts` se actualiza tras cada comando ejecutado
- `cd_home` es un cache para optimizar `cd` sin argumentos

### t_token_part (Partes de Token)
Representa fragmentos individuales dentro de un token, preservando información sobre comillas:

```c
typedef struct s_token_part
{
    char                *content;    // Contenido del fragmento
    int                 is_squote;   // 1 si estaba en comillas simples
    int                 is_dquote;   // 1 si estaba en comillas dobles
    struct s_token_part *next;       // Siguiente parte
}   t_token_part;
```

**Propósito:** Esta estructura permite distinguir entre `echo "HOME"` (literal) y `echo $HOME` (expandible).

### t_token (Token)
Unidad básica léxica del input:

```c
typedef struct s_token
{
    char            *raw;            // Contenido completo del token
    int             is_squote;       // Token completo entre comillas simples
    int             is_dquote;       // Token completo entre comillas dobles
    int             is_assignment;   // Es una asignación (VAR=value)
    t_token_part    *parts;          // Lista de partes del token
    struct s_token  *next;           // Siguiente token
}   t_token;
```

### t_redir (Redirección)
Representa una redirección de entrada o salida:

```c
typedef struct s_redir
{
    char            *target;     // Archivo destino o delimitador (heredoc)
    int             hd_expand;   // 1 si el heredoc debe expandir variables
    int             in_redir;    // 0=none, 1=<, 2=<<
    int             out_redir;   // 0=none, 1=>, 2=>>
    int             fd;          // File descriptor abierto
    t_heredoc_data  *hd_data;    // Datos del heredoc (si aplica)
    struct s_redir  *next;       // Siguiente redirección
}   t_redir;
```

**Decisión de diseño:**
- `hd_expand=0` cuando el delimitador del heredoc tiene comillas (`<<"EOF"`)
- `hd_expand=1` cuando el delimitador es literal (`<<EOF`)

### t_cmd (Comando)
Representa un comando individual en un pipeline:

```c
typedef struct s_cmd
{
    t_redir         *redirs;    // Lista de redirecciones
    t_token         *tokn;      // Tokens sin procesar
    char            **tokens;   // Array final de argumentos
    struct s_cmd    *next;      // Siguiente comando (pipe)
}   t_cmd;
```

### t_exec (Contexto de Ejecución)
Gestiona la ejecución de pipelines:

```c
typedef struct s_exec
{
    int     *pipe_fds;      // Array de file descriptors de pipes
    int     n_pipes;        // Número de pipes (n_cmds - 1)
    pid_t   *pids;          // Array de PIDs de procesos hijo
    int     n_cmds;         // Número de comandos
    int     stdin_backup;   // Backup de STDIN
    int     stdout_backup;  // Backup de STDOUT
}   t_exec;
```

### t_heredoc_data (Datos de Heredoc)
```c
typedef struct s_heredoc_data
{
    int     pipe_fd[2];     // Pipe para escribir el heredoc
    char    **lines;        // Líneas del heredoc almacenadas
}   t_heredoc_data;
```

---

## Pipeline de Ejecución

### 1. Loop Principal (src/aux_minishell/)

**Archivos principales:**
- `loop.c` - Bucle principal del shell
- `init.c` - Inicialización de estructuras
- `mshlvl.c` - Gestión de SHLVL
- `underscore.c` - Gestión de variable $_
- `error.c` - Manejo de errores

```
loop (aux_minishell/loop.c)
  ↓
readline → ft_signal → parse → heredocs → executor
```

### 2. Fases de Procesamiento (src/parse/)

**Archivos de control:**
- `parse.c` - Orquestador de las fases de parsing
- `parse_assign.c` - Procesamiento de assignments (VAR=value)
- `parse_utils.c` - Utilidades de parsing

```
input → tokenizer → lexer → assignments → expander → heredocs → executor
```


---

## Sistema de Parsing

### Tokenizer (parse/tokenizer/)

**Función:** Convierte el input en una lista de tokens preservando la información de comillas.

**Archivos principales:**
- `tokenizer.c` - Lógica principal del tokenizador
- `tokenizer_handlers.c` - Manejadores para diferentes tipos de caracteres
- `tokenizer_helpers.c` - Funciones auxiliares
- `tokenizer_parts.c` - Creación y gestión de `t_token_part`
- `tokenizer_utils.c` - Utilidades varias

**Algoritmo:**
1. Recorre el input carácter por carácter
2. Identifica operadores (`|`, `<`, `>`, `<<`, `>>`)
3. Maneja comillas simples y dobles
4. Crea `t_token_part` para cada fragmento

**Ejemplo de tokenización:**

```
Input:  echo "$HOME" '$USER' 'literal'
                ↓
Tokens:
  - Token: "echo"
    parts: [{"echo", sq=0, dq=0}]

  - Token: "$HOME"
    parts: [{"$HOME", sq=0, dq=1}]    // dq=1 indica comillas dobles

  - Token: "$USER"
    parts: [{"$USER", sq=1, dq=0}]    // sq=1 indica comillas simples

  - Token: "literal"
    parts: [{"literal", sq=1, dq=0}]
```

**Puntos críticos:**
- Los operadores siempre son tokens independientes
- Las comillas se eliminan pero su presencia se registra en `is_squote`/`is_dquote`
- Los espacios dentro de comillas se preservan

### Lexer (parse/lexer/)

**Función:** Agrupa tokens en comandos y extrae redirecciones.

**Archivos principales:**
- `lexer.c` - Lógica principal del lexer
- `lexer_classify.c` - Clasificación de tokens (pipes, redirecciones)
- `lexer_cmd_create.c` - Creación de estructuras `t_cmd`
- `lexer_pipe_split.c` - División de comandos por pipes
- `lexer_pipe_valid.c` - Validación de sintaxis de pipes
- `lexer_quotes.c` - Manejo de comillas
- `lexer_redir.c` - Creación de redirecciones
- `lexer_redir_proc.c` - Procesamiento de redirecciones
- `lexer_token.c` - Creación y manipulación de tokens
- `lexer_token_arr.c` - Conversión de tokens a arrays
- `lexer_utils.c` - Utilidades del lexer

**Proceso:**
1. **Validación de sintaxis de pipes:** Detecta pipes al inicio/final o consecutivos
2. **División por pipes:** Separa comandos en `t_cmd` individuales
3. **Extracción de redirecciones:**
   - Identifica tokens de redirección (`<`, `>`, `<<`, `>>`)
   - Valida que tengan un target válido
   - Los elimina de la lista de tokens y los mueve a `cmd->redirs`
4. **Marcado de assignments:** Identifica tokens `VAR=value` antes del primer comando
5. **Conversión a array:** Convierte `t_token` a `char **tokens`

**Ejemplo de procesamiento:**

```
Input: cat < input.txt | grep "test" > output.txt

Después del lexer:
  cmd[0]:
    tokens = ["cat"]
    redirs = [<input.txt]

  cmd[1]:
    tokens = ["grep", "test"]
    redirs = [>output.txt]
```

---

## Sistema de Expansión

### Lógica de Expansión

El sistema de expansión respeta las reglas de bash:
- **Comillas simples (`'`):** Bloquean toda expansión
- **Comillas dobles (`"`):** Permiten expansión de variables pero bloquean word splitting
- **Sin comillas:** Permiten expansión de variables y word splitting

### Sistema de Expansión (parse/expand/)

**Archivos principales:**
- `expand.c` - Orquestador principal de expansiones
- `expand_str.c` - Expansión de strings con contexto de comillas
- `expand_vars.c` - Expansión de variables ($VAR, $?, $$, etc.)
- `expand_extract.c` - Extracción de nombres de variables
- `expand_tilde.c` - Expansión de tilde (~)
- `expand_redirs.c` - Expansión en redirecciones
- `expand_utils.c` - Utilidades de expansión
- `expand_split/` - Word splitting después de expansión

**Función:** Expande un fragmento de string respetando el contexto de comillas.

**Algoritmo:**
```
Para cada carácter:
  Si es '$' y expand=1:
    - Extraer nombre de variable
    - Buscar valor en env o local_vars
    - Reemplazar por el valor
  Si es '~' al inicio y expand=1:
    - Expandir a HOME
  Sino:
    - Copiar literalmente
```

### Casos Especiales de Expansión

#### Por qué `$"HOME"` devuelve `$HOME`:

```
Input: $"HOME"
         ↓ Tokenizer
Token: parts = [
    {"$", sq=0, dq=0},      // $ sin comillas
    {"HOME", sq=0, dq=1}    // HOME con comillas dobles
]
         ↓ Expander
- Parte 1: "$" → Como no hay nombre de variable válido tras $, se copia literal "$"
- Parte 2: "HOME" → Entre comillas dobles, pero es literal (no hay $)
         ↓
Result: "$HOME"
```

**Razón:** El `$` y `"HOME"` son tokens distintos en el parsing. El `$` solo no es una variable válida.

#### Por qué `"$HOME"` se expande:

```
Input: "$HOME"
         ↓ Tokenizer
Token: parts = [
    {"$HOME", sq=0, dq=1}    // Todo entre comillas dobles
]
         ↓ Expander
- Parte 1: "$HOME" con dq=1
  - expand_flag = 1 (las comillas dobles no bloquean expansión)
  - Detecta $ seguido de nombre válido "HOME"
  - Busca HOME en env
  - Reemplaza por su valor
         ↓
Result: "/home/usuario"
```

### expand_var (parse/expand/expand_vars.c)

**Variables especiales:**
- `$?` → Exit status del último comando
- `$$` → PID del shell (`80085`)
- `$0` → Nombre del shell
- `$_` → Último argumento del comando anterior

**Orden de búsqueda:**
1. Variables especiales
2. Variables de entorno (`env`)
3. Variables locales (`local_vars`)

### Word Splitting (parse/expand/expand_split/)

**Archivos:**
- `expand_split.c` - Lógica principal de word splitting
- `expand_split_add.c` - Añade tokens split a la lista
- `expand_split_count.c` - Cuenta palabras para splitting
- `expand_split_helpers.c` - Funciones helper
- `expand_split_utils.c` - Utilidades
- `expand_split_word.c` - Extracción de palabras individuales

**Cuándo ocurre:**
- Solo en tokens sin comillas
- Después de la expansión de variables
- Basado en espacios/tabs/newlines (IFS)

**Ejemplo:**

```
Input: echo $PATH

Si PATH="/usr/bin /bin /usr/sbin"

Sin word splitting (con comillas):
  echo "$PATH" → argv = ["echo", "/usr/bin /bin /usr/sbin"]

Con word splitting (sin comillas):
  echo $PATH → argv = ["echo", "/usr/bin", "/bin", "/usr/sbin"]
```

---

## Ejecutor

### Arquitectura del Ejecutor (execution/executor/)

**Archivos principales:**
- `executor.c` - Orquestador principal de ejecución
- `exec_single.c` - Ejecución de comando único
- `exec_child.c` - Proceso hijo en pipelines
- `exec_pipes.c` - Gestión de pipes
- `exec_wait.c` - Espera de procesos
- `exec_path.c` - Resolución de rutas de comandos
- `exec_env.c` - Construcción del entorno para execve
- `exec_utils.c` - Utilidades de ejecución

```
executor (execution/executor/executor.c)
  │
  ├─ Comando único → execute_single_command
  │   ├─ Builtin → Ejecuta directamente
  │   └─ Externo → fork + execve
  │
  └─ Pipeline → Múltiples comandos
      ├─ Crear pipes
      ├─ Fork para cada comando
      ├─ Configurar redirecciones
      └─ Esperar procesos
```

### Ejecución de Pipeline

**Pasos:**
1. **Crear pipes:** `n_cmds - 1` pipes
2. **Fork procesos:** Un hijo por comando
3. **Configurar FDs en cada hijo:**
   ```c
   // Comando i en un pipeline de n comandos:
   if (i > 0)
       dup2(pipe_fds[(i-1)*2], STDIN);   // Lee del pipe anterior
   if (i < n-1)
       dup2(pipe_fds[i*2+1], STDOUT);    // Escribe al pipe siguiente
   ```
4. **Aplicar redirecciones:** Las redirecciones sobreescriben los pipes
5. **Ejecutar comando**
6. **Cerrar pipes en padre**
7. **Esperar todos los procesos**

### Manejo de Builtins

**Builtins en comando único:** Se ejecutan en el proceso padre (para que `cd`, `export`, etc. afecten el shell)

**Builtins en pipeline:** Se ejecutan en proceso hijo (comportamiento de bash)

**Ejemplo:**
```bash
cd /tmp         # Ejecuta en padre → cambia directorio del shell
echo a | cd /   # cd ejecuta en hijo → no afecta al shell
pwd             # Sigue en el directorio original
```

### Construcción del Entorno (execution/executor/exec_env.c)

```c
char **build_exec_env(t_mini *mini)
{
    // Combina env + local_vars para pasar a execve
    // Las variables locales se heredan a comandos externos
}
```

---

## Built-in Commands (src/built_ins/)

### Estructura de Built-ins

**Archivos principales:**
- `builtins_core.c` - Dispatcher y verificación de built-ins
- `builtins_utils.c` - Funciones compartidas (getenv, setenv, etc.)
- `builtin_echo.c` - Implementación de echo
- `builtin_pwd.c` - Implementación de pwd
- `builtin_exit.c` - Implementación de exit
- `builtin_unset.c` - Implementación de unset

### Built-in: cd (src/built_ins/builtin_cd/)

**Archivos:**
- `builtin_cd.c` - Lógica principal de cd
- `builtin_cd_path.c` - Resolución de rutas (relativos, absolutos, ..)
- `builtin_cd_utils.c` - Actualización de PWD/OLDPWD, caché de HOME

**Características especiales:**
- Cache del directorio HOME para optimización
- Validación de directorios inexistentes
- Actualización automática de PWD y OLDPWD
- Soporte para `cd -` (OLDPWD)

### Built-in: env (src/built_ins/builtin_env/)

**Archivos:**
- `builtin_env.c` - Lógica principal
- `builtin_env_cmd.c` - Parsing de argumentos
- `builtin_env_exec.c` - Ejecución de comandos con env modificado
- `builtin_env_fork.c` - Fork para env con comando
- `builtins_setenv.c` - Seteo de variables de entorno

**Comportamiento:**
- Sin argumentos: imprime todas las variables de entorno
- Con comando: ejecuta comando con entorno modificado
- Soporta assignments temporales (`env VAR=val cmd`)

### Built-in: export (src/built_ins/builtin_export/)

**Archivos:**
- `builtin_export.c` - Lógica principal
- `builtin_export_print.c` - Impresión de variables exportadas
- `builtin_export_set.c` - Seteo de variables
- `builtin_export_validate.c` - Validación de nombres de variables
- `builtin_export_utils.c` - Utilidades (sorting, búsqueda)

**Características:**
- Sin argumentos: imprime variables exportadas ordenadas
- Validación de sintaxis (`VAR=value`)
- Distinción entre variables de entorno y locales

---

## Sistema de Prompt (src/prompt/)

### Generación de Prompt

**Archivos:**
- `prompt.c` - Constructor principal del prompt
- `prompt_git.c` - Detección de branch de git
- `prompt_path.c` - Formateo de ruta (home → ~)
- `prompt_utils.c` - Utilidades (hostname, username, etc.)

### Formato del Prompt

```
user@hostname:path (git_branch) $
```

**Componentes:**
1. **Usuario y Hostname:** Extraídos de variables de entorno
2. **Path:** Directorio actual (con ~ para HOME)
3. **Git Branch:** Detectado automáticamente si estás en un repo
4. **Símbolo:** `$` para usuarios normales

**Ejemplo:**
```
ravazque@laptop:~/minishell (main) $
```

### Detección de Git

El prompt detecta automáticamente si estás en un repositorio git:
- Lee `.git/HEAD` para obtener el branch actual
- Formatea el branch entre paréntesis
- Solo se muestra si existe un repositorio válido

---

## Sistema de Limpieza (src/cleaner/)

**Archivos:**
- `cleanup.c` - Limpieza general del shell
- `cleanup_exec.c` - Limpieza de estructuras de ejecución
- `free_structs.c` - Liberación de estructuras individuales

**Funciones principales:**
- `cleanup_mini()` - Libera toda la estructura `t_mini`
- `free_cmds()` - Libera lista de comandos
- `free_redirs()` - Libera redirecciones
- `free_token_list()` - Libera tokens
- `free_token_parts()` - Libera partes de tokens
- `cleanup_exec()` - Libera contexto de ejecución

**Gestión de memoria:**
- Todas las estructuras tienen su función de limpieza específica
- Limpieza en cascada (liberar `t_cmd` libera sus tokens y redirecciones)
- Cierre automático de file descriptors

---

## Sistema de Señales

### Estados de Señales

El shell maneja señales de forma diferente según el contexto:

| Contexto | SIGINT (Ctrl+C) | SIGQUIT (Ctrl+\\) | SIGWINCH |
|----------|-----------------|-------------------|----------|
| **Interactivo** (esperando input) | Nueva línea + redisplay | Ignorada | Resize terminal |
| **Heredoc** | Cancelar heredoc | Ignorada | - |
| **Ejecución** | Default (termina hijo) | Default | - |
| **Waiting** | Ignorada | Ignorada | - |

### Variable Global

```c
volatile sig_atomic_t g_signal_received = 0;
```

**Propósito:** Comunicar interrupciones entre el handler y el loop principal de forma segura.

#### Explicación de `volatile sig_atomic_t`

**Por qué usar `volatile`:**

La palabra clave `volatile` indica al compilador que esta variable puede cambiar en cualquier momento, sin que el código lo haga directamente. En minishell, esta variable se modifica desde signal handlers (manejadores de señales).

**Escenario sin `volatile`:**
```c
// Variable global
sig_atomic_t g_signal_received = 0;

// En el bucle principal
while (g_signal_received == 0) {
    // hacer cosas...
}
```

El compilador podría optimizar esto así:
```c
// El compilador lee g_signal_received UNA vez
int temp = g_signal_received;
while (temp == 0) {  // Siempre usa el valor cacheado - NUNCA sale del bucle
    // hacer cosas...
}
```

**Con `volatile`:** El compilador está obligado a leer la variable de memoria cada vez, detectando cambios hechos por el signal handler.

**Qué es `sig_atomic_t`:**

`sig_atomic_t` es un tipo definido en `<signal.h>` que garantiza:

- ✅ **Lectura/escritura atómica:** La operación se completa en una sola instrucción
- ✅ **Seguridad en signal handlers:** No se puede corromper el valor
- ✅ **Sin race conditions en asignaciones simples**

```c
// SEGURO en signal handlers
sig_atomic_t counter = 0;
counter = 42;  // Operación atómica

// NO SEGURO (típicamente)
long long big_number = 0;
big_number = 12345678;  // Podría requerir múltiples instrucciones
```

**Por qué usar ambos juntos:**

```c
volatile sig_atomic_t g_signal_received = 0;
         ^                ^
         |                |
         |                +-- Garantiza escritura atómica (seguridad)
         +------------------- Evita optimizaciones (visibilidad)
```

- `volatile`: Asegura que el compilador siempre lea de memoria (visibilidad)
- `sig_atomic_t`: Asegura que las operaciones sean atómicas (integridad)

### Sistema de Señales (src/signals/)

**Archivos:**
- `signals.c` - Configuración de señales para diferentes contextos
- `signal_handlers.c` - Manejadores de señales (SIGINT, SIGQUIT, etc.)

**setup_interactive_signals:**

```c
void setup_interactive_signals(void)
{
    // SIGINT → handle_sigint: Imprime \n y redibuja prompt
    // SIGQUIT → SIG_IGN: Ignorada
    // SIGWINCH → handle_sigwinch: Ajusta terminal
}
```

**handle_sigint:**
```c
void handle_sigint(int sig)
{
    g_signal_received = SIGINT;
    write(STDOUT_FILENO, "\n", 1);
    rl_on_new_line();           // Readline: nueva línea
    rl_replace_line("", 0);     // Limpia buffer
    rl_redisplay();             // Redibuja prompt
}
```

### Señales en Heredoc

Durante la lectura de heredocs, las señales se configuran para permitir cancelación con Ctrl+C:

```c
setup_heredoc_signals()
  → SIGINT configurado con handler específico
  → Al recibir SIGINT, collect_lines retorna error
  → Se limpia el heredoc parcial
  → Se restaura exit_sts = 130
```

### Señales en Ejecución

Los procesos hijo heredan las señales por defecto:
```c
restore_default_signals()
  → SIGINT = SIG_DFL
  → SIGQUIT = SIG_DFL
```

Esto permite que `Ctrl+C` termine comandos externos normalmente.

---

## Heredocs y Redirecciones

### Heredocs (execution/heredocs/)

**Archivos:**
- `heredocs.c` - Procesamiento principal de heredocs
- `heredocs_collect.c` - Recolección de líneas del heredoc
- `heredocs_handler.c` - Manejo de señales y errores en heredocs

**Procesamiento de heredocs:**

1. **Pre-ejecución:** Los heredocs se procesan ANTES de ejecutar comandos
2. **Recolección de líneas:**
   ```c
   collect_lines(delimiter, mini, expand, &lines)
     → Lee líneas con readline
     → Para cada línea:
         - Si es igual al delimiter → termina
         - Si expand=1 → expandir variables
         - Agregar a array de líneas
   ```
3. **Almacenamiento:**
   ```c
   create_heredoc_data(lines)
     → Crea pipe
     → Escribe todas las líneas al pipe
     → Cierra extremo de escritura
     → Almacena FD de lectura en redir->fd
   ```

**Ejemplo completo:**

```bash
cat <<EOF
Hello $USER
EOF
```

**Procesamiento:**
1. Lexer detecta `<<EOF` → crea `t_redir` con `target="EOF"`, `in_redir=2`, `hd_expand=1`
2. Antes de ejecutar, `process_heredoc`:
   - Llama a `collect_lines` con delimiter="EOF"
   - Usuario escribe: `Hello $USER`
   - Como `hd_expand=1`, expande `$USER` → `Hello ravazque`
   - Usuario escribe: `EOF` → termina recolección
3. `create_heredoc_data`:
   - Crea pipe: `pipe_fd[0]` (lectura), `pipe_fd[1]` (escritura)
   - Escribe "Hello ravazque\n" al pipe
   - Cierra `pipe_fd[1]`
4. Ejecución de `cat`:
   - En el hijo, `redirections` hace `dup2(redir->fd, STDIN)`
   - `cat` lee del pipe como si fuera un archivo

### Heredocs con Comillas

```bash
cat <<"EOF"
$USER will not expand
EOF
```

**Diferencia:**
- Lexer detecta comillas en delimiter → `hd_expand=0`
- `collect_lines` NO expande variables
- Resultado: `$USER will not expand` (literal)

### Redirecciones (execution/redirs/)

**Archivos:**
- `redirections.c` - Aplicación de redirecciones (dup2, open, etc.)

**Orden de aplicación:**
1. Redirecciones de entrada (`<`, `<<`)
2. Redirecciones de salida (`>`, `>>`)

**Múltiples redirecciones:**
```bash
cat < in1.txt < in2.txt > out.txt
```
- Se aplican en orden
- La última redirección de cada tipo gana
- Resultado: lee de `in2.txt`, escribe a `out.txt`

---

## Casos Especiales y Decisiones de Diseño

### 1. Expansión de Variables

**Problema:** `$"HOME"` vs `"$HOME"`

**Solución:** El tokenizer preserva la información de dónde estaban las comillas. Durante la expansión, se consulta cada `t_token_part` individualmente.

### 2. Variables Locales vs Entorno

**Diseño:**
```bash
VAR=value cmd args   # VAR se pasa solo a cmd
export VAR=value     # VAR se agrega a env permanentemente
```

**Implementación:**
- Assignments detectadas en lexer (`is_assignment=1`)
- En `parse/parse_assign.c`: se procesan y agregan a `local_vars`
- En `execution/executor/exec_env.c`: se combinan `env` + `local_vars`
- `local_vars` se limpian al salir del comando

### 3. Exit Status

**Reglas:**
- Comando exitoso: `exit_sts = 0`
- Comando falla: `exit_sts = código del comando`
- Señal: `exit_sts = 128 + señal` (ej: Ctrl+C → 130)
- Pipeline: `exit_sts = exit del último comando`

### 4. Fork Bomb Detection (src/fork_bomb/)

**Archivos:**
- `fork_bomb.c` - Lógica de detección y manejo de fork bombs
- `fork_bomb_utils.c` - Utilidades y validación de patrones

**Implementación:**
- Detecta patterns como `:() { : | : & }; :`
- Activa `apology_mode`
- Requiere disculpa del usuario para continuar

### 5. Salir de un directorio inexistente

**Implementación:**
- Invalida movimientos como `cd ..`
- Permite moverse a una ruta valida, absoluta o relativa

---

## Librería Auxiliar (src/aux_libft/)

### Estructura

**Directorios:**
- `include/libft.h` - Headers de la librería
- `src/` - Implementaciones de funciones

**Propósito:**
Librería personalizada que proporciona funciones auxiliares para el proyecto:
- Manipulación de strings
- Conversión de tipos
- Gestión de memoria
- Funciones de escritura
- Y otras utilidades estilo libc

**Integración:**
- Compilada como parte del proyecto
- Incluida en el header principal de minishell
- Proporciona funcionalidades base para todo el shell

---

## Resumen de Flujo Completo

```
1. Usuario escribe: echo "$HOME" > /tmp/test.txt
                          ↓
2. Tokenizer:
   Tokens: ["echo"] ["$HOME"] [">"] ["/tmp/test.txt"]
                          ↓
3. Lexer:
   cmd->tokens = ["echo", "$HOME"]
   cmd->redirs = [>"/tmp/test.txt"]
                          ↓
4. Expander:
   cmd->tokens = ["echo", "/home/ravazque"]
                          ↓
5. Executor:
   - Fork proceso hijo
   - dup2(fd_outfile, STDOUT)
   - execve("/bin/echo", ["echo", "/home/ravazque"], env)
                          ↓
6. Resultado: "/home/ravazque" escrito en /tmp/test.txt
```
