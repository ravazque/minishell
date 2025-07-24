# Comportamiento esperado: 

Este documento explica cómo debe comportarse el minishell al manejar redirecciones, pipes y sub-shells. Incluye definiciones, ejemplos y pruebas asociadas.

---

## 📁 Redirecciones

### 🔹 Tipos

|Símbolo|Función|
|--:|---|
|`>`|Redirecciona la salida estándar (stdout) a un archivo (sobrescribe).|
|`>>`|Redirecciona la salida estándar al final del archivo (append).|
|`<`|Redirecciona la entrada estándar (stdin) desde un archivo.|
|`<<`|Here-document: lee de la entrada estándar hasta un delimitador.|

### 🧪 Pruebas sugeridas

```bash
# Prueba 1: Redirección de salida
echo "Hola mundo" > out.txt
cat out.txt    # Debería imprimir: Hola mundo

# Prueba 2: Redirección de salida con append
echo "Linea 1" > file.txt
echo "Linea 2" >> file.txt
cat file.txt   # Debería imprimir ambas líneas

# Prueba 3: Redirección de entrada
cat < file.txt # Debería imprimir el contenido de file.txt

# Prueba 4: Here-document sin comillas (expande variables)
export VAR="Hola"
cat << EOF
$VAR mundo
EOF
# Debería imprimir: Hola mundo

# Prueba 5: Here-document con comillas (no expande)
cat << 'EOF'
$VAR mundo
EOF
# Debería imprimir: $VAR mundo
```

### 🛠️ Consideraciones de implementación

- Usar `open()` y `dup2()` para redirigir `stdin` o `stdout`.
    
- Cerrar descriptores que no se usan.
    
- Eliminar archivos temporales en el caso del `<<`.
    

---

## 🔗 Pipes

### 🔹 Qué son

Conectan la salida de un comando con la entrada de otro.

```bash
ls -l | grep ".c" | wc -l
```

### 🧪 Pruebas sugeridas

```bash
# Prueba 1: Pipeline simple
echo "uno\ndos\ntres" | wc -l
# Esperado: 3

# Prueba 2: Múltiples pipes
cat /etc/passwd | grep root | wc -l
# Debería contar las líneas que contienen "root"

# Prueba 3: Pipeline con errores
echo hola | comando_inexistente
# Debería mostrar error, pero no colapsar el shell
```

### 🛠️ Consideraciones de implementación

- Crear un pipe (`pipe()`), luego hacer `fork()` por cada comando.
    
- Redirigir `stdin` y `stdout` usando `dup2()`.
    
- Cerrar correctamente extremos de pipes para evitar fugas.
    

---

## 🌀 Sub-shells

### 🔹 Qué son

Un sub-shell ejecuta comandos en un nuevo contexto de proceso.

```bash
(cd /tmp && ls)
pwd
```

### 🧪 Pruebas sugeridas

```bash
# Prueba 1: Sub-shell con cambio de directorio
echo "Antes: $(pwd)"
(cd /tmp && echo "Dentro: $(pwd)")
echo "Después: $(pwd)"
# Esperado: el cambio a /tmp no afecta el directorio actual

# Prueba 2: Sub-shell con pipe dentro
(cd /bin | grep bash)
# Debería mostrar salida sin error

# Prueba 3: Variables en sub-shell
export VAR=42
( export VAR=99; echo $VAR )
echo $VAR
# Esperado: 99 dentro del sub-shell, 42 fuera
```

### 🛠️ Consideraciones de implementación

- Detectar paréntesis `(` `)` como delimitadores de sub-shell.
    
- Ejecutar el bloque dentro de un `fork()` y tratarlo como si fuera un nuevo minishell.
    

---

## ✅ Conclusión

Este documento cubre el comportamiento esperado y las pruebas necesarias para garantizar el funcionamiento correcto del minishell en lo relativo a:

- Redirecciones (`>`, `>>`, `<`, `<<`)
    
- Pipes (`|`)
    
- Sub-shells (`(comando)`)
    

Estas pruebas pueden formar parte del conjunto de pruebas manuales o automatizadas del proyecto.