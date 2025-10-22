
# Parseo en Minishell

---

## Introducción

El parseo es el proceso de convertir una línea de texto que escribe el usuario en estructuras de datos ejecutables por el shell. Este proceso transforma algo como `echo "hello $USER" > file.txt` en comandos con sus argumentos y redirecciones correctamente organizados.

El parseo en minishell se divide en **tres fases secuenciales**:
1. **Tokenización**: Divide el input en tokens básicos respetando comillas y operadores
2. **Lexer**: Organiza los tokens en comandos, identifica redirecciones y separa pipes
3. **Expansión**: Expande variables de entorno, elimina comillas y aplica word splitting

---

## Estructuras de Datos

### t_token_part: Fragmentos de Token

```c
typedef struct s_token_part
{
    char                *content;    // Texto del fragmento
    int                 is_squote;   // ¿Entre comillas simples?
    int                 is_dquote;   // ¿Entre comillas dobles?
    struct s_token_part *next;       // Siguiente fragmento
} t_token_part;
```

**Propósito**: Representa un fragmento de un token. Un token puede dividirse en múltiples partes según tenga comillas o no.

**Por qué existe**: Necesitamos recordar qué partes tenían comillas para saber si expandir variables y si aplicar word splitting.

**Ejemplo**: En `hello"world"'test'` tendríamos tres partes:
- `hello` (sin comillas)
- `world` (con comillas dobles)
- `test` (con comillas simples)

**Dónde se usa**: Durante la tokenización se construyen estas partes. Durante la expansión se procesan según su tipo de comillas.

---

### t_token: Token Completo

```c
typedef struct s_token
{
    char            *raw;         // Texto original completo
    int             is_squote;    // ¿Todo el token es squote?
    int             is_dquote;    // ¿Todo el token es dquote?
    t_token_part    *parts;       // Lista de fragmentos
    struct s_token  *next;        // Siguiente token
} t_token;
```

**Propósito**: Representa un token completo (palabra, operador, etc.) extraído del input del usuario.

**Campos importantes**:
- `raw`: Guarda el texto original sin modificar, útil para debugging
- `parts`: Si el token tiene mezcla de quotes y no-quotes, aquí se dividen
- `is_squote/is_dquote`: Si todo el token estaba entre un único par de comillas

**Ejemplo**: Para `"hello world"`, tendríamos:
- `raw = "hello world"` (con las comillas)
- `is_dquote = 1`
- `parts = NULL` o una sola parte con el contenido

**Dónde se usa**: Los tokens se crean en la tokenización y se van transformando hasta convertirse en el array final de argumentos.

---

### t_redir: Redirección

```c
typedef struct s_redir
{
    char            *target;      // Archivo destino
    int             hd_expand;    // ¿Expandir en heredoc?
    int             in_redir;     // Tipo entrada: 1='<', 2='<<'
    int             out_redir;    // Tipo salida: 1='>', 2='>>'
    int             fd;           // File descriptor abierto
    struct s_redir  *next;        // Siguiente redirección
} t_redir;
```

**Propósito**: Almacena información sobre una redirección de un comando.

**Campos importantes**:
- `target`: El nombre del archivo (ej: `file.txt` en `> file.txt`)
- `hd_expand`: Solo para heredoc. Si el delimiter tenía quotes, no se expanden variables dentro
- `in_redir/out_redir`: Indican el tipo de redirección (input, output, append, heredoc)
- `fd`: Cuando se abre el archivo, aquí se guarda su file descriptor

**Ejemplo**: Para `echo hello > output.txt >> log.txt` tendríamos dos t_redir en la lista:
1. `target="output.txt"`, `out_redir=1`
2. `target="log.txt"`, `out_redir=2`

**Dónde se usa**: Se crean en el lexer cuando se detectan operadores de redirección. Se usan en el executor para abrir archivos.

---

### t_cmd: Comando Completo

```c
typedef struct s_cmd
{
    t_redir     *redirs;      // Lista de redirecciones
    t_token     *tokn;        // Tokens temporales (se liberan)
    char        **tokens;     // Array final de argumentos
    struct s_cmd *next;       // Siguiente comando (pipe)
} t_cmd;
```

**Propósito**: Representa un comando completo con todos sus componentes.

**Campos importantes**:
- `redirs`: Lista enlazada de todas las redirecciones de este comando
- `tokn`: Lista temporal de tokens usada durante el parseo, se libera después
- `tokens`: Array final de strings listo para pasar a `execve`, equivalente al `argv`
- `next`: Si hay pipes, cada comando está enlazado al siguiente

