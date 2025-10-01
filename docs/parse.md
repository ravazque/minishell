
# Minishell - Parse & Signals

## 📦 ESTRUCTURAS DE DATOS

### `t_mini` - Estructura Principal del Shell

```c
typedef struct s_mini
{
    char        *prompt;      // Prompt con colores que se muestra
    char        *input;       // Lo que el usuario escribe (de readline)
    char        *pwd;         // Directorio actual de trabajo
    int         argc;         // Número de argumentos del programa
    char        **argv;       // Copia de argumentos del programa
    char        **env;        // Copia del environment (variables)
    int         exit_sts;     // Código de salida del último comando
    t_cmd       *cmds;        // Lista enlazada de comandos parseados
} t_mini;
```


**¿Para qué sirve cada campo?**

| Campo | Propósito | Ejemplo |
|-------|-----------|---------|
| `prompt` | Guarda el string del prompt coloreado | `"\001\033[36m\002minishell\001\033[0m\002 $ "` |
| `input` | Almacena lo que readline lee del usuario | `"echo hello \| grep h"` |
| `pwd` | Directorio actual, para el prompt y pwd | `"/home/ravazque/minishell"` |
| `argc/argv` | Info del programa (minishell -c "comando") | `argc=3, argv=["minishell", "-c", "echo"]` |
| `env` | Array de strings del environment | `["USER=ravazque", "HOME=/home/...", NULL]` |
| `exit_sts` | Último código de salida (para $?) | `0` (éxito), `127` (cmd not found), `130` (Ctrl+C) |
| `cmds` | Cabeza de la lista de comandos parseados | `cmd1 → cmd2 → cmd3 → NULL` |

**Ejemplo de estado durante ejecución:**

```c
// Usuario escribe: echo hello | grep h
mini = {
    prompt = "minishell $ ",
    input = "echo hello | grep h",
    pwd = "/home/ravazque",
    env = ["USER=ravazque", "PATH=/usr/bin", ...],
    exit_sts = 0,
    cmds → cmd1 → cmd2 → NULL
}
```

---

### `t_cmd` - Representa un Comando

```c
typedef struct s_cmd
{
    t_redir     *redirs;      // Lista de redirecciones del comando
    t_token     *tokn;        // Lista de tokens (temporal, se libera)
    char        **tokens;     // Array final de argumentos
    struct s_cmd *next;       // Siguiente comando en el pipeline
} t_cmd;
```


**¿Para qué sirve cada campo?**

| Campo | Propósito | Cuándo se usa | Cuándo se libera |
|-------|-----------|---------------|------------------|
| `redirs` | Guarda redirecciones (< > >> <<) del comando | Lexer lo crea | Al final |
| `tokn` | Lista temporal de tokens con info de comillas | Tokenizer → Expander | Después de expandir |
| `tokens` | Array final listo para execve | Después del Expander | Al final |
| `next` | Puntero al siguiente comando (pipe) | Si hay pipes | Limpieza recursiva |

**¿Por qué dos representaciones de tokens (`tokn` y `tokens`)?**

```c
// tokn (temporal) - Durante parsing:
tokn → {raw="hello", parts=[{content="hello", dq=1}]} → NULL
       ↓
       Necesario para saber si venía de comillas dobles

// tokens (final) - Después de expandir:
tokens = ["echo", "hello ravazque", NULL]
         ↓
         Listo para execve(path, tokens, env)
```

**Ejemplo de comando en memoria:**

```c
// Comando: cat < in.txt > out.txt
cmd = {
    redirs → {target="in.txt", i_redir=1} → {target="out.txt", o_redir=1} → NULL,
    tokn = NULL,  // Ya se liberó
    tokens = ["cat", NULL],
    next = NULL  // No hay pipe
}
```

---

### `t_token` - Token con Información de Comillas

```c
typedef struct s_token
{
    char            *raw;          // Token completo concatenado
    int             is_squote;     // ¿Todo el token es de comillas simples?
    int             is_dquote;     // ¿Todo el token es de comillas dobles?
    t_token_part    *parts;        // Lista de partes del token
    struct s_token  *next;         // Siguiente token
} t_token;
```


**¿Para qué sirve cada campo?**

| Campo | Propósito | Ejemplo |
|-------|-----------|---------|
| `raw` | Contenido completo del token (todas las partes unidas) | `"helloworldtest"` |
| `is_squote` | Marca si TODO el token era `'...'` | `1` si es `'hello'`, `0` si es `"hello"` |
| `is_dquote` | Marca si TODO el token era `"..."` | `1` si es `"hello"`, `0` si es `hello` |
| `parts` | Lista de fragmentos que componen el token | Ver t_token_part |
| `next` | Siguiente token en la lista | Siguiente palabra del comando |

**¿Por qué necesitamos `parts`?**

Porque un token puede ser una mezcla:

```bash
echo "hello"'world'test
```

**Ejemplo:**

```c
// Token: "hello"'world'test
token = {
    raw = "helloworldtest",  // Todo junto
    is_squote = 0,  // No es SOLO comilla simple
    is_dquote = 0,  // No es SOLO comilla doble
    parts → part1 → part2 → part3 → NULL
}
```

---

