## Componentes para Integrar

1. **Estructuras de Datos del AST** - Prioridad alta
   - Enum `t_ast_node_type`
   - Struct `t_ast_redirect`
   - Struct `t_ast_node`

2. **Funciones de Creación de Nodos** - Prioridad alta
   - `create_command_node`
   - `create_pipeline_node`
   - `create_redirect_node`

3. **Lógica de Construcción del AST** - Prioridad alta
   - `build_ast_from_tokens` (actualmente solo maneja comandos simples, necesita ampliación)

4. **Funciones de Gestión de Memoria** - Prioridad alta
   - `free_ast` (para limpieza adecuada)

5. **Funciones de Depuración/Visualización** - Prioridad media
   - `print_ast` (invaluable durante el desarrollo)

## Orden de Implementación

Se recomienda este orden para la integración:

1. **Primero: Completar el Lexer** (tokenizador)
   - Parece que ya está bastante avanzado según la documentación
   - Asegurar que los tokens se generan correctamente antes de construir el AST

2. **Segundo: Implementar el Parser con AST**
   - Añadir las estructuras AST a minishell.h
   - Crea un nuevo archivo como `src/parser/ast.c` para la implementación del AST
   - Implementar las funciones principales de creación de nodos

3. **Tercero: Ampliar la Funcionalidad del Parser**
   - Comenzar con comandos simples (que ya tiene)
   - Añadir soporte para pipelines
   - Añadir soporte para operadores lógicos (AND/OR)
   - Añadir soporte para subshells
   - Implementar cada uno en orden de precedencia según se detalla en AST_Orden_de_prioridades.md

4. **Cuarto: Reemplazar Detective Conan**
   - Reemplazar `detective_conan` con la función `execute_ast`
   - La implementación debería seguir AST_Diagrama_de_flujo.md

## Notas Importantes

1. **Parser vs AST**:
   - El parser va antes del AST en el flujo de procesamiento
   - El parser convierte tokens → estructura AST
   - El AST es luego ejecutado por los componentes del ejecutor

1. **Limitaciones del tester actual**:
   - Solo maneja comandos simples con redirecciones
   - Necesita ampliación para pipes, operadores AND/OR, etc.
   - La tarea sería implementar un análisis recursivo descendente basado en las reglas de precedencia

3. **Parser Recursivo Descendente**:
   - Seguir la estructura en AST_Orden_de_prioridades.md
   - Comenzar con operaciones de mayor precedencia primero (comandos)
   - Luego construir pipelines, luego operadores AND/OR

4. **Estrategia de Integración**:
   - Usar el lexer existente como entrada para el parser
   - Reemplaza la función `detective_conan` con `build_ast_from_tokens` → `execute_ast`
   - Pruebas incrementales comenzando con comandos simples

## Próximos Pasos

Basado en el progreso del proyecto, se recomienda:

1. Completar las estructuras de nodos AST y funciones de creación
2. Implementar un parser recursivo descendente siguiendo las reglas de precedencia
3. Comenzar con la ejecución de comandos simples a través del AST
4. Añadir manejo de pipelines después
5. Añadir operadores lógicos (AND/OR) al final

Este enfoque permitiría integrar el AST de forma incremental mientras se mantiene una shell funcional durante todo el proceso.