**Ejemplo**: Para `cat file.txt | grep hello` tendríamos dos t_cmd:
1. Comando 1: `tokens=["cat", "file.txt", NULL]`, `redirs=NULL`, `next→`
2. Comando 2: `tokens=["grep", "hello", NULL]`, `redirs=NULL`, `next=NULL`

**Dónde se almacena**: La lista de comandos se guarda en `mini->cmds` (en la estructura t_mini principal).

---

### t_mini: Estado Global del Shell

```c
typedef struct s_mini
{
    char    **env;          // Variables de entorno
    t_cmd   *cmds;          // Lista de comandos parseados
    char    *input;         // Input del usuario
    int     exit_sts;       // Exit status del último comando
    // ... otros campos
} t_mini;
```

**Propósito**: Mantiene el estado global del shell.

**Uso durante parseo**:
- `input`: Contiene la línea que escribió el usuario
- `cmds`: Aquí se construye la lista de comandos parseados
- `env`: Se usa durante la expansión para resolver variables

---

## Flujo General

```
Input del usuario
        ↓
    [Validación]
        ↓
  [Tokenización] → Crea t_token con t_token_part
        ↓
     [Lexer] → Crea t_cmd con t_redir, separa por pipes
        ↓
   [Expansión] → Transforma t_token en char** final
        ↓
   Comandos listos para ejecutar
```

**Almacenamiento durante el flujo**:
1. **Inicio**: `mini->input` tiene el texto crudo
2. **Post-tokenización**: `mini->cmds` tiene un solo t_cmd con lista de t_token en `tokn`
3. **Post-lexer**: `mini->cmds` es lista de t_cmd (uno por pipe), cada uno con t_token en `tokn` y t_redir en `redirs`
4. **Post-expansión**: Cada t_cmd tiene su array `tokens` listo, `tokn` se libera

---

## Fase 1: Tokenización

### Objetivo
Convertir el string del input en una lista de tokens, respetando que lo que está entre comillas se trata como una unidad.

### Validación de Comillas
Antes de empezar, se verifica que todas las comillas estén cerradas usando `quotes_balanced()`.

**Lógica**: Recorre el string manteniendo contadores de comillas simples y dobles abiertas. Al final, ambos deben ser 0.

### Máquina de Estados

El tokenizador funciona como una máquina de estados que lee carácter por carácter:

**Estados**:
1. **Espacio**: Finaliza el token actual y espera el siguiente
2. **Comilla**: Inicia lectura literal hasta la comilla de cierre
3. **Operador** (`|`, `<`, `>`): Crea token de operador (pueden ser dobles: `<<`, `>>`)
4. **Regular**: Acumula el carácter en el token actual

### Buffer y Partes

Durante el proceso se mantiene:
- `bf`: Buffer que acumula caracteres
- `tp`: Lista de t_token_part siendo construida
- `in_tok`: Flag que indica si estamos dentro de un token

**Funcionamiento**:
```
Input: echo "hello world"
      ↓
Estado Inicial: bf=NULL, tp=NULL, in_tok=0
      ↓
Carácter 'e': Regular → bf="e", in_tok=1
Carácter 'c': Regular → bf="ec"
Carácter 'h': Regular → bf="ech"
Carácter 'o': Regular → bf="echo"
      ↓
Carácter ' ': Espacio → flush_part(bf) crea t_token_part con "echo"
                      → finalize_tok() crea t_token con ese part
                      → bf=NULL, tp=NULL, in_tok=0
      ↓
Carácter '"': Quote → is_dquote=1
                    → lee hasta siguiente '"'
                    → bf="hello world"
                    → flush_part() con is_dquote=1
                    → finalize_tok()
```

### Función flush_part

**Propósito**: Convierte el buffer actual en un t_token_part y lo añade a la lista.

**Parámetros**:
- `bf`: Buffer con el contenido
- `sq/dq`: Flags indicando tipo de comillas
- `tp`: Lista donde añadir el part

**Resultado**: Crea un nuevo t_token_part, lo añade a la lista y resetea el buffer.

### Función finalize_tok

**Propósito**: Convierte la lista de t_token_part en un t_token completo.

**Proceso**:
1. Junta todos los `content` de los parts en un string `raw`
2. Crea el t_token con ese `raw` y la lista de `parts`
3. Añade el token al comando actual

### Manejo de Operadores

