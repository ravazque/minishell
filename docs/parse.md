
# PARSING EN MINISHELL - LÓGICA FUNDAMENTAL

---

## VISIÓN GENERAL

### Flujo Completo
```
INPUT: "echo hello | cat < file > out"
    ↓
TOKENIZER: Divide respetando comillas y operadores
    ["echo", "hello", "|", "cat", "<", "file", ">", "out"]
    ↓
LEXER: Separa por pipes y procesa redirecciones
    cmd1: tokens=["echo","hello"]
    cmd2: tokens=["cat"], redirs=[< file, > out]
    ↓
EXPANDER: Expande variables ($VAR, $?)
    tokens finales listos para ejecutar
    ↓
EXECUTOR
```

---

## ESTRUCTURAS DE DATOS

### La Jerarquía Completa

```
t_mini
├─ cmds → t_cmd (comando 1)
           ├─ tokens = ["echo", "hello"]  ← Array final de args
           ├─ redirs = NULL
           └─ next → t_cmd (comando 2)
                     ├─ tokens = ["cat"]
                     ├─ redirs → t_redir (< input.txt)
                     │           └─ next → t_redir (> output.txt)
                     └─ next = NULL
```

### t_token_part - Fragmento de token

```c
typedef struct s_token_part
{
    char    *content;      // Texto del fragmento
    int     is_squote;     // ¿Entre comillas simples?
    int     is_dquote;     // ¿Entre comillas dobles?
    struct  *next;
} t_token_part;
```

**¿Por qué existe?**
Para tokens mixtos como `"hello"world'$VAR'`:
```
parts[0]: content="hello",  is_dquote=1
parts[1]: content="world",  is_dquote=0
parts[2]: content="$VAR",   is_squote=1
```

### t_token - Token completo

```c
typedef struct s_token
{
    char            *raw;        // Token unido: "helloworld$VAR"
    int             is_squote;   // ¿TODO entre ''?
    int             is_dquote;   // ¿TODO entre ""?
    t_token_part    *parts;      // Lista de partes (o NULL)
    struct s_token  *next;
} t_token;
```

**Casos:**
- Token simple: `hello` → raw="hello", parts=NULL
- Token entrecomillado: `'hello'` → raw="hello", is_squote=1, parts=NULL
- Token mixto: `"a"b'c'` → raw="abc", parts=[3 partes]

### t_redir - Redirección

```c
typedef struct s_redir
{
    char    *target;      // Archivo: "input.txt"
    int     hd_expand;    // Heredoc: ¿expandir variables?
    int     in_redir;     // 0=no, 1=<, 2=<<
    int     out_redir;    // 0=no, 1=>, 2=>>
    struct  *next;
} t_redir;
```

### t_cmd - Comando

```c
typedef struct s_cmd
{
    t_redir     *redirs;     // Lista enlazada de redirecciones
    t_token     *tokn;       // Lista de tokens SIN expandir (temporal)
    char        **tokens;    // Array final: ["cat", "file.txt", NULL]
    struct      *next;       // Siguiente comando (pipe)
} t_cmd;
```

**IMPORTANTE:** 
- `tokn` se usa durante el parsing
- `tokens` es el resultado final para el executor
- Después del expander, `tokn` se libera

---

## TOKENIZER

### LÓGICA: ¿Qué hace?

**Objetivo:** Dividir el input en tokens respetando:
1. Espacios = separadores
2. Comillas = protegen espacios
3. Operadores = tokens especiales

**Principio clave:**
```
"echo hello" → 2 tokens
echo "hello world" → 2 tokens (el espacio está protegido)
echo hello world → 3 tokens
```

### Estados del Tokenizer

```
Leyendo input carácter por carácter:

ESPACIO → Finalizar token actual, skip espacios

COMILLA (' o ") → Cambiar a modo "dentro de comillas"
                  Leer hasta cerrar comilla
                  Marcar esta parte con flag

OPERADOR (|, <, >, <<, >>) → Finalizar token actual
                              Crear token para operador
                              Continuar

CARÁCTER NORMAL → Añadir al buffer del token actual
```

### ¿Cómo Sabe si Está en Comillas?

**Sistema de buffer y partes:**