### `t_token_part` - Parte de un Token

```c
typedef struct s_token_part
{
    char                *content;      // Contenido de esta parte
    int                 is_squote;     // ¿Esta parte es de comillas simples?
    int                 is_dquote;     // ¿Esta parte es de comillas dobles?
    struct s_token_part *next;         // Siguiente parte
} t_token_part;
```


**¿Para qué sirve?**

Guarda **cada fragmento** del token con información de si venía de comillas.

**Ejemplo visual:**
```bash
Token: "hello"'world'test

part1 = {content="hello", is_squote=0, is_dquote=1}  // De comillas dobles
part2 = {content="world", is_squote=1, is_dquote=0}  // De comillas simples
part3 = {content="test",  is_squote=0, is_dquote=0}  // Sin comillas
```

**¿Por qué es crucial esta información?**

```bash
echo "$USER"'$HOME'$PWD
      ↓       ↓      ↓
   EXPANDE  NO EXP  EXPANDE

Resultado: "ravazque$HOME/home/ravazque"
```

El expander recorre las partes y decide según `is_squote`:
```c
if (part->is_squote)
    copiar literal (NO expandir)
else
    buscar y expandir variables
```

---

### `t_redir` - Redirección

```c
typedef struct s_redir
{
    char            *target;      // Archivo o delimitador heredoc
    int             hd_expand;    // Heredoc: ¿expandir variables?
    int             i_redir;      // Tipo de redirección entrada
    int             o_redir;      // Tipo de redirección salida
    struct s_redir  *next;        // Siguiente redirección
} t_redir;
```


**¿Para qué sirve cada campo?**

| Campo | Valores | Significado |
|-------|---------|-------------|
| `target` | `"file.txt"`, `"EOF"` | Nombre del archivo o delimitador |
| `hd_expand` | `0` o `1` | En heredoc: `1`=expandir vars, `0`=no expandir |
| `i_redir` | `0`, `1`, `2` | `0`=nada, `1`=`<`, `2`=`<<` |
| `o_redir` | `0`, `1`, `2` | `0`=nada, `1`=`>`, `2`=`>>` |
| `next` | puntero | Siguiente redirección del comando |

**Tabla de tipos:**

| Operador | `i_redir` | `o_redir` | `hd_expand` | Acción |
|----------|-----------|-----------|-------------|--------|
| `<` | 1 | 0 | 0 | Leer desde archivo |
| `<<` | 2 | 0 | 1 (por defecto) | Heredoc |
| `>` | 0 | 1 | 0 | Escribir truncando |
| `>>` | 0 | 2 | 0 | Escribir añadiendo |

**Ejemplos:**

```c
// cat < in.txt
redir = {
    target = "in.txt",
    hd_expand = 0,
    i_redir = 1,  // Entrada desde archivo
    o_redir = 0,
    next = NULL
}

// cat << EOF
redir = {
    target = "EOF",
    hd_expand = 1,  // Expandir variables dentro del heredoc
    i_redir = 2,    // Heredoc
    o_redir = 0,
    next = NULL
}

// cat << 'EOF'  (comillas en delimitador)
redir = {
    target = "EOF",
    hd_expand = 0,  // NO expandir por las comillas
    i_redir = 2,
    o_redir = 0,
    next = NULL
}
```

**Múltiples redirecciones:**

```bash
cat < in1.txt < in2.txt > out1.txt > out2.txt

redirs → {target="in1.txt", i_redir=1}
      → {target="in2.txt", i_redir=1}  ← Prevalece esta
      → {target="out1.txt", o_redir=1}
      → {target="out2.txt", o_redir=1}  ← Prevalece esta
      → NULL
```

---

## 🔤 TOKENIZER

### ¿Qué Hace?

Convierte el string de entrada en una **lista de tokens**, donde cada token guarda:
1. Su contenido (`raw`)
2. Si venía de comillas simples/dobles
3. Las partes que lo componen

### Lógica Completa

```
ENTRADA: char *input = "echo \"hello $USER\" 'world'"

PASO 1: Recorrer carácter por carácter
  - Acumular en buffer temporal
  - Detectar espacios → fin de token
  - Detectar comillas → lectura especial

PASO 2: Por cada token detectado
  - Crear estructura t_token
  - Guardar partes (t_token_part)
  - Añadir a lista cmd->tokn

SALIDA: Lista enlazada de t_token
```

### Proceso Detallado

#### Fase 1: Inicialización

```c
int tokenizer(t_mini **mini)
{
    char *in = (*mini)->input;           // Input del usuario
    size_t i = 0;                        // Índice actual
    char *bf = NULL;                     // Buffer temporal para caracteres
    t_token_part *tp = NULL;             // Partes del token actual
    t_cmd *cmd = malloc(...);            // Comando contenedor
    int in_tok = 0;                      // Flag: ¿estamos en un token?
    
    // Bucle principal...
}
```

#### Fase 2: Bucle Principal