Los operadores se tratan especialmente:
- Si estábamos en un token, primero se finaliza
- Se crea un token solo para el operador
- Se verifica si es doble (`<<` o `>>`)
- Se finaliza inmediatamente ese token

**Resultado**: Los operadores quedan como tokens independientes en la lista.

### Estado Final de la Tokenización

**En memoria**:
```
mini->cmds: Puntero a un único t_cmd
            ↓
         t_cmd:
           - tokn: Lista enlazada de t_token
           - redirs: NULL (aún no procesadas)
           - tokens: NULL (aún no creado)
```

**Ejemplo de lista de tokens** para `echo "hello" > file`:
```
Token 1: raw="echo", parts=NULL
    ↓
Token 2: raw="hello", parts=[part(content="hello", is_dquote=1)]
    ↓
Token 3: raw=">", parts=NULL
    ↓
Token 4: raw="file", parts=NULL
    ↓
NULL
```

---

## Fase 2: Lexer

### Objetivo
Organizar los tokens en comandos independientes (separados por pipes) e identificar las redirecciones.

### Paso 1: División por Pipes

**Validación previa**:
- No puede empezar con pipe
- No puede haber pipes consecutivos
- No puede terminar en pipe

**Proceso**:
1. Se recorre la lista de tokens buscando `|`
2. Cada secuencia de tokens entre pipes se convierte en un t_cmd separado
3. Los tokens se copian (incluyendo sus parts) a cada comando

**Copia profunda**: Cada t_cmd tiene su propia copia de los tokens, no comparten memoria.

**Ejemplo** para `cmd1 | cmd2 | cmd3`:
```
ANTES (un solo t_cmd):
tokn: [cmd1] → [|] → [cmd2] → [|] → [cmd3]

DESPUÉS (tres t_cmd):
t_cmd1: tokn: [cmd1]
  ↓
t_cmd2: tokn: [cmd2]
  ↓
t_cmd3: tokn: [cmd3]
```

**En memoria**:
```
mini->cmds apunta al primer comando
Cada t_cmd->next apunta al siguiente
El último tiene next=NULL
```

### Paso 2: Procesamiento de Redirecciones

Para cada t_cmd, se recorre su lista `tokn`:

**Detección**: Cuando encuentra `<`, `>`, `<<` o `>>`:
1. Valida que haya un token después (el target)
2. Valida que el siguiente no sea otro operador
3. Crea un t_redir con el tipo y target

**Tipo de redirección**:
- `<` → `in_redir = 1`
- `<<` → `in_redir = 2`
- `>` → `out_redir = 1`
- `>>` → `out_redir = 2`

**Heredoc especial**: 
Si es `<<` (heredoc), se verifica si el delimiter tiene comillas:
- Con comillas: `hd_expand = 0` (no expandir variables dentro)
- Sin comillas: `hd_expand = 1` (expandir variables dentro)

**Eliminación de tokens**: 
El operador y su target se eliminan de la lista `tokn`, dejando solo argumentos.

**Ejemplo** para `cat < input.txt > output.txt`:
```
ANTES del procesamiento de redirs:
tokn: [cat] → [<] → [input.txt] → [>] → [output.txt]

DESPUÉS:
tokn: [cat]
redirs: redir1 → redir2
        ↓        ↓
     (<, input.txt)  (>, output.txt)
```

### Paso 3: Conversión a Array

Al final del lexer, se convierte la lista `tokn` en el array `tokens`:

**Proceso**:
1. Cuenta los tokens restantes en `tokn`
2. Reserva memoria para array de strings (+ NULL)
3. Copia el `raw` de cada token al array
4. El campo `tokn` se mantiene para la fase de expansión

**Array tokens**: Es temporal en este punto, se reconstruirá en la expansión.

### Estado Final del Lexer

**En memoria**:
```
mini->cmds: Lista enlazada de t_cmd
            ↓
         t_cmd1:
           - tokn: Lista de t_token (argumentos)
           - redirs: Lista de t_redir
           - tokens: Array temporal
           - next → t_cmd2
                    ↓
                  t_cmd2: (siguiente comando)
```

---

## Fase 3: Expansión

### Objetivo
Expandir variables, eliminar comillas y aplicar word splitting, produciendo el array final de argumentos.

### Paso 1: Expansión de Variables

Para cada token en cada comando:

**Detección de variables**: Se buscan `$` seguidos de caracteres válidos.

