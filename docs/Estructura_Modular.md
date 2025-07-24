# Estructura Modular

Este documento describe una arquitectura modular sugerida para el proyecto Minishell. Cada módulo está pensado para cumplir una función específica, permitiendo claridad, mantenimiento y pruebas más fáciles.

---

## 🧱 Módulos Principales

### 1. `main.c`

- Punto de entrada del programa.
    
- Inicializa el entorno.
    
- Configura señales.
    
- Llama al bucle principal de ejecución.
    

---

### 2. `prompt/`

- Muestra el prompt personalizado.
    
- Usa `readline()` para leer la entrada del usuario.
    
- Añade la línea válida al historial (`add_history`).
    

---

### 3. `lexer/`

**Responsabilidad:** Analizar la cadena de entrada y dividirla en tokens.

- Detecta palabras, operadores (`|`, `>`, `<`, `<<`, etc.), comillas y delimitadores.
    
- Trata correctamente los caracteres de escape (`\`), espacios, comillas abiertas.
    
- Genera una lista de tokens que será procesada por el parser.
    

**Archivos sugeridos:**

- `lexer.c`, `lexer_utils.c`, `token.c`, `quote_handling.c`
    

---

### 4. `parser/`

**Responsabilidad:** Interpretar los tokens y construir una estructura ejecutable.

- Verifica la validez sintáctica (pipes, redirecciones mal ubicadas, etc.).
    
- Organiza los tokens en estructuras (por ejemplo, árboles de comandos o listas encadenadas).
    
- Maneja la precedencia de operadores.
    

**Archivos sugeridos:**

- `parser.c`, `command_builder.c`, `syntax_validator.c`
    

---

### 5. `expander/`

**Responsabilidad:** Expandir variables de entorno y realizar sustituciones.

- Reemplaza `$VAR` por su valor del entorno.
    
- Trata con comillas: no expande dentro de comillas simples.
    
- Preprocesa here-documents (`<<`) si existen.
    

**Archivos sugeridos:**

- `expander.c`, `env_utils.c`, `heredoc.c`
    

---

### 6. `executor/`

**Responsabilidad:** Ejecutar comandos, manejar procesos, redirecciones y pipes.

- Determina si el comando es built-in o externo.
    
- Ejecuta con `fork()` + `execve()` si es externo.
    
- Redirige entrada/salida con `dup2()` según lo indicado por el parser.
    
- Maneja múltiples procesos con `pipe()`.
    
- Ejecuta built-ins en el proceso padre si es necesario (como `cd`, `exit`, etc.).
    

**Archivos sugeridos:**

- `executor.c`, `pipe.c`, `redir.c`, `builtin_wrapper.c`, `process.c`
    

---

### 7. `builtins/`

**Responsabilidad:** Implementar comandos internos (`cd`, `pwd`, `echo`, etc.).

- Cada built-in debe tener su propia función.
    
- Idealmente usar una tabla de punteros a funciones (`struct` con nombre + función).
    

**Archivos sugeridos:**

- `builtin_cd.c`, `builtin_echo.c`, etc.
    

---

### 8. `env/`

**Responsabilidad:** Administrar el entorno (`envp`, `export`, `unset`).

- Maneja internamente una copia de las variables de entorno.
    
- Permite modificarlas, consultarlas, imprimirlas.
    

**Archivos sugeridos:**

- `env.c`, `env_utils.c`
    

---

### 9. `signals/`

**Responsabilidad:** Manejar interrupciones como `Ctrl+C`, `Ctrl+\`, etc.

- Define los handlers (`SIGINT`, `SIGQUIT`).
    
- Comportamiento distinto según si el shell está ejecutando o esperando input.
    

**Archivos sugeridos:**

- `signals.c`, `signal_utils.c`
    

---

### 10. `utils/`

**Responsabilidad:** Funciones de ayuda generales.

- `ft_strjoin`, `ft_split`, `ft_strlen`, manejo de errores, limpieza de memoria.
    

---

### 11. `memory/`

**Responsabilidad:** Gestión de memoria dinámica y limpieza.

- Limpia estructuras de parser, lexer, heredocs, etc.
    
- Evita memory leaks (clave para pasar `valgrind`).
    

**Archivos sugeridos:**

- `free_all.c`, `gc.c`, `leak_debug.c`
    

---

## 🔄 Relación entre módulos (resumen de flujo)

```
main → prompt → lexer → parser → expander → executor → builtins/env/signals → utils/memory
```

Cada módulo tiene responsabilidad única, lo que permite testearlos y desarrollarlos de forma aislada.

---