```c
while (in[i])
{
    if (is_space(in[i]))
    {
        // CASO 1: ESPACIO
        // Si estábamos en un token, finalizarlo
        if (in_tok)
        {
            // 1. Convertir buffer a part
            flush_part(&bf, 0, 0, &tp)
            
            // 2. Convertir parts a token
            token = mk_tok_from_parts(tp)
            
            // 3. Añadir token a cmd->tokn
            add_tok_to_cmd(token, cmd)
            
            in_tok = 0
        }
        
        // Saltar todos los espacios
        while (is_space(in[i]))
            i++
    }
    else if (in[i] == '\'' || in[i] == '"')
    {
        // CASO 2: COMILLA
        char q = in[i]           // Guarda tipo (' o ")
        i++                      // Salta comilla apertura
        
        // Si había buffer previo, guardarlo como parte sin comillas
        if (bf != NULL)
            flush_part(&bf, 0, 0, &tp)
        
        // Leer TODO hasta comilla de cierre
        while (in[i] && in[i] != q)
        {
            append_char(&bf, in[i])
            i++
        }
        
        i++  // Salta comilla de cierre
        
        // Guardar esta parte MARCADA como de comillas
        int sq = (q == '\'') ? 1 : 0
        int dq = (q == '"') ? 1 : 0
        flush_part(&bf, sq, dq, &tp)
        
        in_tok = 1
    }
    else
    {
        // CASO 3: CARÁCTER NORMAL
        append_char(&bf, in[i])
        i++
        in_tok = 1
    }
}

// Guardar último token si quedó pendiente
if (in_tok)
    finalize_tok(&bf, &tp, cmd)
```

### Funciones Auxiliares del Tokenizer

#### `flush_part()` - Crea y guarda una parte

```c
static int flush_part(char **bf, int sq, int dq, t_token_part **parts)
{
    // Crea estructura t_token_part
    part = malloc(sizeof(t_token_part))
    part->content = strdup(bf ? bf : "")
    part->is_squote = sq
    part->is_dquote = dq
    part->next = NULL
    
    // Añade a lista de partes
    add_tok_part(parts, part)
    
    // Limpia buffer
    free(bf)
    bf = NULL
}
```

#### `mk_tok_from_parts()` - Crea token desde partes

```c
static t_token *mk_tok_from_parts(t_token_part *parts)
{
    // Une todas las partes en un string
    char *joined = ""
    for (part in parts)
        joined = strcat(joined, part->content)
    
    // Crea estructura t_token
    tok = malloc(sizeof(t_token))
    tok->raw = joined           // "helloworldtest"
    tok->is_squote = 0          // Calculado según partes
    tok->is_dquote = 0          // Calculado según partes
    tok->parts = parts          // Guarda lista de partes
    tok->next = NULL
    
    return tok
}
```

### Ejemplo Visual Completo

```bash
INPUT: echo "hello $USER" 'world'
```

**Procesamiento carácter por carácter:**

```
i=0: 'e' → bf="e", in_tok=1
i=1: 'c' → bf="ec"
i=2: 'h' → bf="ech"
i=3: 'o' → bf="echo"
i=4: ' ' → ESPACIO
     ↓
     flush_part(bf="echo", sq=0, dq=0)
     ↓
     part1 = {content="echo", is_squote=0, is_dquote=0}
     ↓
     mk_tok_from_parts([part1])
     ↓
     token1 = {raw="echo", is_squote=0, is_dquote=0, parts=[part1]}
     ↓
     add_tok_to_cmd(token1, cmd)
     ↓
     cmd->tokn = token1 → NULL
     
     bf=NULL, in_tok=0

i=5: '"' → COMILLA DOBLE
     ↓
     q='"', i=6
     ↓
     Leer hasta siguiente '"'
     
i=6-16: "hello $USER"
     ↓
     bf="hello $USER"
     
i=17: '"' → Cierra comilla
     ↓
     flush_part(bf="hello $USER", sq=0, dq=1)
     ↓
     part2 = {content="hello $USER", is_squote=0, is_dquote=1}
     ↓
     mk_tok_from_parts([part2])
     ↓
     token2 = {raw="hello $USER", is_squote=0, is_dquote=1, parts=[part2]}
     ↓
     cmd->tokn = token1 → token2 → NULL

i=18: ' ' → ESPACIO (resetea)

i=19: '\'' → COMILLA SIMPLE
     ↓
     Mismo proceso...
     ↓
     token3 = {raw="world", is_squote=1, is_dquote=0, parts=[part3]}
     ↓
     cmd->tokn = token1 → token2 → token3 → NULL
```

**Resultado final:**

```c
cmd->tokn:
  ├─ token1: {raw="echo", parts=[{content="echo", sq=0, dq=0}]}
  ├─ token2: {raw="hello $USER", parts=[{content="hello $USER", sq=0, dq=1}]}
  └─ token3: {raw="world", parts=[{content="world", sq=1, dq=0}]}
```

**¿Qué guarda `cmd` después del tokenizer?**

```c
cmd = {
    redirs = NULL,          // Aún no procesadas
    tokn = lista_tokens,    // Lista enlazada de t_token
    tokens = NULL,          // Aún no creado
    next = NULL             // Aún no hay pipes
}
```

---

## ⚙️ LEXER

### ¿Qué Hace?

Toma la lista de tokens y:
1. **Separa comandos** por pipes `|`
2. **Identifica redirecciones** (`<`, `>`, `>>`, `<<`)
3. **Crea estructuras `t_redir`**
4. **Elimina operadores** de la lista de tokens
5. **Convierte tokens** a array final `char **`