**Variables especiales**:
```
$?  → exit_sts del último comando
$$  → PID del shell (simulado como "80085")
$0  → Nombre del programa (mini->argv[0])
$VAR → Valor de la variable VAR del entorno
```

**Reglas según quotes**:
- **Comillas simples**: No se expande nada, todo literal
- **Sin comillas o comillas dobles**: Se expande todo

**Proceso por fragmento**:
Cada t_token_part se procesa individualmente:
1. Si `is_squote`, se copia el contenido sin cambios
2. Si no, se buscan `$` y se reemplazan por sus valores

**Función exp_str_part**:
```
Input: "hello $USER world", mini (con USER=ravazque)
       ↓
Recorre carácter por carácter:
  'h' → Añade 'h'
  'e' → Añade 'e'
  ...
  '$' → Detecta variable
        → Lee "USER"
        → Busca en mini->env
        → Encuentra "ravazque"
        → Añade "ravazque"
  ' ' → Añade ' '
  'w' → Añade 'w'
  ...
       ↓
Output: "hello ravazque world"
```

**Función exp_tok_parts**:
Junta todos los parts expandidos del token en un solo string.

### Paso 2: Expansión de Redirecciones

Los targets de las redirecciones también se expanden:

**Excepción**: En heredoc donde `hd_expand=0`, el delimiter no se expande.

**Ejemplo**:
```
> $FILE.txt con FILE=output
     ↓
> output.txt
```

### Paso 3: Word Splitting

Después de expandir, algunos tokens deben dividirse por espacios:

**Criterios para splitting**:
- No está completamente entre quotes
- Tiene al menos un fragmento sin quotes
- No es una asignación (formato `VAR=valor`)

**Ejemplo**:
```
Token con parts: ["hello"] [" world"] ["test"]
                   (no-q)    (dquote)   (no-q)
       ↓
Expandido: "hello world test"
       ↓
No se splitea porque parte del medio tiene quotes

Pero:
Token: $FILES con FILES="file1 file2 file3"
       ↓
Expandido: "file1 file2 file3" (sin quotes)
       ↓
Se splitea: ["file1", "file2", "file3"]
```

**Función split_by_spaces**:
1. Cuenta palabras (secuencias sin espacios)
2. Reserva array
3. Extrae cada palabra

### Paso 4: Construcción del Array Final

**Función exp_cmd_toks_with_split**:
1. Cuenta cuántos tokens resultan después de expansión y splitting
2. Reserva memoria para el array final
3. Para cada token original:
   - Expande
   - Si está vacío y sin quotes, descarta
   - Si necesita splitting, divide y añade cada palabra
   - Si no, añade como un solo elemento

**Tokens vacíos**: 
Un token que se expande a "" se mantiene si tenía quotes, se elimina si no.

**Ejemplo**:
```
Input: echo $EMPTY "$EMPTY" hello
       ↓ (con EMPTY="")
Tokens expandidos: "" "" "hello"
       ↓
Aplicando regla de vacíos:
- Primer "": sin quotes → eliminado
- Segundo "": con quotes → mantenido
       ↓
Resultado: ["echo", "", "hello", NULL]
```

### Limpieza Final

Una vez construido el array `tokens`:
1. Se libera la lista `tokn` completa (con todos sus parts)
2. Solo queda el array `tokens` en el t_cmd

### Estado Final de la Expansión

**En memoria**:
```
mini->cmds: Lista enlazada de t_cmd
            ↓
         t_cmd:
           - tokn: NULL (liberado)
           - redirs: Lista de t_redir con targets expandidos
           - tokens: ["comando", "arg1", "arg2", NULL]
           - next → siguiente comando
```

**Este array `tokens` es lo que se pasa directamente a `execve()`.**

---

## Ejemplos Prácticos

### Ejemplo 1: Comando Simple

**Input**: `echo "hello $USER" world`

**Tokenización**:
```
Token 1: raw="echo"
Token 2: raw="hello $USER" con parts=[part("hello $USER", dquote)]
Token 3: raw="world"
```

**Lexer**:
```
t_cmd:
  - tokn: [echo] → ["hello $USER"] → [world]
  - redirs: NULL
```

**Expansión** (con USER=ravazque):
```
Token 1: "echo" → "echo"
Token 2: "hello $USER" → "hello ravazque" (mantiene como uno)
Token 3: "world" → "world"

Resultado:
tokens: ["echo", "hello ravazque", "world", NULL]
```

---

### Ejemplo 2: Redirecciones Múltiples