```
Input: echo "hello world"test

i=0: 'e' → buffer = "e"
i=1: 'c' → buffer = "ec"
i=2: 'h' → buffer = "ech"
i=3: 'o' → buffer = "echo"
i=4: ' ' → ESPACIO → crear token("echo"), buffer = NULL

i=5: '"' → COMILLA DOBLE
         → flush buffer actual (vacío)
         → modo: dentro_comillas=1, tipo=DOBLE
         
i=6-16: leer "hello world"
        buffer = "hello world"
        
i=17: '"' → CERRAR COMILLA
          → crear parte: content="hello world", is_dquote=1
          → buffer = NULL
          
i=18: 't' → buffer = "t"
i=19: 'e' → buffer = "te"
i=20: 's' → buffer = "tes"
i=21: 't' → buffer = "test"
i=22: EOF → crear parte: content="test", is_dquote=0
          → crear token con 2 partes

Resultado:
token->raw = "hello worldtest"
token->parts:
  ├─ part[0]: "hello world" (is_dquote=1)
  └─ part[1]: "test" (is_dquote=0)
```

### Operadores Especiales

**Lógica:**
```
Si encuentro '<' o '>':
    ¿El siguiente también es '<' o '>'?
    ├─ SÍ: Operador doble (<<, >>)
    │      Crear token de 2 caracteres
    │      Avanzar índice +2
    └─ NO: Operador simple (<, >)
           Crear token de 1 carácter
           Avanzar índice +1
```

**Ejemplo:**
```
Input: cat << EOF > out

Tokens:
[0] "cat"
[1] "<<"    ← Detecta doble
[2] "EOF"
[3] ">"     ← Detecta simple
[4] "out"
```

### Resultado del Tokenizer

**De esto:**
```bash
echo "hello world" test | cat
```

**A esto:**
```
cmd->tokn (lista enlazada):
├─ token[0]: raw="echo"
├─ token[1]: raw="hello world", parts=[part("hello world", dq=1)]
├─ token[2]: raw="test"
├─ token[3]: raw="|"
└─ token[4]: raw="cat"
```

---

## LEXER

### LÓGICA: ¿Qué hace?

**Objetivo:** 
1. Separar comandos por pipes (crear nodos de `t_cmd`)
2. Extraer redirecciones de cada comando
3. Crear array final de tokens

### 1. SEPARAR POR PIPES

**Principio:**
```
Cada pipe (|) separa comandos
→ Cada comando = un nodo t_cmd
→ Los nodos se enlazan con next
```

**Algoritmo:**
```
Recorrer lista de tokens:
    Contar tokens hasta encontrar |
    Crear t_cmd con esos tokens
    Añadir a lista de comandos
    Saltar el |
    Repetir
```

**Ejemplo visual:**
```bash
Input: echo hello | cat | grep test

Tokens: [echo] [hello] [|] [cat] [|] [grep] [test]
              └─cmd1─┘   │ └cmd2┘ │ └──cmd3──┘
                         |        |
                      separa   separa

Resultado:
cmd1 → next → cmd2 → next → cmd3 → NULL
```

**Validación de sintaxis:**
```
Errores:
- | al inicio: "| cat" → ERROR
- | al final: "cat |" → ERROR  
- || seguidos: "cat | | grep" → ERROR
```

### 2. PROCESAR REDIRECCIONES

**Principio clave:**
```
Una redirección son DOS tokens:
    [operador] [archivo]
    
Estos tokens se ELIMINAN de la lista de tokens
y se MUEVEN a la lista de redirecciones
```

**Algoritmo:**
```
Para cada comando:
    Recorrer sus tokens:
        ¿Es < > << >> ?
        ├─ SÍ:
        │   1. Verificar que exista token siguiente (archivo)
        │   2. Crear t_redir con operador y archivo
        │   3. Añadir a cmd->redirs
        │   4. ELIMINAR ambos tokens de cmd->tokn
        └─ NO: continuar
```

**Ejemplo visual:**
```bash
Input: cat < input.txt file.txt > output.txt

ANTES del lexer (lista de tokens):
[cat] [<] [input.txt] [file.txt] [>] [output.txt]

DESPUÉS del lexer:

cmd->tokens:
["cat", "file.txt", NULL]

cmd->redirs:
├─ redir1: in_redir=1, target="input.txt"
└─ redir2: out_redir=1, target="output.txt"
```

**¿Por qué se eliminan?**
Porque las redirecciones NO son argumentos del comando:
```bash
cat file.txt < input.txt  # cat recibe solo "file.txt"
cat < input.txt file.txt  # cat recibe solo "file.txt"
```

### 3. HEREDOC ESPECIAL

**Lógica del hd_expand:**
```
<< delimitador

¿El delimitador tiene comillas?
├─ SÍ: 'EOF' o "EOF"
│      → hd_expand = 0 (NO expandir variables)
└─ NO: EOF
       → hd_expand = 1 (SÍ expandir variables)
```

**Ejemplo:**
```bash
<< EOF          → hd_expand = 1
<< 'EOF'        → hd_expand = 0
<< "EOF"        → hd_expand = 0
<< E'OF'        → hd_expand = 0 (tiene comillas)
```