### Lógica Completa

```
ENTRADA: cmd->tokn = ["echo", "hello", "|", "grep", "h", ">", "out.txt"]

FASE 1 - split_pipes():
  Busca "|" en la lista
  Crea comando separado por cada segmento
  ↓
  cmd1->tokn = ["echo", "hello"]
  cmd2->tokn = ["grep", "h", ">", "out.txt"]

FASE 2 - proc_redirs():
  Para cada comando:
    Busca <, >, >>, <<
    Crea t_redir
    ELIMINA operador y target de tokn
  ↓
  cmd1->tokn = ["echo", "hello"]
  cmd1->redirs = NULL
  
  cmd2->tokn = ["grep", "h"]
  cmd2->redirs = [{target="out.txt", o_redir=1}]

FASE 3 - finalize_cmds():
  Convierte tokn (lista) → tokens (array)
  ↓
  cmd1->tokens = ["echo", "hello", NULL]
  cmd2->tokens = ["grep", "h", NULL]

SALIDA: Lista de comandos con tokens y redirs separados
```

### Fase 1: División por Pipes

```c
static int split_pipes(t_mini *mini)
{
    // Entrada: cmd->tokens = ["echo", "hello", "|", "grep", "h"]
    char **toks = mini->cmds->tokens
    t_cmd *lst = NULL  // Nueva lista de comandos
    int start = 0
    int i = 0
    
    while (toks[i])
    {
        if (strcmp(toks[i], "|") == 0)
        {
            // Encontró pipe
            // Crear comando con tokens[start...i-1]
            
            new_cmd = malloc(sizeof(t_cmd))
            new_cmd->tokn = NULL
            
            // Copiar tokens del rango al nuevo comando
            for (j = start; j < i; j++)
            {
                tok_node = mk_tok_node(toks[j])
                add_tok(new_cmd, tok_node)
            }
            
            // Añadir a lista
            add_cmd_to_lst(&lst, new_cmd)
            
            start = i + 1  // Siguiente comando empieza después del pipe
        }
        i++
    }
    
    // Último comando (o único si no hay pipes)
    mk_cmd_range(toks, start, i, &lst)
    
    // Liberar tokens originales y reemplazar
    free_dblptr(mini->cmds->tokens)
    free_cmds(mini->cmds)
    mini->cmds = lst  // Ahora mini->cmds apunta a lista de comandos
}
```

**Ejemplo visual:**
```
ANTES:
mini->cmds: {tokn=NULL, tokens=["echo", "|", "grep"]}

DESPUÉS:
mini->cmds → cmd1 → cmd2 → NULL
             ↓       ↓
       tokn=[echo] tokn=[grep]
```

### Fase 2: Procesamiento de Redirecciones

```c
static int proc_redirs(t_cmd *cmd)
{
    t_token *curr = cmd->tokn
    t_token *prev = NULL
    
    while (curr)
    {
        next = curr->next
        
        // ¿Es operador de redirección?
        if (is_redir(curr->raw))  // <, >, >>, <<
        {
            // Debe haber siguiente token (el archivo)
            if (!next)
                return error("syntax error near newline")
            
            // CREAR ESTRUCTURA t_redir
            redir = malloc(sizeof(t_redir))
            redir->target = strdup(next->raw)  // Nombre del archivo
            redir->next = NULL
            
            // Determinar tipo según operador
            if (curr->raw == "<")
                redir->i_redir = 1
            else if (curr->raw == "<<")
                redir->i_redir = 2, redir->hd_expand = 1
            else if (curr->raw == ">")
                redir->o_redir = 1
            else if (curr->raw == ">>")
                redir->o_redir = 2
            
            // Añadir a lista de redirecciones
            add_redir(cmd, redir)
            
            // ELIMINAR curr y next de cmd->tokn
            rm_redir_toks(&cmd->tokn, curr, prev)
            
            // Ajustar punteros para continuar
            curr = prev ? prev->next : cmd->tokn
            continue
        }
        
        prev = curr
        curr = next
    }
}
```

**Ejemplo paso a paso:**
```
ENTRADA: cmd->tokn = [cat] → [<] → [in.txt] → [>] → [out.txt] → NULL

Iteración 1:
  curr = [cat]
  No es redir → prev=[cat], curr=[<]

Iteración 2:
  curr = [<], next = [in.txt]
  ES REDIR
  ↓
  redir1 = {target="in.txt", i_redir=1, o_redir=0}
  add_redir(cmd, redir1)
  ↓
  rm_redir_toks(): Elimina [<] y [in.txt]
  ↓
  cmd->tokn = [cat] → [>] → [out.txt] → NULL
  cmd->redirs = redir1 → NULL
  ↓
  curr = [cat]->next = [>]

Iteración 3:
  curr = [>], next = [out.txt]
  ES REDIR
  ↓
  redir2 = {target="out.txt", i_redir=0, o_redir=1}
  add_redir(cmd, redir2)
  ↓
  rm_redir_toks(): Elimina [>] y [out.txt]
  ↓
  cmd->tokn = [cat] → NULL
  cmd->redirs = redir1 → redir2 → NULL

RESULTADO:
  cmd->tokn = [cat]
  cmd->redirs = [{target="in.txt", i_redir=1}] 
              → [{target="out.txt", o_redir=1}]
```