**Input**: `cat < input.txt > output.txt 2> error.log`

**Tokenización**:
```
[cat] → [<] → [input.txt] → [>] → [output.txt] → [2>] → [error.log]
```

**Lexer**:
```
t_cmd:
  - tokn: [cat]
  - redirs: 
      redir1: (in_redir=1, target="input.txt")
      → redir2: (out_redir=1, target="output.txt")
      → redir3: (out_redir=1, target="2>error.log")
```

**Nota**: El `2>` se tokeniza como un token, no como dos. El manejo de file descriptors específicos se hace en el executor.

---

### Ejemplo 3: Heredoc

**Input**: `cat << EOF`

**Tokenización**:
```
[cat] → [<<] → [EOF]
```

**Lexer**:
```
t_cmd:
  - tokn: [cat]
  - redirs: redir(in_redir=2, target="EOF", hd_expand=1)
```

**Si fuera `cat << "EOF"`**:
```
t_cmd:
  - tokn: [cat]
  - redirs: redir(in_redir=2, target="EOF", hd_expand=0)
```

El flag `hd_expand` indica si las variables dentro del heredoc deben expandirse.

---

### Ejemplo 4: Pipeline

**Input**: `ls -la | grep txt | wc -l`

**Tokenización**:
```
[ls] → [-la] → [|] → [grep] → [txt] → [|] → [wc] → [-l]
```

**Lexer** (división por pipes):
```
t_cmd1: tokn:[ls][-la] → redirs:NULL → next→
t_cmd2: tokn:[grep][txt] → redirs:NULL → next→
t_cmd3: tokn:[wc][-l] → redirs:NULL → next:NULL
```

**Expansión**:
```
t_cmd1: tokens:["ls", "-la", NULL]
t_cmd2: tokens:["grep", "txt", NULL]
t_cmd3: tokens:["wc", "-l", NULL]
```

Tres comandos independientes listos para conectar con pipes en el executor.

---

### Ejemplo 5: Expansión con Word Splitting

**Input**: `echo $FILES` (con FILES="file1 file2 file3")

**Tokenización**:
```
[echo] → [$FILES]
```

**Expansión**:
```
Token 1: "echo" → "echo"
Token 2: "$FILES" → "file1 file2 file3"
        ↓ (sin quotes, se splitea)
        ["file1", "file2", "file3"]

Resultado:
tokens: ["echo", "file1", "file2", "file3", NULL]
```

**Si fuera** `echo "$FILES"`:
```
Token 2: "$FILES" con dquote → "file1 file2 file3"
        ↓ (con quotes, NO se splitea)
        ["file1 file2 file3"]

Resultado:
tokens: ["echo", "file1 file2 file3", NULL]
```

---

### Ejemplo 6: Variable Vacía

**Input**: `echo $EMPTY hello` (con EMPTY="")

**Expansión**:
```
Token 1: "echo" → "echo"
Token 2: "$EMPTY" → "" (vacío sin quotes)
        ↓ (se elimina)
Token 3: "hello" → "hello"

Resultado:
tokens: ["echo", "hello", NULL]
```

**Si fuera** `echo "$EMPTY" hello`:
```
Token 2: "$EMPTY" con dquote → "" (vacío con quotes)
        ↓ (se mantiene)

Resultado:
tokens: ["echo", "", "hello", NULL]
```

---

## Resumen del Flujo de Datos

```
Input del usuario: 'echo "hello $USER" > file.txt | grep world'
                              ↓
                    ┌─────────────────┐
                    │  TOKENIZACIÓN   │
                    └────────┬────────┘
                             ↓
        t_cmd con lista de t_token en tokn:
        [echo]["hello $USER"][>][file.txt][|][grep][world]
                             ↓
                    ┌─────────────────┐
                    │     LEXER       │
                    └────────┬────────┘
                             ↓
        t_cmd1: tokn:[echo]["hello $USER"]
                redirs: (>, file.txt)
                next → t_cmd2
                
        t_cmd2: tokn:[grep][world]
                redirs: NULL
                             ↓
                    ┌─────────────────┐
                    │   EXPANSIÓN     │
                    └────────┬────────┘
                             ↓
        t_cmd1: tokens:["echo", "hello ravazque", NULL]
                redirs: (>, file.txt)
                
        t_cmd2: tokens:["grep", "world", NULL]
                redirs: NULL
                             ↓
                    ┌─────────────────┐
                    │    EXECUTOR     │
                    └─────────────────┘
```