**¿Cómo se detecta?**
```c
if (operador == "<<")
{
    token_archivo = siguiente token;
    
    if (token_archivo->is_squote || token_archivo->is_dquote)
        hd_expand = 0;
    else if (token_archivo->parts tiene alguna parte con comillas)
        hd_expand = 0;
    else
        hd_expand = 1;
}
```

### 4. CREAR ARRAY FINAL

**De lista enlazada a array:**
```
tokn (lista):       tokens (array):
├─ token1 →        [0] "cat"
├─ token2 →        [1] "file.txt"
└─ NULL            [2] NULL
```

**¿Por qué?**
Porque `execve()` necesita `char **argv`:
```c
execve("/bin/cat", cmd->tokens, env);
                   ^^^^^^^^^^^^
                   Debe ser array
```

---

## EXPANDER

### LÓGICA: ¿Cuándo expandir?

**Regla de oro:**
```
Comillas simples    → NUNCA expandir
Sin comillas        → SÍ expandir
Comillas dobles     → SÍ expandir
```

### ¿Cómo Sabe si Expandir?

**Sistema de flags:**
```
Token o parte de token tiene:
├─ is_squote = 1 → NO expandir
└─ is_squote = 0 → SÍ expandir
```

**Ejemplo:**
```bash
echo $USER '$USER' "$USER"

token[1]: $USER
├─ is_squote = 0
└─ expandir = SÍ → "john"

token[2]: $USER
├─ is_squote = 1
└─ expandir = NO → "$USER"

token[3]: $USER
├─ is_dquote = 1
└─ expandir = SÍ → "john"
```

### Proceso de Expansión

**Algoritmo:**
```
Para cada token:
    Para cada parte del token (o el token completo):
        ¿is_squote = 1?
        ├─ SÍ: copiar contenido tal cual
        └─ NO: buscar variables y expandir
```

**Búsqueda de variables:**
```
Recorrer string carácter por carácter:

Encuentro '$':
    ¿Siguiente carácter es '?' ?
    ├─ SÍ: reemplazar por exit_status
    └─ NO: ¿Siguiente es letra/_ ?
           ├─ SÍ: extraer nombre (hasta espacio/no-válido)
           │      buscar en env
           │      reemplazar por valor (o "" si no existe)
           └─ NO: dejar '$' literal
```

**Ejemplo paso a paso:**
```bash
Input: echo $USER is at $PWD

Token: "$USER"
├─ i=0: '$' → siguiente es 'U' (válido)
│            extraer "USER"
│            buscar en env → "john"
│            resultado parcial = "john"
├─ i=5: ' ' → resultado parcial = "john "
├─ i=6-8: "is at " → resultado parcial = "john is at "
└─ i=9: '$' → siguiente es 'P' (válido)
             extraer "PWD"
             buscar en env → "/home/john"
             resultado final = "john is at /home/john"
```

### Variables Especiales

**$? - Código de salida:**
```c
if (variable == "?")
    return itoa(mini->exit_sts);
```

**$VAR - Variable de entorno:**
```c
buscar en mini->env:
    "USER=john"
    "PWD=/home/john"
    ...
    
Si env[i] empieza con "VAR=":
    retornar lo que viene después del '='
Si no existe:
    retornar NULL (se convierte en "")
```

### Tokens Vacíos

**Lógica:**
```
Después de expandir:
    ¿Token resultó vacío?
    ├─ Tenía comillas → MANTENER (es un arg vacío)
    └─ No tenía comillas → ELIMINAR
```

**Ejemplo:**
```bash
echo $NOEXISTE "hola" $TAMPOCO

Expansión:
token[1]: $NOEXISTE → "" (sin comillas) → ELIMINAR
token[2]: "hola" → "hola" (tenía comillas) → MANTENER
token[3]: $TAMPOCO → "" (sin comillas) → ELIMINAR

Resultado final:
tokens = ["echo", "hola", NULL]
```

### Redirecciones

**Lógica diferente:**
```
Redirección normal (< > >>):
    SIEMPRE expandir

Heredoc (<<):
    Mirar flag hd_expand:
    ├─ hd_expand = 1 → SÍ expandir
    └─ hd_expand = 0 → NO expandir
```

**Ejemplo:**
```bash
cat < $FILE          → expandir $FILE
cat << EOF           → hd_expand=1, expandir contenido
cat << 'EOF'         → hd_expand=0, NO expandir contenido
```

---

## EJEMPLOS COMPLETOS

### Ejemplo 1: Comando Simple con Variable

```bash
Input: echo $USER
```

**TOKENIZER:**
```
tokens:
├─ [0] raw="echo"
└─ [1] raw="$USER"
```

**LEXER:**
```
cmd1:
├─ tokens = ["echo", "$USER"]
└─ redirs = NULL
```

