
# Minishell - Arquitectura de Procesamiento de Comandos

## Estructuras de Datos Principales

### t_mini (Estructura Principal)
```c
typedef struct s_mini {
    t_cmd    *cmds;      // Lista enlazada de comandos
    char     *input;     // Input original del usuario
    char     **env;      // Variables de entorno
    int      exit_sts;   // Último exit status
    // ... otros campos
}
```

### t_cmd (Nodo de Comando)
```c
typedef struct s_cmd {
    t_redir     *redirs;   // Lista de redirecciones
    t_token     *tokn;     // Lista de tokens (temporal durante parsing)
    char        **tokens;  // Array final de strings del comando
    struct s_cmd *next;    // Siguiente comando en el pipeline
}
```

### t_token (Token durante parsing)
```c
typedef struct s_token {
    char         *raw;       // Contenido crudo del token
    int          is_squote;  // Flag: está entre comillas simples
    int          is_dquote;  // Flag: está entre comillas dobles
    t_token_part *parts;     // Partes del token (para comillas mixtas)
    struct s_token *next;    // Siguiente token
}
```

### t_redir (Redirección)
```c
typedef struct s_redir {
    char    *target;     // Archivo/delimitador objetivo
    int     hd_expand;   // Si heredoc expande variables (0: no, 1: sí)
    int     i_redir;     // Tipo input (0: ninguno, 1: <, 2: <<)
    int     o_redir;     // Tipo output (0: ninguno, 1: >, 2: >>)
    struct s_redir *next;
}
```

## Flujo Completo de Procesamiento

### Input de Ejemplo
```bash
cat << 'EOF' | echo "hola" como estas '$USER' en "$PWD" | cat -e hola.txt
```

## FASE 1: TOKENIZER
**Función:** `tokenizer()`  
**Objetivo:** Dividir el input respetando comillas y espacios

### Proceso:
1. Recorre carácter por carácter
2. Detecta comillas y las marca
3. Separa por espacios (excepto dentro de comillas)
4. Crea una lista enlazada de tokens con metadata de comillas

### Resultado después del Tokenizer:
```
mini->cmds = {
    tokn: [
        {raw: "cat", is_squote: 0, is_dquote: 0} ->
        {raw: "<<", is_squote: 0, is_dquote: 0} ->
        {raw: "EOF", is_squote: 1, is_dquote: 0} ->  // Nota: marcado como comilla simple
        {raw: "|", is_squote: 0, is_dquote: 0} ->
        {raw: "echo", is_squote: 0, is_dquote: 0} ->
        {raw: "hola", is_squote: 0, is_dquote: 1} ->  // Nota: marcado como comilla doble
        {raw: "como", is_squote: 0, is_dquote: 0} ->
        {raw: "estas", is_squote: 0, is_dquote: 0} ->
        {raw: "$USER", is_squote: 1, is_dquote: 0} -> 
        {raw: "en", is_squote: 0, is_dquote: 0} ->
        {raw: "$PWD", is_squote: 0, is_dquote: 1} ->
        {raw: "|", is_squote: 0, is_dquote: 0} ->
        {raw: "cat", is_squote: 0, is_dquote: 0} ->
        {raw: "-e", is_squote: 0, is_dquote: 0} ->
        {raw: "hola.txt", is_squote: 0, is_dquote: 0} ->
        NULL
    ]
    tokens: NULL  // Aún no se ha llenado
    redirs: NULL  // Aún no procesado
    next: NULL    // Aún no hay pipeline
}
```

## FASE 2: LEXER
**Función:** `lexer()`  
**Objetivo:** Separar por pipes y procesar redirecciones

### Proceso:
1. **split_pipes()**: Divide la lista de tokens por pipes ('|')
2. **proc_redirs()**: Identifica y extrae redirecciones de cada comando
3. **finalize_cmds()**: Convierte tokens a array de strings

### Resultado después del Lexer:

```
mini->cmds = {
    // COMANDO 1: cat << 'EOF'
    {
        tokn: NULL,  // Ya procesado y liberado
        tokens: ["cat", NULL],
        redirs: {
            target: "EOF",
            hd_expand: 0,  // No expande porque 'EOF' tenía comillas
            i_redir: 2,    // Tipo heredoc (<<)
            o_redir: 0,
            next: NULL
        },
        next: -> COMANDO 2
    },
    
    // COMANDO 2: echo "hola" como estas '$USER' en "$PWD"
    {
        tokn: NULL,
        tokens: ["echo", "hola", "como", "estas", "$USER", "en", "$PWD", NULL],
        redirs: NULL,  // Sin redirecciones
        next: -> COMANDO 3
    },
    
    // COMANDO 3: cat -e hola.txt
    {
        tokn: NULL,
        tokens: ["cat", "-e", "hola.txt", NULL],
        redirs: NULL,  // Sin redirecciones
        next: NULL
    }
}
```

