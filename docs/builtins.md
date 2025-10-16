# BUILT-INS EN MINISHELL - LÓGICA FUNDAMENTAL

---

## QUÉ SON LOS BUILT-INS

### Definición

**Built-in** = Comando ejecutado **dentro del proceso del shell**, sin crear proceso hijo.

**¿Por qué?**
Porque necesitan modificar el estado del shell padre:
- Variables de entorno
- Directorio actual
- Salir del shell

### Lógica de Detección

```
Después del parsing:
    ↓
¿cmd->tokens[0] es built-in?
├─ SÍ: Ejecutar en el proceso actual
│      → Modificar mini->env, cambiar directorio, etc.
│      → Actualizar mini->exit_sts
│      → return TRUE (no ir a executor)
└─ NO: return FALSE
       → Ir a executor
       → Crear fork + execve
```

**Código de decisión:**
```c
if (strcmp(cmd, "exit") == 0 || strcmp(cmd, "bye") == 0)
    → builtin_exit()
else if (strcmp(cmd, "cd") == 0)
    → builtin_cd()
else if (strcmp(cmd, "pwd") == 0)
    → builtin_pwd()
// ... etc
else
    → return false (no es built-in)
```

---

## ESTRUCTURA Y FLUJO

### Datos del Shell

```c
typedef struct s_mini
{
    char    **env;        // ["USER=john", "PWD=/home", ..., NULL]
    int     exit_sts;     // Último código de salida (para $?)
    char    **argv;       // Argumentos parseados
    t_cmd   *cmds;        // Comandos parseados
} t_mini;
```

### Flujo General

```
parse() → mini->cmds está listo
    ↓
built_ins(mini)
├─ ¿Es built-in?
│  ├─ SÍ: Ejecutar built-in
│  │      Actualizar mini->exit_sts
│  │      return TRUE
│  └─ NO: return FALSE
│
└─ executor(mini)  ← Solo si built_ins retornó FALSE
```

---

## EXIT

### Lógica

```
1. Imprimir "exit\n"
2. Verificar argumentos
3. Validar que sea número
4. Convertir a código de salida (módulo 256)
5. Limpiar memoria
6. exit()
```

### Casos

```bash
exit            → Salir con exit_sts actual
exit 42         → Salir con código 42
exit 300        → Salir con código 44 (300 % 256)
exit abc        → Error "numeric argument required", código 2, SALIR
exit 1 2        → Error "too many arguments", código 2, NO SALIR
```

**Lógica del código de salida:**
```
Argumento se convierte a número:
    Si es > 255 o < 0:
        exit_code = número % 256
    Ejemplo:
        300 → 300 % 256 = 44
        -1  → (-1 % 256) = 255
```

**¿Por qué "too many arguments" NO sale?**
Porque bash se comporta así:
```bash
$ exit 1 2
bash: exit: too many arguments
$ echo $?
1    # Sigue en el shell
```

### Flujo

```
¿Tiene 2+ argumentos después de "exit"?
├─ SÍ: Error, exit_sts=2, return (NO SALIR)
└─ NO: Continuar

¿Tiene 1 argumento?
├─ SÍ: ¿Es numérico?
│      ├─ SÍ: exit_sts = argumento % 256
│      └─ NO: Error, exit_sts=2
└─ NO: exit_sts = actual

cleanup_mini()
exit(exit_sts)
```

---

## CD

### Lógica

```
1. Determinar ruta destino
2. chdir(ruta)
3. Actualizar PWD y OLDPWD
```

### Casos

```bash
cd              → ir a $HOME
cd -            → ir a $OLDPWD, imprimir ruta
cd /ruta        → ir a ruta absoluta
cd dir          → ir a ruta relativa
cd a b          → Error "too many arguments"
```

### Flujo de Determinación de Ruta

```
¿Cuántos argumentos?
├─ 2+: Error "too many arguments", exit_sts=2, return
└─ 0 o 1: Continuar

¿Argumento es NULL?
├─ SÍ: path = getenv("HOME")
│      Si HOME no existe: error, exit_sts=1, return
└─ NO: ¿Argumento es "-"?
       ├─ SÍ: path = getenv("OLDPWD")
       │      Si OLDPWD no existe: error, exit_sts=1, return
       │      printf("%s\n", path)  ← Imprimir ruta
       └─ NO: path = argumento
```

### Actualización de Variables

```
ANTES de chdir():
    oldpwd = getenv("PWD")  → Guardar PWD actual

chdir(path):
├─ Éxito:
│   pwd = getcwd()
│   Si getcwd() falla:
│       pwd = get_logical_pwd(oldpwd, path)
│   
│   setenv("OLDPWD", oldpwd)
│   setenv("PWD", pwd)
│   exit_sts = 0
│
└─ Fallo:
    perror()
    exit_sts = 1
```