**EXPANDER:**
```
token[1]: "$USER"
├─ is_squote = 0 → expandir
├─ buscar USER en env → "john"
└─ resultado = "john"

tokens finales = ["echo", "john", NULL]
```

---

### Ejemplo 2: Pipes y Redirecciones

```bash
Input: cat < file.txt | grep test > out.txt
```

**TOKENIZER:**
```
tokens:
[cat] [<] [file.txt] [|] [grep] [test] [>] [out.txt]
```

**LEXER - Separar por pipes:**
```
Tokens hasta |: [cat] [<] [file.txt]
→ Crear cmd1

Tokens después de |: [grep] [test] [>] [out.txt]
→ Crear cmd2
```

**LEXER - Procesar redirecciones:**
```
cmd1:
├─ Encontrar [<] [file.txt]
│  → Crear redir: in_redir=1, target="file.txt"
│  → ELIMINAR tokens [<] y [file.txt]
├─ tokens = ["cat"]
└─ redirs → [< file.txt]

cmd2:
├─ Encontrar [>] [out.txt]
│  → Crear redir: out_redir=1, target="out.txt"
│  → ELIMINAR tokens [>] y [out.txt]
├─ tokens = ["grep", "test"]
└─ redirs → [> out.txt]
```

**EXPANDER:**
```
(No hay variables, tokens sin cambios)
```

**RESULTADO FINAL:**
```
mini->cmds:
├─ cmd1: tokens=["cat", NULL]
│        redirs=[< file.txt]
│        next → cmd2
└─ cmd2: tokens=["grep", "test", NULL]
         redirs=[> out.txt]
         next=NULL
```

---

### Ejemplo 3: Comillas Mixtas

```bash
Input: echo "$USER" '$PWD' test
```

**TOKENIZER:**
```
tokens:
├─ [0] raw="echo"
├─ [1] raw="$USER", parts=[part("$USER", dq=1)]
├─ [2] raw="$PWD", parts=[part("$PWD", sq=1)]
└─ [3] raw="test"
```

**LEXER:**
```
cmd1:
├─ tokens = ["echo", "$USER", "$PWD", "test"]
└─ redirs = NULL
```

**EXPANDER:**
```
token[1]: "$USER"
├─ part[0]: is_dquote=1, is_squote=0
├─ → expandir
└─ resultado = "john"

token[2]: "$PWD"
├─ part[0]: is_squote=1
├─ → NO expandir
└─ resultado = "$PWD"

token[3]: "test"
├─ sin comillas
├─ no tiene $
└─ resultado = "test"

tokens finales = ["echo", "john", "$PWD", "test", NULL]
```

---

### Ejemplo 4: Heredoc

```bash
Input: cat << 'EOF' | grep test
$USER
EOF
```

**TOKENIZER:**
```
tokens:
[cat] [<<] [EOF] [|] [grep] [test]
      └─ is_squote=1
```

**LEXER:**
```
cmd1:
├─ Encontrar [<<] [EOF]
│  EOF tiene is_squote=1
│  → hd_expand = 0
│  → Crear redir: in_redir=2, target="EOF", hd_expand=0
│  → ELIMINAR tokens [<<] y [EOF]
├─ tokens = ["cat"]
└─ redirs → [<< EOF, hd_expand=0]

cmd2:
├─ tokens = ["grep", "test"]
└─ redirs = NULL
```

**EXPANDER:**
```
cmd1->redirs[0]:
├─ in_redir = 2 (heredoc)
├─ hd_expand = 0
└─ NO expandir $USER en el contenido
```

---

## RESUMEN DE LA LÓGICA

### TOKENIZER
- **Qué hace:** Divide el input en tokens
- **Cómo:** Lee char por char, detecta espacios, comillas, operadores
- **Guarda:** Lista enlazada de `t_token` con flags de comillas

### LEXER
- **Qué hace:** Separa comandos por pipes y extrae redirecciones
- **Cómo:** 
  - Cuenta tokens entre pipes → crea nodos `t_cmd`
  - Detecta `< > << >>` → crea `t_redir` y los ELIMINA de tokens
- **Guarda:** Lista de `t_cmd`, cada uno con su lista de `t_redir`

### EXPANDER
- **Qué hace:** Expande variables ($VAR, $?)
- **Cómo:** Mira flags `is_squote` de cada token/parte
  - `is_squote=1` → NO expandir
  - `is_squote=0` → SÍ expandir
- **Resultado:** Array final `char **tokens` listo para `execve()`

### FLUJO DE DATOS
```
INPUT (string)
    ↓
t_token (lista con partes)
    ↓
t_cmd (nodos separados)
├─ t_redir (lista)
└─ char **tokens (array)
    ↓
EXECUTOR
```