### Fase 3: Conversión a Array

```c
static int finalize_cmds(t_mini *mini)
{
    t_cmd *curr = mini->cmds
    
    while (curr)
    {
        // Convertir lista de tokens → array
        curr->tokens = toks_to_arr(curr->tokn)
        
        curr = curr->next
    }
}
```

```c
static char **toks_to_arr(t_token *toks)
{
    // Cuenta tokens
    cnt = 0
    for (t in toks)
        cnt++
    
    // Reserva array
    arr = malloc(sizeof(char*) * (cnt + 1))
    
    // Copia cada token->raw al array
    i = 0
    for (t in toks)
    {
        arr[i] = strdup(t->raw)
        i++
    }
    arr[cnt] = NULL
    
    return arr
}
```

**Transformación:**
```
ANTES:
cmd->tokn = [tok1] → [tok2] → [tok3] → NULL
             ↓        ↓        ↓
           raw="cat" raw="hello" raw="world"

DESPUÉS:
cmd->tokens = ["cat", "hello", "world", NULL]
                ↓       ↓         ↓
	       raw="cat" raw="hello" raw="world"
```

**¿Qué contiene `cmd` después del lexer?**

```c
cmd = {
    redirs = lista_redirecciones,  // t_redir creadas
    tokn = lista_tokens,            // Aún existe (se libera después)
    tokens = array_strings,         // Listo para execve
    next = siguiente_comando        // Si hay pipe
}
```

---

## 🔄 EXPANDER

### ¿Qué Hace?

Recorre `cmd->tokens` y **sustituye variables** por sus valores:
1. Lee cada token
2. Verifica si tenía comillas simples (NO expandir)
3. Busca `$VARIABLE` en el string
4. Extrae nombre de variable
5. Busca valor en `mini->env`
6. Sustituye en el resultado

### Lógica Completa

```
ENTRADA: cmd->tokn con información de comillas

PARA CADA COMANDO:
  PARA CADA TOKEN:
    SI token tiene comillas simples:
      NO EXPANDIR → copiar literal
    
    SI token tiene comillas dobles o sin comillas:
      exp_str_part():
        Buscar '# 📚 Documentación Simplificada - Minishell

> **Autor:** ravazque  
> **Proyecto:** Minishell - Shell Bash simplificado  

---

## 📋 Índice

1. [Visión General](#visión-general)
2. [Tokenizer](#tokenizer)
3. [Lexer](#lexer)
4. [Expander](#expander)
5. [Señales](#señales)
6. [Ejemplo Completo](#ejemplo-completo)

---

## 🎯 Visión General

### ¿Qué hace tu programa?

Tu minishell procesa comandos en 5 pasos:

```

Input: echo "Hello $USER" | grep world > file.txt

1. TOKENIZER  → Divide en tokens respetando comillas
2. LEXER           → Separa por pipes y detecta redirecciones
3. EXPANDER   → Sustituye variables ($USER → ravazque)
4. SEÑALES      → Maneja Ctrl+C, Ctrl+D
5. EJECUCIÓN (HEREDOC, REDIRECCIONES, ETC...) → ❌ POR IMPLEMENTAR

```

### Estructura Principal

```c
typedef struct s_mini
{
    char     *input;      // Lo que escribe el usuario
    char     **env;       // Variables de entorno
    int      exit_sts;    // Código de salida último comando
    t_cmd    *cmds;       // Lista de comandos parseados
} t_mini;

typedef struct s_cmd
{
    char     **tokens;    // ["echo", "hello", NULL]
    t_redir  *redirs;     // Lista de redirecciones
    t_cmd    *next;       // Siguiente comando (para pipes)
} t_cmd;
```

---
---
## 🔤 TOKENIZER

### ¿Qué hace?

Convierte el string de entrada en **tokens** (palabras), respetando comillas.

### Lógica

```
Input: echo "hello world" 'test'