**get_logical_pwd:**
```
Si path es absoluto (/...):
    return path

Si path es ".":
    return oldpwd

Si path es relativo (dir):
    return oldpwd + "/" + path
```

**¿Por qué getcwd() puede fallar?**
Si el directorio fue eliminado:
```bash
$ mkdir test && cd test
$ rm -rf ../test  # Eliminar directorio actual
$ pwd             # getcwd() falla
$ cd ..           # Usar lógica con OLDPWD
```

---

## PWD

### Lógica Simple

```
pwd = getcwd()
├─ Éxito: printf("%s\n", pwd)
│         exit_sts = 0
└─ Fallo: Buscar en mini->pwd o getenv("PWD")
          Si existe: printf("%s\n", pwd)
                     exit_sts = 0
          Si no: error, exit_sts = 1
```

---

## ECHO

### Lógica

```
1. Detectar flag -n (múltiples: -nnn...)
2. Imprimir argumentos separados por espacio
3. Si NO hay -n: imprimir \n
```

### Flag -n

**Detección:**
```
Empezando en argv[1]:
    Mientras argv[i] empiece con "-n":
        ¿Todos los caracteres después del "-" son "n"?
        ├─ SÍ: flag = 1, i++
        └─ NO: break
    
Ejemplos:
    -n      → flag = 1
    -nnn    → flag = 1
    -n -n   → flag = 1 (ambos se saltan)
    -na     → flag = 0 (tiene "a", se imprime)
    -n test → flag = 1, imprime "test"
```

### Flujo

```
i = 1
flag = 0

Mientras argv[i] sea "-n" válido:
    flag = 1
    i++

Mientras argv[i]:
    printf("%s", argv[i])
    Si argv[i+1] existe:
        printf(" ")
    i++

Si flag == 0:
    printf("\n")

exit_sts = 0
```

---

## ENV

### Lógica

```
1. Verificar que NO haya argumentos
2. Imprimir solo variables con "=" (exportadas)
```

### Flujo

```
¿Hay argumentos?
├─ SÍ: Error "No such file or directory"
│      exit_sts = 127
│      return
└─ NO: Continuar

Para cada env[i]:
    Si env[i] contiene '=':
        printf("%s\n", env[i])

exit_sts = 0
```

**¿Por qué verificar '='?**
Porque `export VAR` (sin valor) crea variable NO exportada:
```bash
$ export TEST
$ env | grep TEST
(vacío)  ← No aparece

$ export TEST=value
$ env | grep TEST
TEST=value  ← Ahora sí aparece
```

---

## EXPORT

### Lógica

```
Sin argumentos: Mostrar todas las variables
Con argumentos: Validar y añadir/modificar
```

### Casos

```bash
export                      → Mostrar todo con formato declare -x
export VAR=value            → Crear/modificar variable
export VAR                  → Crear variable sin valor (no exportada)
export "VAR=value"          → Crear con valor
export 123=value            → Error "not a valid identifier"
export VAR-NAME=value       → Error (guión no válido)
```

### Validación de Nombres

**Reglas:**
```
Primer carácter:
    Debe ser letra (a-z, A-Z) o _

Resto de caracteres:
    Pueden ser letras, números o _

Ejemplos:
    VAR       → Válido
    VAR_123   → Válido
    _VAR      → Válido
    123VAR    → Inválido (empieza con número)
    VAR-NAME  → Inválido (contiene -)
    VAR NAME  → Inválido (espacio)
```

### Flujo

```
¿Sin argumentos?
├─ SÍ: Mostrar variables ordenadas
│      exit_sts = 0
│      return
└─ NO: Continuar

has_error = 0

Para cada argumento:
    ¿Empieza con '=' o es vacío?
    ├─ SÍ: Error, has_error = 1, continuar
    └─ NO: Continuar
    
    Validar cada carácter hasta '=' (si existe):
        ¿Es válido?
        ├─ NO: Error, has_error = 1, continuar
        └─ SÍ: Continuar
    
    ¿Tiene '='?
    ├─ SÍ: Dividir en nombre=valor
    │      setenv(nombre, valor)
    └─ NO: setenv(nombre, NULL)

exit_sts = has_error ? 1 : 0
```

### Formato declare -x

```bash
$ export VAR=hello TEST="world"

Salida:
declare -x TEST="world"
declare -x VAR="hello"

Formato:
    Variables sin valor:    declare -x VAR
    Variables con valor:    declare -x VAR="valor"
    Orden: alfabético
```

---

## UNSET

### Lógica

```
1. Para cada argumento:
   Buscar en env
   Si existe: eliminar
```

### Flujo

