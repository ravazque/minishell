# Análisis de Casos de Uso

Este documento recopila y describe una serie de **casos de uso clave** para validar el funcionamiento del Minishell, tanto en escenarios simples como en situaciones límite. Cada caso incluye una descripción, comando de ejemplo y lo que se espera del comportamiento del shell.

---

## 🟢 Casos Simples

### 1. Ejecutar un comando externo sin argumentos

```bash
ls
```

**Esperado**: Se ejecuta `ls` y muestra el contenido del directorio actual.

---

### 2. Ejecutar un built-in simple

```bash
pwd
```

**Esperado**: Muestra la ruta del directorio actual.

---

### 3. Comando con argumentos

```bash
echo hola mundo
```

**Esperado**: Imprime `hola mundo` con un salto de línea.

---

### 4. Redirección de salida

```bash
echo hola > salida.txt
```

**Esperado**: Crea o sobrescribe `salida.txt` con el texto `hola`.

---

### 5. Redirección de entrada

```bash
cat < archivo.txt
```

**Esperado**: Muestra el contenido de `archivo.txt` si existe.

---

### 6. Pipe entre dos comandos

```bash
ls | grep minishell
```

**Esperado**: Lista archivos que contengan `minishell`.

---

### 7. Expansión de variables

```bash
echo $HOME
```

**Esperado**: Muestra la ruta del home del usuario.

---

### 8. Uso de comillas

```bash
echo "Hola $USER"
echo 'Hola $USER'
```

**Esperado**:

- Con comillas dobles, expande `$USER`.
    
- Con comillas simples, imprime `$USER` literalmente.
    

---

## 🟡 Casos Compuestos

### 1. Pipe + redirección

```bash
cat archivo.txt | grep hola > resultado.txt
```

**Esperado**: El resultado del `grep` se guarda en `resultado.txt`.

---

### 2. Comando con múltiples redirecciones

```bash
grep error < logs.txt >> errores.txt
```

**Esperado**: Lee de `logs.txt` y agrega resultados a `errores.txt`.

---

### 3. Sub-shell

```bash
(cd /tmp && ls)
pwd
```

**Esperado**: El cambio de directorio no afecta al shell principal.

---

### 4. Here-document

```bash
cat << EOF
línea 1
línea 2
EOF
```

**Esperado**: Imprime las líneas hasta encontrar `EOF`.

---

## 🔴 Casos Límite y de Error

### 1. Comillas sin cerrar

```bash
echo "Hola
```

**Esperado**: Detecta error de sintaxis y muestra mensaje claro.

---

### 2. Pipe al inicio o al final

```bash
| ls
ls |
```

**Esperado**: Error de sintaxis, no se ejecuta.

---

### 3. Comando inexistente

```bash
comando_que_no_existe
```

**Esperado**: Error: comando no encontrado.

---

### 4. Redirección sin archivo

```bash
cat >
```

**Esperado**: Error de sintaxis: falta nombre de archivo.

---

### 5. Variable no definida

```bash
echo $VARIABLE_INEXISTENTE
```

**Esperado**: Imprime una línea vacía (sin error).

---

Este análisis de casos de uso permite validar que el Minishell cumpla con los requisitos funcionales esperados y tenga un comportamiento robusto tanto en uso regular como ante entradas incorrectas.