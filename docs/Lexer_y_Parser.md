El lexer(o tokenizer) y el parser permiten interpretar la entrada del usuario de manera estructurada y precisa. Cada uno cumple funciones muy específicas y se complementan para transformar una línea de comandos en una estructura de datos que el programa pueda ejecutar.

---

### El Lexer

El **lexer** es el encargado de leer la cadena de entrada y segmentarla en unidades mínimas llamadas *tokens*. Cada token representa un fragmento semántico relevante: puede ser un comando, un argumento, un símbolo para redirecciones (`>`, `<`), pipes (`|`), o incluso operadores especiales y delimitadores. Algunas de sus tareas principales son:

- **Separación de tokens:** Analiza la cadena caracter a caracter para identificar las secuencias válidas. Por ejemplo, al recibir `echo "Hola Mundo"`, el lexer debe generar un token para `echo` y otro para `"Hola Mundo"`, considerando el manejo correcto de las comillas.
- **Gestión de comillas y escapes:** Debe distinguir entre comillas simples y dobles, permitiendo que los caracteres dentro de ellas se consideren literalmente o se expandan variables, según corresponda. Los caracteres de escape (`\`) agregan complejidad, ya que deben permitir que ciertos caracteres se traten de forma especial.
- **Manejo de espacios y separadores:** No todos los espacios solo sirven como separadores; a veces forman parte del contenido de un argumento, especialmente cuando están delimidados por comillas.
  
El lexer podría recorrer la cadena de entrada con un índice o un puntero, creando nodos o estructuras para cada token identificado.

---

### El Parser

Una vez que el lexer ha generado la lista de tokens, entra en juego el **parser**. Su objetivo es interpretar la secuencia de tokens y construir una estructura de datos (a menudo similar a un árbol sintáctico abstracto o una lista encadenada) que represente la intención del usuario. Algunas de sus funciones son:

- **Interpretar la sintaxis:** Convierte la secuencia lineal de tokens en comandos ejecutables, identificando la relación entre ellos. Por ejemplo, en una línea como `ls -l | grep ".c" > salida.txt`, el parser debe reconocer que existe un pipe que conecta dos comandos y que hay una redirección de salida en el segundo comando.
- **Jerarquización de operaciones:** Establece la precedencia entre diferentes operadores, como redirecciones y pipes.
- **Validación de la sintaxis:** Identifica y maneja errores sintácticos. Si se encuentra un token inesperado o una secuencia inválida, el parser debe avisar al usuario con un mensaje de error, evitando ejecuciones peligrosas o confusas.
  
El parser suele trabajar de manera recursiva o por medio de una tabla de análisis que le permita identificar patrones en la secuencia de tokens. Por ejemplo, puede existir una función para manejar los comandos y otra para manejar las estructuras más complejas como sub-shells o agrupaciones de comandos.

---

### Ejemplo Práctico

Imagina la siguiente entrada:

```bash
cat file.txt | grep "texto" > output.txt
```

1. **En el Lexer:**
   - Se identifican tokens como:
     - `cat`
     - `file.txt`
     - `|`
     - `grep`
     - `"texto"`
     - `>`
     - `output.txt`

2. **En el Parser:**
   - Se interpreta que `cat file.txt` es el primer comando.
   - El símbolo `|` indica un pipe, conectando el output de `cat` con el siguiente comando.
   - `grep "texto"` se entiende como el comando que filtra la salida.
   - Finalmente, `>` redirige la salida filtrada hacia `output.txt`.

---

### Consideraciones

- **Ambigüedad y errores:** El parser debe ser lo suficientemente inteligente para detectar ambigüedades o errores de sintaxis y ofrecer retroalimentación útil. Un token mal formado o una secuencia inválida (como una comilla sin cerrar) podrían llevar a un comportamiento inesperado si no se validan correctamente.
  
- **Modularidad y escalabilidad:** Separar las funciones del lexer y del parser permite no solo una implementación más clara y mantenible, sino también mejoras futuras (por ejemplo, la adición de nuevas funcionalidades o el manejo de constructos más complejos).

- **Integración con el resto del Minishell:** Los datos estructurados que genera el parser se usan posteriormente para lanzar procesos, gestionar señales y administrar redirecciones.

---
