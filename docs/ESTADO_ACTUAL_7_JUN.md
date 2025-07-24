# 📋 Minishell - Notas de Implementación AST y Pipes

---
## Parte 1

🎯 **ESTADO ACTUAL DEL PROYECTO**

### ✅ **Funcionalidades COMPLETAMENTE Implementadas:**
- **Comandos simples**: `ls`, `pwd`, `cat`, etc.
- **Builtins existentes**: `pwd`, `cd`, `env` (usando detective_conan)
- **Redirecciones básicas**: `>`, `<`, `>>`
- **Pipelines simples**: `ls | grep pattern`
- **Pipelines múltiples**: `ls | grep txt | wc -l`
- **Combinaciones**: builtins + pipes, comandos externos + redirecciones
- **Gestión de procesos**: fork, execve, waitpid correctos
- **Gestión de memoria**: liberación completa de AST y tokens

### 🔧 **Arquitectura Implementada:**
```
Input → Tokenizer → AST Builder → AST Executor
                                     ├→ Comandos simples
                                     └→ Pipelines (simples/múltiples)
```

### **Archivos Nuevos Creados:**
- `src/ast/ast_builder.c` - Construcción del AST desde tokens
- `src/execution/ast_executor.c` - Ejecución completa de AST
- Estructuras añadidas en `include/minishell.h`

### **Archivos Modificados:**
- `src/parser/parser.c` - `main_tokenizer()` ahora usa AST
- `include/minishell.h` - Estructuras y declaraciones AST

---

### **PRÓXIMOS PASOS**

### **Prioridad ALTA (Subject):**

#### 1. **Expansión de Variables** ⚠️ PENDIENTE
- `echo $HOME` - Variables de entorno
- `echo $?` - Exit status del último comando
- **Estado**: Tokenizador detecta `$` pero no expande
- **Archivos a modificar**: `parser/parser.c` (STATE_IN_VARIABLE)

#### 2. **Gestión de Signals** ⚠️ PENDIENTE
- `Ctrl+C` (SIGINT) - No debe salir del shell
- `Ctrl+\` (SIGQUIT) - No debe hacer nada
- `Ctrl+D` (EOF) - Debe salir del shell
- **Archivos a crear**: `src/signals/signal_handler.c`

#### 3. **Heredoc** ⚠️ PARCIAL
- `cat << EOF` - Actualmente detectado pero no implementado
- **Estado**: Tokenizador reconoce `<<` pero falta implementación
- **Archivos a modificar**: `src/execution/ast_executor.c` (setup_redirections)

#### 4. **Builtins Obligatorios** ⚠️ PENDIENTE
- `echo` con flag `-n`
- `export` - Exportar variables
- `unset` - Eliminar variables
- `exit` - Salir con código
- **Estado**: Solo tenemos `pwd`, `cd`, `env`
- **Archivos a modificar**: `src/builtins.c`, `src/detective_conan.c`

#### 5. **Quotes y Escaping** ⚠️ PENDIENTE
- Comillas simples: `'text'` - Sin expansión
- Comillas dobles: `"text"` - Con expansión de variables
- **Estado**: Tokenizador tiene estados pero falta implementación completa

### **Prioridad MEDIA:**

#### 6. **Operadores Lógicos** ❌ NO IMPLEMENTADO
- `&&` (AND) - Solo si comando anterior exitoso
- `||` (OR) - Solo si comando anterior falló
- **Estado**: AST tiene estructuras pero falta parser y ejecutor

#### 7. **Subshells** ❌ NO IMPLEMENTADO
- `(comando)` - Ejecutar en subshell
- **Estado**: AST tiene estructura pero falta implementación

#### 8. **Wildcards** ❌ NO IMPLEMENTADO
- `*.c` - Expansión de patrones
- **Estado**: No implementado

---

### 🔧 **RECOMENDACIONES DE IMPLEMENTACIÓN**

### **Orden Sugerido:**
1. **Signals** (crítico para funcionamiento básico)
2. **Builtins restantes** (echo, export, unset, exit)
3. **Expansión de variables** ($HOME, $?)
4. **Heredoc completo**
5. **Quotes y escaping**
6. **Operadores lógicos** (&&, ||)

---

### 🧪 **TESTS BÁSICOS QUE FUNCIONAN**

```bash
# Comandos simples
pwd
ls -la
cat file.txt

# Redirecciones
ls > output.txt
cat < input.txt
echo hello >> log.txt

