
Este documento describe los built-ins que deben implementarse en el proyecto Minishell. Incluye descripción, comportamiento esperado, ejemplos, notas de implementación y consejos útiles.

---

## 📜 Built-ins requeridos por el subject

### 1. `echo`

**Descripción**: Imprime los argumentos dados, seguidos por un salto de línea, a menos que se use `-n`.

**Sintaxis**:

```bash
echo [-n] [arg ...]
```

**Ejemplos**:

```bash
echo hola mundo          # hola mundo
echo -n hola mundo       # hola mundo (sin salto de línea)
```

**Consejos**:

- Soportar múltiples `-n` juntos (`-nnnn` también debe funcionar).
    

---

### 2. `cd`

**Descripción**: Cambia el directorio de trabajo actual.

**Sintaxis**:

```bash
cd [dir]
```

**Ejemplos**:

```bash
cd /tmp      # Cambia a /tmp
cd           # Cambia al valor de $HOME
```

**Consejos**:

- Actualiza `PWD` y `OLDPWD`.
    
- Manejar errores como directorio inexistente.
    

---

### 3. `pwd`

**Descripción**: Imprime el directorio actual.

**Sintaxis**:

```bash
pwd
```

**Ejemplo**:

```bash
pwd          # /home/usuario
```

**Consejos**:

- Utiliza `getcwd()` para obtener la ruta.
    

---

### 4. `export`

**Descripción**: Asigna una variable de entorno o la muestra si no se dan argumentos.

**Sintaxis**:

```bash
export [VAR[=value]]
```

**Ejemplos**:

```bash
export VAR=42        # Asigna una variable
export               # Muestra todas las variables exportadas
```

**Consejos**:

- Ordenar alfabéticamente si se imprimen las variables.
    
- Validar nombres válidos: empiezan con letra o '_', luego letras, números o '_'.
    

---

### 5. `unset`

**Descripción**: Elimina variables de entorno o de shell.

**Sintaxis**:

```bash
unset VAR
```

**Ejemplo**:

```bash
unset PATH          # Elimina la variable PATH
```

**Consejos**:

- No hacer nada si la variable no existe.
    

---

### 6. `env`

**Descripción**: Imprime las variables de entorno activas.

**Sintaxis**:

```bash
env
```

**Ejemplo**:

```bash
env            # Muestra las variables exportadas
```

**Consejos**:

- Solo mostrar variables con valor asignado (i.e. con `=`).
    
- No acepta argumentos (según subject).
    

---

### 7. `exit`

**Descripción**: Sale del shell con un código de salida opcional.

**Sintaxis**:

```bash
exit [número]
```

**Ejemplos**:

```bash
exit         # Sale con el código del último comando
exit 42      # Sale con código 42
```

**Consejos**:

- Validar que el argumento sea numérico.
    
- Si hay más de un argumento, mostrar error: `exit: too many arguments`.
    
- Llamar a `exit()` desde el proceso principal.
    

---

## 🛠️ Consejos generales de implementación

- **Built-ins vs comandos externos**:
    
    - Si es un built-in, ejecutarlo **sin `execve`**.
        
    - Si está en un pipeline, usualmente se ejecuta en un **subproceso**.
        
- **`cd`, `export`, `unset`, `exit`** deben ejecutarse **en el proceso padre** si están solos, porque modifican el entorno del shell principal.
    
- Considerar si el comando está solo o dentro de una tubería (`|`). Si está en pipe, se ejecuta en un fork (no puede modificar el entorno del padre).
    
- Centralizar la verificación con una función tipo `is_builtin(cmd)` y luego un `exec_builtin(cmd)`.
    

---

## ✅ Checklist de tests sugeridos

```bash
# echo
./minishell -c "echo hola mundo"
./minishell -c "echo -n hola"

# cd
./minishell -c "cd /tmp && pwd"
./minishell -c "cd && pwd"

# pwd
./minishell -c "pwd"

# export
./minishell -c "export VAR=42 && echo $VAR"
./minishell -c "export"

# unset
./minishell -c "unset VAR && echo $VAR"

# env
./minishell -c "env"

# exit
./minishell -c "exit 42"
```

---

Este documento sirve como referencia para implementar correctamente los built-ins requeridos por el proyecto Minishell. Probar cada uno en distintos contextos: solo, en pipelines, con errores y con múltiples argumentos.