1. Lee carácter por carácter
2. Espacios → separan tokens
3. Comillas → agrupa todo lo que esté dentro como un solo token
4. Guarda cada token con información de si venía de comillas
```

### Funciones Principales

#### `tokenizer()` - Función principal

```c
int tokenizer(t_mini **mini)
{
    // Variables
    char *in = input del usuario
    char *buffer = acumula caracteres del token actual
    int in_tok = flag "¿estoy construyendo un token?"
    
    // Recorre input carácter por carácter
    while (hay caracteres)
    {
        if (es_espacio)
            → Guarda token actual y resetea buffer
        
        else if (es_comilla ' o ")
            → Lee todo hasta cerrar comilla
            → Marca que viene de comillas
        
        else
            → Añade carácter al buffer
    }
    
    // Guarda último token
}
```

#### `handle_quote()` - Maneja comillas

```c
static int handle_quote(...)
{
    // Guarda tipo de comilla (' o ")
    char q = comilla actual
    
    // Salta comilla de apertura
    i++
    
    // Lee TODO hasta encontrar comilla de cierre
    while (no encuentro comilla de cierre)
        añade carácter al buffer
    
    // Marca esta parte como "de comillas"
    // Importante: esto permite saber después si expandir o no
}
```

### Ejemplo Visual

```
Input: echo "hello $USER" world

Paso 1: 'e' 'c' 'h' 'o'
  → buffer = "echo"
  
Paso 2: ' ' (espacio)
  → Guarda token: {content="echo", comillas=NO}
  → Resetea buffer

Paso 3: '"' (comilla doble)
  → Lee hasta siguiente '"'
  → buffer = "hello $USER"
  → Guarda token: {content="hello $USER", comillas=DOBLES}

Paso 4: ' ' (espacio)
  → Resetea buffer

Paso 5: 'w' 'o' 'r' 'l' 'd'
  → buffer = "world"
  → Guarda token: {content="world", comillas=NO}

RESULTADO:
Token 1: "echo" (sin comillas)
Token 2: "hello $USER" (con comillas dobles)
Token 3: "world" (sin comillas)
```

### ¿Por qué es importante guardar si venía de comillas?

Porque después el **EXPANDER** necesita saber:
- Comillas simples `'$USER'` → NO expandir (dejar literal)
- Comillas dobles `"$USER"` → SÍ expandir
- Sin comillas `$USER` → SÍ expandir

---

## ⚙️ LEXER

### ¿Qué hace?

1. **Separa comandos** por pipes `|`
2. **Identifica redirecciones** (`<`, `>`, `>>`, `<<`)
3. **Organiza la información** para que sea fácil ejecutar

### Lógica

```
Input tokenizado: ["echo", "hello", "|", "grep", "h", ">", "out.txt"]

FASE 1 - División por pipes:
  Busca "|" en los tokens
  Crea un comando separado por cada pipe
  
  Resultado:
    Comando 1: ["echo", "hello"]
    Comando 2: ["grep", "h", ">", "out.txt"]

FASE 2 - Procesar redirecciones:
  Busca operadores de redirección (<, >, >>, <<)
  Extrae archivo/target
  Elimina operador y archivo de la lista de tokens
  
  Resultado:
    Comando 1: tokens=["echo", "hello"], redirs=NULL
    Comando 2: tokens=["grep", "h"], redirs=[{target="out.txt", tipo=salida}]
```

### Funciones Principales

#### `lexer()` - Función principal

```c
int lexer(t_mini *mini)
{
    // 1. Divide por pipes
    split_pipes(mini)  // echo hello | grep h  →  2 comandos
    
    // 2. Para cada comando, procesa redirecciones
    for cada comando
        proc_redirs(comando)  // grep h > out.txt  →  extrae redirección
    
    // 3. Convierte todo a arrays finales
    finalize_cmds(mini)
}
```

#### `split_pipes()` - Separa por pipes

```c
static int split_pipes(t_mini *mini)
{
    // Recorre todos los tokens
    for cada token
    {
        if (token == "|")
        {
            // Crea nuevo comando con tokens anteriores
            crear_comando(desde_inicio, hasta_aquí)
            
            // Marca nuevo inicio
            inicio = posición_actual + 1
        }
    }
    
    // Crea último comando
    crear_comando(desde_inicio, hasta_final)
}
```

#### `proc_redirs()` - Procesa redirecciones

```c
static int proc_redirs(t_cmd *cmd)
{
    // Recorre tokens del comando
    for cada token
    {
        if (token es <, >, >>, <<)
        {
            siguiente_token = archivo/delimitador
            
            // Crea estructura de redirección
            redir = {
                target = siguiente_token
                tipo = entrada/salida/append/heredoc
            }
            
            // Añade a lista de redirecciones
            añadir_a_lista(cmd->redirs, redir)
            
            // ELIMINA ambos tokens (operador y archivo)
            eliminar(token_actual y siguiente_token)
        }
    }
}
```

### Tipos de Redirección

```c
< archivo     →  i_redir=1  (entrada desde archivo)
> archivo     →  o_redir=1  (salida truncando)
>> archivo    →  o_redir=2  (salida añadiendo)
<< delimiter  →  i_redir=2  (heredoc)
```

### Ejemplo Visual

```
Input: cat < in.txt | grep hello > out.txt

DESPUÉS DEL TOKENIZER:
["cat", "<", "in.txt", "|", "grep", "hello", ">", "out.txt"]

DESPUÉS DE split_pipes():
  Comando 1: ["cat", "<", "in.txt"]
  Comando 2: ["grep", "hello", ">", "out.txt"]

DESPUÉS DE proc_redirs():
  Comando 1:
    tokens = ["cat"]
    redirs = [{target="in.txt", i_redir=1}]
  
  Comando 2:
    tokens = ["grep", "hello"]
    redirs = [{target="out.txt", o_redir=1}]
```

---

## 🔄 EXPANDER

### ¿Qué hace?

Sustituye las **variables de entorno** (`$USER`, `$HOME`, `$?`) por sus valores reales.

### Reglas

```
'$USER'      → NO expande (comilla simple)
"$USER"      → SÍ expande (comilla doble)
$USER        → SÍ expande (sin comillas)
$?           → Sustituye por código de salida
$NOEXISTE    → Sustituye por vacío ""
```

### Lógica

```
Para cada comando:
    Para cada token:
        Si el token tenía comillas simples:
            NO EXPANDIR (dejar como está)
        
        Si el token tenía comillas dobles o sin comillas:
            Buscar todos los $VARIABLE
            Sustituir por su valor
            Si no existe → sustituir por ""
```

### Funciones Principales

#### `expander()` - Función principal

```c
int expander(t_mini *mini)
{
    for cada comando
    {
        // 1. Expande los tokens
        exp_cmd_toks(comando)
        
        // 2. Expande archivos de redirecciones
        exp_redirs(comando)
    }
}
```

#### `exp_tok_parts()` - Expande un token

```c
static char *exp_tok_parts(t_token *tok, t_mini *mini)
{
    resultado = ""
    
    // Cada token puede tener múltiples "partes"
    // (por ejemplo: "hello"'world' son 2 partes)
    
    for cada parte del token
    {
        if (parte es de comillas simples)
            NO expandir → añadir literal
        
        else
            expandir_string(parte) → buscar $VAR y sustituir
            añadir al resultado
    }
    
    return resultado
}
```

#### `exp_str_part()` - Expande un string

```c
static char *exp_str_part(const char *s, t_mini *mini, int expandir)
{
    resultado = ""
    
    for cada carácter en s
    {
        if (carácter es '$' Y permitido expandir Y hay siguiente carácter)
        {
            // Extrae nombre de variable
            var_name = leer_hasta_no_alfanumérico()  // $USER123 → "USER123"
            
            // Busca valor en environment
            valor = buscar_en_env(var_name, mini->env)
            
            // Añade valor al resultado
            resultado += valor
        }
        else
        {
            // Carácter normal
            resultado += carácter
        }
    }
    
    return resultado
}
```

#### `get_env_val()` - Busca variable en environment

```c
static char *get_env_val(const char *key, char **env)
{
    // env = ["HOME=/home/user", "USER=ravazque", "PATH=/usr/bin", ...]
    
    for cada línea en env
    {
        if (línea empieza con "KEY=")
        {
            return parte después del '='
        }
    }
    
    return NULL  // No encontrada
}
```

### Variables Especiales

```c
$?  →  Código de salida del último comando
       Ejemplo: echo $? → "0" si el último comando fue exitoso
```

### Ejemplo Visual

```
Input: echo "$USER lives in $HOME" '$PWD'

TOKENS DESPUÉS DEL TOKENIZER:
Token 1: "echo" (sin comillas)
Token 2: "$USER lives in $HOME" (comillas dobles)
Token 3: "$PWD" (comillas simples)

PROCESO DE EXPANSIÓN:

Token 1: "echo"
  → Sin variables → resultado: "echo"

Token 2: "$USER lives in $HOME"
  → Tiene comillas dobles → SÍ EXPANDIR
  
  Procesamiento:
  i=0: '$' → Encuentra variable
       var_name = "USER"
       buscar_en_env("USER") → "ravazque"
       resultado = "ravazque"
  
  i=5-14: " lives in "
       resultado = "ravazque lives in "
  
  i=15: '$' → Encuentra variable
       var_name = "HOME"
       buscar_en_env("HOME") → "/home/ravazque"
       resultado = "ravazque lives in /home/ravazque"

Token 3: "$PWD"
  → Tiene comillas simples → NO EXPANDIR
  → resultado: "$PWD" (literal)

TOKENS FINALES:
["echo", "ravazque lives in /home/ravazque", "$PWD"]
```

### Tokens Vacíos

```c
Input: echo "" $NOEXISTE "hello"

Expansión:
  ""         → Tiene comillas → SE CONSERVA como ""
  $NOEXISTE  → Sin comillas y no existe → SE ELIMINA
  "hello"    → Tiene comillas → SE CONSERVA como "hello"

Resultado: ["echo", "", "hello"]
```

---

## 🚨 SEÑALES

### ¿Qué hace?

Maneja las interrupciones del usuario para que el shell no se rompa.

### Señales Principales

```
Ctrl+C (SIGINT)   → Interrumpe comando, NO sale del shell
Ctrl+D (EOF)      → Sale del shell limpiamente
Ctrl+\ (SIGQUIT)  → Ignorada en modo interactivo
```

### Lógica

```
SETUP INICIAL:
  Configura handler personalizado para Ctrl+C
  Ignora Ctrl+\

CUANDO USUARIO PRESIONA Ctrl+C:
  1. Handler se ejecuta
  2. Imprime nueva línea
  3. Limpia línea de readline
  4. Redibuja prompt
  5. Marca variable global g_signal_received = SIGINT

EN EL LOOP PRINCIPAL:
  Verifica si g_signal_received tiene valor
  Si es SIGINT → establece exit_status = 130
  Resetea g_signal_received a 0
```

### Funciones Principales

#### `setup_interactive_signals()` - Configuración inicial

```c
void setup_interactive_signals(void)
{
    // Configura handler para SIGINT (Ctrl+C)
    sigaction(SIGINT, handler_personalizado)
    
    // Ignora SIGQUIT (Ctrl+\)
    signal(SIGQUIT, SIG_IGN)
}
```

#### `handle_sigint()` - Handler de Ctrl+C

```c
static void handle_sigint(int sig)
{
    // Marca que recibimos la señal
    g_signal_received = SIGINT
    
    // Nueva línea
    write(STDOUT, "\n")
    
    // Limpia readline
    rl_on_new_line()
    rl_replace_line("", 0)
    rl_redisplay()
}
```

#### `ft_signal()` - Verificación en loop

```c
void ft_signal(t_mini *mini)
{
    // Si readline devolvió NULL (Ctrl+D)
    if (!mini->input)
    {
        write("exit\n")
        cleanup_and_exit()
    }
    
    // Si recibimos Ctrl+C
    if (g_signal_received == SIGINT)
    {
        mini->exit_sts = 130  // Código estándar
        g_signal_received = 0  // Reset
    }
}
```

### Comportamiento Visual

```bash
# Usuario escribiendo comando
$ sleep 10
  ^C        ← Presiona Ctrl+C

$ [prompt limpio, ready para nuevo comando]
```

```bash
# EOF (Ctrl+D)
$ [Ctrl+D]
exit
[Shell termina]
```

### Códigos de Salida

```
0    → Éxito
1    → Error general
2    → Mal uso de comando
127  → Comando no encontrado
130  → Terminado con Ctrl+C
```

---

## 📝 EJEMPLO COMPLETO

### Input del usuario:

```bash
echo "Hello $USER" | grep $HOME > out.txt
```

### Paso 1: TOKENIZER

```
Resultado:
Token 1: "echo"
Token 2: "Hello $USER" (comillas dobles)
Token 3: "|"
Token 4: "grep"
Token 5: "$HOME" (sin comillas)
Token 6: ">"
Token 7: "out.txt"
```

### Paso 2: LEXER

```
División por pipes:
  Comando 1: ["echo", "Hello $USER"]
  Comando 2: ["grep", "$HOME", ">", "out.txt"]

Procesamiento de redirecciones:
  Comando 1: 
    tokens = ["echo", "Hello $USER"]
    redirs = NULL
  
  Comando 2:
    tokens = ["grep", "$HOME"]
    redirs = [{target="out.txt", o_redir=1}]
```

### Paso 3: EXPANDER

```
Comando 1:
  "echo" → "echo" (sin cambios)
  "Hello $USER" (comillas dobles) → "Hello ravazque"
  
  RESULTADO: tokens = ["echo", "Hello ravazque"]

Comando 2:
  "grep" → "grep" (sin cambios)
  "$HOME" (sin comillas) → "/home/ravazque"
  
  RESULTADO: 
    tokens = ["grep", "/home/ravazque"]
    redirs = [{target="out.txt", o_redir=1}]
```

### Estructura Final en Memoria

```c
mini->cmds:
  ├─ Comando 1:
  │    tokens = ["echo", "Hello ravazque", NULL]
  │    redirs = NULL
  │    next → Comando 2
  │
  └─ Comando 2:
       tokens = ["grep", "/home/ravazque", NULL]
       redirs → {target="out.txt", o_redir=1}
       next = NULL
```

---
---
# RESUMEN DE FUNCIONALIDADES

### ✅ Implementado

| Componente | Funcionalidad |
|------------|---------------|
| **Tokenizer** | Divide input en tokens respetando comillas |
| **Lexer** | Separa comandos por pipes y detecta redirecciones |
| **Expander** | Expande variables de entorno ($VAR, $?) |
| **Señales** | Maneja Ctrl+C, Ctrl+D, Ctrl+\ correctamente |
| **Built-ins** | echo, cd, pwd, env, exit funcionan |
| **Prompt** | Prompt con colores y detección de git |
| **Memoria** | Sistema completo de limpieza (free) |

### ❌ Pendiente (Prioridad Alta)

| Componente        | Qué falta                                    |
| ----------------- | -------------------------------------------- |
| **Ejecución**     | Ejecutar comandos externos con fork/execve   |
| **Pipes**         | Conectar comandos con tuberías funcionales   |
| **Redirecciones** | Aplicar < > >> realmente                     |
| **Heredocs**      | Implementar << con lectura hasta delimitador |
| **export/unset**  | Modificar variables de entorno               |

---

## 💡 CONCEPTOS CLAVE

### ¿Por qué 3 pasos (Tokenizer → Lexer → Expander)?

Cada paso tiene una responsabilidad clara:

1. **Tokenizer**: "¿Cuáles son las palabras?"
2. **Lexer**: "¿Cómo se organizan? (pipes, redirs)"
3. **Expander**: "¿Qué valores tienen las variables?"

Separar estas tareas hace el código más limpio y fácil de debuggear.

### ¿Por qué guardar info de comillas?

```bash
echo '$USER'    # Debe imprimir: $USER
echo "$USER"    # Debe imprimir: ravazque
```

El tokenizer marca de dónde viene cada token, para que el expander sepa si debe expandir o no.

### ¿Por qué usar listas enlazadas?

```c
cmd1 → cmd2 → cmd3 → NULL
```

Porque no sabes de antemano cuántos comandos habrá en un pipe:
- `echo hello` → 1 comando
- `echo hello | grep h | wc -l` → 3 comandos