```
Para cada argumento:
    Para cada env[i]:
        ¿env[i] empieza con "argumento=" o "argumento\0"?
        ├─ SÍ: Eliminar env[i]
        │      Desplazar resto del array
        │      break
        └─ NO: Continuar

exit_sts = 0
```

**Desplazar array:**
```
Antes:
env[0] = "USER=john"
env[1] = "PWD=/home"
env[2] = "PATH=..."
env[3] = NULL

Eliminar env[1]:
env[0] = "USER=john"
env[1] = "PATH=..."     ← Desplazado
env[2] = NULL           ← Desplazado
```

**Código:**
```c
free(env[j]);
while (env[j + 1])
{
    env[j] = env[j + 1];
    j++;
}
env[j] = NULL;
```

---

## GESTIÓN DEL ENTORNO

### Estructura del Array

```c
char **env = [
    "USER=john",
    "HOME=/home/john",
    "PWD=/current/path",
    ...,
    NULL  ← Siempre termina en NULL
];
```

### get_local_env - Obtener valor

**Lógica:**
```
buscar("USER"):
    Para cada env[i]:
        ¿empieza con "USER="?
        ├─ SÍ: return puntero a lo que viene después de '='
        └─ NO: continuar
    
    return NULL (no existe)
```

**Ejemplo:**
```c
env[0] = "USER=john"
         ^^^^^ ^^^^
         name  valor

get_local_env("USER") → retorna puntero a "john"
```

### ft_setenv - Modificar/Crear variable

**Lógica:**
```
setenv("VAR", "value"):

1. Buscar si VAR ya existe en env
   ├─ SÍ: Reemplazar
   │      free(env[i])
   │      env[i] = "VAR=value"
   │      return
   └─ NO: Continuar

2. Expandir array:
   env_len = contar elementos
   new_env = malloc((env_len + 2) * sizeof(char*))
   
   Copiar punteros:
       new_env[0..env_len-1] = env[0..env_len-1]
   
   Añadir nueva:
       new_env[env_len] = "VAR=value"
       new_env[env_len+1] = NULL
   
   free(env)  ← Solo el array, no los strings
   env = new_env
```

**Caso especial - Variable sin valor:**
```c
setenv("VAR", NULL):
    → Crear "VAR" (sin '=')
    → Para EXPORT, no para ENV
```

### Memoria del Entorno

**CRÍTICO:**
```
env es un array de PUNTEROS:

env → [ptr0] → "USER=john"
      [ptr1] → "PWD=/home"
      [ptr2] → NULL

Al modificar:
    ✓ CORRECTO: free(env[i])
                env[i] = nuevo_string

    ✗ INCORRECTO: env = otro_array
                  (perdemos el array original)
```

**Expandir array:**
```
Antes:
env → [ptr0, ptr1, ptr2, NULL]

Después de añadir:
new_env → [ptr0, ptr1, ptr2, ptr3, NULL]
           (punteros copiados)  (nuevo)

free(env)  ← Solo libera el array
env = new_env
```

---

## CÓDIGOS DE SALIDA

### Valores Estándar

```
0       → Éxito
1       → Error general
2       → Error de sintaxis / mal uso
127     → Comando no encontrado
130     → Ctrl+C (SIGINT)
```

### ¿Cuándo actualizar exit_sts?

**Siempre al final del built-in:**
```c
void builtin_pwd(t_mini *mini)
{
    char *cwd = getcwd(NULL, 0);
    
    if (cwd)
    {
        printf("%s\n", cwd);
        free(cwd);
        mini->exit_sts = 0;  ← Éxito
    }
    else
    {
        perror("pwd");
        mini->exit_sts = 1;  ← Error
    }
}
```

### Acceso desde el Shell

```bash
$ pwd
/home/john
$ echo $?
0        ← mini->exit_sts

$ cd /noexiste
cd: /noexiste: No such file or directory
$ echo $?
1        ← mini->exit_sts

$ export 123=value
export: `123=value': not a valid identifier
$ echo $?
1        ← mini->exit_sts
```

---

## RESUMEN LÓGICO

### EXIT
- Validar argumentos (0, 1, o error)
- Convertir a código (% 256)
- Salir del shell

### CD
- Determinar ruta (HOME, OLDPWD, o argumento)
- chdir()
- Actualizar PWD y OLDPWD

### PWD
- getcwd() o usar PWD guardado
- Imprimir ruta

### ECHO
- Detectar -n (múltiples válidos)
- Imprimir argumentos con espacios
- Salto de línea si no hay -n

### ENV
- Verificar sin argumentos
- Imprimir solo variables con '='

### EXPORT
- Sin args: mostrar todo ordenado
- Con args: validar nombre y setenv

### UNSET
- Buscar variable en env
- Eliminar y desplazar array

### Gestión de Entorno
- Array dinámico de strings
- Búsqueda por prefijo "VAR="
- Expansión al añadir nuevas variables

---