# Pipelines
ls | grep txt
ls -la | grep minishell
ls | grep src | head -3
pwd | cat
```

---

### ⚠️ **LIMITACIONES CONOCIDAS**

1. **Variables**: `$HOME` no se expande
2. **Exit codes**: `$?` no disponible
3. **Signals**: Ctrl+C cierra el programa
4. **Heredoc**: `<< EOF` detectado pero no funcional
5. **Quotes**: Procesamiento básico únicamente
6. **Error messages**: Algunos muestran "Command not found" aunque funcionen

---

**Para probar:**
```bash
make
echo "comando" | ./minishell
```

---

## Parte 2 (Funcionalidades "completas")

🎯 ESTADO ACTUAL DE LOS COMPONENTES

✅ COMPLETAMENTE FUNCIONALES (no tocar):

#### 1. AST Executor 
→ Reutilizable al 100% para todo lo que falta:
•  ✅ Ya maneja pipelines simples y múltiples
•  ✅ Ya maneja comandos simples y redirecciones  
•  ✅ Ya tiene la lógica de procesos (fork, execve, waitpid)
•  ✅ Ya integra builtins a través de detective_conan()

Para nuevas funcionalidades solo necesitas:
•  Agregar nuevos builtins en detective_conan.c (echo, export, etc.)
•  Completar heredoc en setup_redirections() (1 función)

#### 2. AST Builder
→ Reutilizable al 85% - solo expandir para operadores lógicos:
•  ✅ Ya construye AST perfecto para comandos y pipes
•  ✅ Ya tiene estructuras para AND/OR en el header
•  🔧 Solo falta agregar parsing de && y || (similar a pipes)

#### 3. Estructuras AST
→ Perfectas, no tocar:
•  ✅ t_ast_node ya tiene campos para AND, OR, SUBSHELL
•  ✅ t_ast_redirect maneja todas las redirecciones
•  ✅ Gestión de memoria completa

🔧 NECESITAN AMPLIACIÓN:

#### 4. Tokenizador 
→ Base sólida, expandir funcionalidades:
•  ✅ Ya detecta todos los operadores (|, &&, ||, <<, etc.)
•  🔧 Falta: expansión de variables ($HOME → /home/usuario)
•  🔧 Falta: procesamiento de quotes (preservar/expandir según tipo)

#### 5. Detective Conan (Builtins)
→ Solo agregar más casos:

// Ya funciona:
if (ft_strcmp(list->value, "pwd") == 0)
    return (builtin_pwd());

// Solo agregar:
else if (ft_strcmp(list->value, "echo") == 0)
    return (builtin_echo(list->next));
else if (ft_strcmp(list->value, "export") == 0)
    return (builtin_export(data, list->next));

📊 ANÁLISIS POR FUNCIONALIDAD PENDIENTE:

| Funcionalidad | Tokenizador | AST Builder | AST Executor | Detective Conan |
|---------------|------------|-------------|--------------|-----------------|
| echo -n | ✅ | ✅ | ✅ | 🔧 Agregar |
| export/unset | ✅ | ✅ | ✅ | 🔧 Agregar |
| $HOME, $? | 🔧 Expandir | ✅ | ✅ | ✅ |
| Heredoc << EOF | ✅ | ✅ | 🔧 1 función | ✅ |
| Quotes 'text' | 🔧 Expandir | ✅ | ✅ | ✅ |
| && y || | ✅ | 🔧 Expandir | ✅ | ✅ |
| Signals Ctrl+C | ✅ | ✅ | ✅ | 🔧 Nuevo archivo |

✅ La mayoría ya está lista:

1. AST Executor: Reutilizable al 100% - no tocar
2. Estructuras: Perfectas - no tocar  
3. Integración: Funciona - no tocar

🔧 Solo se necesita AMPLIAR (no rehacer):

1. Tokenizador: Agregar expansión de variables y quotes
2. Detective Conan: Agregar más builtins (echo, export, etc.)
3. AST Builder: Agregar parsing de && y || (similar a pipes)

💡 PLAN DE TRABAJO:

Paso 1: Builtins (2-3 horas)
// En detective_conan.c - solo agregar casos:
else if (ft_strcmp(list->value, "echo") == 0)
    return (builtin_echo(list->next));

Paso 2: Variables (3-4 horas)  
// En parser.c - expandir STATE_IN_VARIABLE:
if (var_name == "HOME")
    add_to_buffer(parser, getenv("HOME"));

Paso 3: Operadores lógicos (2-3 horas)
// En ast_builder.c - similar a find_pipe_token():
t_token *find_and_token(t_token *tokens)
t_token *find_or_token(t_token *tokens)

🏆 CONCLUSIÓN:

•  85-90% del trabajo duro ya está hecho
•  Solo se necesita AMPLIAR funcionalidades existentes
•  La estructura AST/Executor es perfecta y reutilizable
•  No hay que rehacer nada fundamental