## FASE 3: EXPANDER
**Función:** `expander()`  
**Objetivo:** Expandir variables y eliminar comillas

### Reglas de Expansión:
- **Sin comillas**: Expande variables ($VAR → valor)
- **Comillas simples ('x')**: NO expande, quita comillas
- **Comillas dobles ("x")**: SÍ expande, quita comillas
- **Heredoc sin comillas (<<EOF)**: Expande en el contenido
- **Heredoc con comillas (<<'EOF')**: NO expande en el contenido

### Resultado Final después del Expander:

```
mini->cmds = {
    // COMANDO 1: cat con heredoc
    {
        tokens: ["cat", NULL],
        redirs: {
            target: "EOF",      // Delimitador sin comillas
            hd_expand: 0,       // No expandirá el contenido del heredoc
            i_redir: 2,
            o_redir: 0,
            next: NULL
        },
        next: -> COMANDO 2
    },
    
    // COMANDO 2: echo con argumentos expandidos
    {
        tokens: [
            "echo",
            "hola",              // Quitó comillas dobles
            "como",              // Sin cambios
            "estas",             // Sin cambios
            "$USER",             // NO expandido (estaba en comillas simples)
            "en",                // Sin cambios
            "/home/usuario",     // Expandido de $PWD (estaba en comillas dobles)
            NULL
        ],
        redirs: NULL,
        next: -> COMANDO 3
    },
    
    // COMANDO 3: cat con opciones
    {
        tokens: ["cat", "-e", "hola.txt", NULL],
        redirs: NULL,
        next: NULL
    }
}
```

## Resumen del Estado Final

### Lista de Comandos (mini->cmds)
Es una **lista enlazada** donde cada nodo representa un comando separado por pipe.

### Cada Nodo de Comando Contiene:
1. **tokens[]**: Array de strings con el comando y sus argumentos
   - tokens[0] = comando a ejecutar
   - tokens[1..n] = argumentos
   - tokens[último] = NULL (terminador)

2. **redirs**: Lista enlazada de redirecciones
   - Cada redirección tiene tipo y archivo objetivo
   - Para heredoc, guarda el delimitador y si debe expandir

3. **next**: Puntero al siguiente comando en el pipeline
   - NULL si es el último comando

### Flujo de Datos entre Fases:

```
INPUT ORIGINAL
    ↓
TOKENIZER (crea tokens con metadata de comillas)
    ↓
LEXER (separa por pipes y extrae redirecciones)
    ↓
EXPANDER (expande variables según contexto)
    ↓
EXECUTOR (ejecuta con la estructura final)
```

## Ejemplo de Acceso a la Estructura Final

```c
// Para ejecutar el pipeline:
t_cmd *cmd = mini->cmds;
while (cmd) {
    // cmd->tokens[0] es el comando
    // cmd->tokens[1...] son los argumentos
    
    // Procesar redirecciones si existen
    t_redir *redir = cmd->redirs;
    while (redir) {
        if (redir->i_redir == 1)      // <
            // Abrir archivo para input
        else if (redir->i_redir == 2) // <<
            // Crear heredoc
        else if (redir->o_redir == 1) // >
            // Abrir archivo para output
        else if (redir->o_redir == 2) // >>
            // Abrir archivo para append
        
        redir = redir->next;
    }
    
    // Ejecutar comando con execve(cmd->tokens[0], cmd->tokens, env)
    
    cmd = cmd->next;  // Siguiente comando en el pipeline
}
```

## Notas Importantes

1. **Liberación de Memoria**: 
   - `tokn` se libera después del lexer
   - `tokens` permanece hasta la ejecución
   - `redirs` permanece hasta la ejecución

2. **Pipes**: 
   - El lexer crea un nodo `t_cmd` por cada segmento entre pipes
   - Los conecta mediante el campo `next`

3. **Variables de Entorno**:
   - Se expanden en la fase de expansión
   - Respetan el contexto de comillas

4. **Heredoc**:
   - El delimitador se guarda en `redir->target`
   - `hd_expand` indica si se deben expandir variables en el contenido
   - 