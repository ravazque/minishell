
Este documento define los requisitos que debe cumplir el proyecto Minishell según los criterios del subject y el comportamiento esperado de un shell.

---

## ✅ Requisitos Funcionales

Los requisitos funcionales definen lo que el programa **debe hacer**.

### 1. Interpretación de comandos

- El minishell debe aceptar líneas de comandos introducidas por el usuario.
    
- Debe separar los comandos, argumentos, operadores (`|`, `>`, `<`, etc.) correctamente.
    

### 2. Ejecución de comandos

- Ejecutar comandos externos que estén disponibles en el `PATH`.
    
- Ejecutar comandos built-in implementados por el propio minishell.
    

### 3. Redirecciones

- Soportar redirección de entrada (`<`) y salida (`>`, `>>`).
    
- Implementar here-doc (`<<`) correctamente.
    

### 4. Pipes

- Conectar múltiples comandos usando el operador `|`.
    
- Manejar múltiples niveles de piping (`cmd1 | cmd2 | cmd3`).
    

### 5. Variables de entorno

- Leer las variables de entorno iniciales.
    
- Implementar `export`, `unset` y expansión de variables (`$VAR`).
    

### 6. Soporte de comillas

- Manejar comillas simples (`'`) y dobles (`"`).
    
- Respetar las reglas de expansión dentro de cada tipo de comilla.
    

### 7. Sub-shells

- Ejecutar comandos entre paréntesis en un subproceso sin afectar el entorno del padre.
    

### 8. Manejo de señales

- Capturar `Ctrl+C` (SIGINT) para no cerrar el shell.
    
- Ignorar `Ctrl+\` (SIGQUIT) si no hay ejecución activa.
    
- Mostrar un prompt limpio tras interrumpir.
    

### 9. Built-ins

- Implementar correctamente: `cd`, `echo`, `pwd`, `export`, `unset`, `env`, `exit`.
    

### 10. Soporte de errores

- Detectar errores sintácticos (comillas sin cerrar, pipes inválidos, etc.).
    
- Mostrar mensajes claros sin cerrar el minishell.
    

---

## ⚙️ Requisitos No Funcionales

Los requisitos no funcionales definen **cómo** debe comportarse el programa, independientemente de su funcionalidad.

### 1. Portabilidad

- El minishell debe compilarse y funcionar correctamente en entornos Linux compatibles (p. ej. Debian/Ubuntu).
    

### 2. Normas de codificación

- El código debe cumplir con las normas de estilo de 42 (Norminette).
    
- Máximo de 25 líneas por función (según la norminette).
    

### 3. Gestión de memoria

- No debe haber memory leaks, double free, o invalid reads.
    
- Verificado con herramientas como `valgrind`.
    

### 4. Modularidad

- Dividir el proyecto en módulos claros: lexer, parser, ejecutor, built-ins, etc.
    

### 5. Estabilidad

- El shell debe ser robusto ante entradas erróneas o interrupciones.
    
- Debe poder ejecutarse indefinidamente sin colapsar.
    

### 6. Interfaz de usuario

- Mostrar un prompt personalizado (ej: `minishell$` ).
    
- Lectura con `readline`, historial activado.
    

### 7. Eficiencia razonable

- Cierre correcto de descriptores y procesos.
    

---

Este documento debe ser consultado regularmente durante el desarrollo para asegurar el cumplimiento de los objetivos del proyecto Minishell y servir como parte de la documentación final.