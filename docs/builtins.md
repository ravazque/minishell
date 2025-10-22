
# Builtins en Minishell

---

## Introducción

Los builtins son comandos que el shell ejecuta directamente sin crear un proceso hijo. Esto es necesario porque estos comandos necesitan modificar el estado interno del shell (variables de entorno, directorio actual, etc.).

**Lista de builtins en minishell**:
- `echo`: Imprime argumentos
- `cd`: Cambia el directorio de trabajo
- `pwd`: Muestra el directorio actual
- `export`: Define o muestra variables de entorno
- `unset`: Elimina variables de entorno
- `env`: Muestra todas las variables de entorno
- `exit`: Termina el shell

---

## Estructura General

### Identificación

```c
bool built_ins(t_mini *mini)
{
    char *cmd = mini->cmds->tokens[0];
    
    if (ft_strcmp(cmd, "echo") == 0)
        builtin_echo(mini);
    else if (ft_strcmp(cmd, "cd") == 0)
        builtin_cd(mini);
    // ... otros builtins
    
    return true;
}
```

**Proceso**:
1. Se verifica si el primer token coincide con un nombre de builtin
2. Se llama a la función específica del builtin
3. La función actualiza `mini->exit_sts` directamente

### Ejecución según Contexto

**Comando simple (sin pipes)**:
```
echo hello world
  ↓
El builtin se ejecuta en el proceso PADRE
  ↓
Modifica el estado del shell directamente
```

**En pipeline**:
```
echo hello | cd /tmp | pwd
           ↓        ↓
    (en hijo)  (en hijo)
```

Los builtins dentro de pipes se ejecutan en procesos hijos. Sus cambios (cd, export, etc.) NO afectan al shell padre.

**Por qué**: En un pipe, necesitamos conectar stdin/stdout entre comandos. Esto requiere hacer fork.

---

## echo

### Propósito
Imprime sus argumentos separados por espacio, seguidos de un salto de línea.

### Sintaxis
```bash
echo [opciones] [argumentos...]
```

### Opciones

**Flag -n**: Suprime el salto de línea final.

**Variantes válidas**:
- `-n`
- `-nn`
- `-nnn` (cualquier cantidad de 'n')

**Validación**:
```
Argumento empieza con "-n"
         ↓
¿Todos los siguientes caracteres son 'n'?
         ↓
    Sí: Es flag válido
    No: Es un argumento normal
```

**Ejemplo**:
```bash
echo -n hello        # Sin newline
echo -nnn world      # Sin newline (válido)
echo -na test        # Con newline (no es flag)
```

### Algoritmo

```
1. Iterar argumentos desde índice 1
   ↓
2. Mientras argumento sea flag -n válido:
   - Marcar flag = 1
   - Avanzar al siguiente argumento
   ↓
3. Para cada argumento restante:
   - Imprimir el argumento
   - Si hay más argumentos: imprimir espacio
   ↓
4. Si flag != 1:
   - Imprimir newline
```

### Implementación

```c
void builtin_echo(t_mini *mini)
{
    int i = 1;
    int flag = 0;
    
    // Procesar flags -n
    while (mini->cmds->tokens[i] && ft_n_flag(mini->cmds->tokens, i))
    {
        flag = 1;
        i++;
    }
    
    // Imprimir argumentos
    while (mini->cmds->tokens[i])
    {
        ft_putstr_fd(mini->cmds->tokens[i], STDOUT_FILENO);
        if (mini->cmds->tokens[i + 1])
            write(STDOUT_FILENO, " ", 1);
        i++;
    }
    
    // Newline si no hay flag
    if (!flag)
        write(STDOUT_FILENO, "\n", 1);
}
```

### Exit Status
Siempre 0 (éxito).

---

## cd

### Propósito
Cambia el directorio de trabajo del shell.

### Sintaxis
```bash
cd [directorio]
```

### Argumentos Especiales

**Sin argumentos o `~`**: Va al directorio HOME
**`-`**: Va al directorio anterior (OLDPWD)
**`..`**: Va al directorio padre

### Variables Involucradas

**HOME**:
- Se obtiene de `mini->env`
- Si no existe en env, se usa `mini->cd_home` (caché)
- Si ninguno existe, error

**OLDPWD**:
- Guarda el directorio anterior
- Se actualiza cada vez que cd tiene éxito

**PWD**:
- Guarda el directorio actual
- Se actualiza después de cada cd exitoso

### Estructura t_mini Relevante

```c
typedef struct s_mini
{
    char **env;        // Variables de entorno (incluye HOME, PWD, OLDPWD)
    char *cd_home;     // Caché del valor de HOME
    char *pwd;         // Directorio actual
    // ...
} t_mini;
```

**cd_home**: Se usa como respaldo si HOME desaparece del entorno.

### Algoritmo

```
1. Validar argumentos
   ↓
   ¿Hay más de un argumento?
   Sí → Error "too many arguments", exit_sts = 2
   ↓
2. Determinar path destino
   ↓
   Sin arg o "~" → HOME
   "-"          → OLDPWD (imprimir después)
   ".."         → Directorio padre del PWD
   Otro         → Ese path
   ↓
3. Intentar cambiar directorio
   ↓
   chdir(path)
   ↓
   ¿Éxito?
   No → Imprimir error según errno, exit_sts = 1, return
   ↓
4. Guardar OLDPWD
   ↓
   oldpwd = valor anterior de PWD
   ↓
5. Calcular nuevo PWD
   ↓
   Intenta getcwd()
   Si falla → Calcula lógicamente basado en path
   ↓
6. Actualizar variables de entorno
   ↓
   OLDPWD = oldpwd guardado
   PWD = nuevo pwd calculado
   ↓
7. exit_sts = 0
```

### Cálculo del Nuevo PWD

**Caso 1: getcwd() funciona**:
```c
char *pwd = getcwd(NULL, 0);
if (pwd)
    return pwd;  // Usar este valor
```

**Caso 2: getcwd() falla (directorio borrado)**:
Se calcula lógicamente:

```
Path absoluto (/tmp/dir) → Usar ese path
Path relativo (subdir)   → Juntar oldpwd + "/" + path
Path "."                 → Usar oldpwd sin cambios
```

### Manejo de Directorios Padres

```c
char *get_parent_from_pwd(t_mini *mini)
{
    char *pwd = get_local_env("PWD", mini->env);
    char *parent = ft_strdup(pwd);
    
    // Buscar último '/'
    char *last_slash = ft_strrchr(parent, '/');
    
    if (last_slash == parent)  // Caso "/algo" → "/"
        return ft_strdup("/");
    
    *last_slash = '\0';  // Truncar en el último /
    return parent;
}
```

### Caché de HOME

```c
char *update_home_cache(t_mini *mini, char *current)
{
    if (!current)  // HOME no existe
    {
        if (mini->cd_home)  // Usar caché
            return mini->cd_home;
        // Error
    }
    
    // Actualizar caché
    if (!mini->cd_home || ft_strcmp(current, mini->cd_home) != 0)
    {
        if (mini->cd_home)
            free(mini->cd_home);
        mini->cd_home = ft_strdup(current);
    }
    
    return mini->cd_home;
}
```

**Por qué caché**: Si `unset HOME`, cd sigue pudiendo usar el último valor conocido.

### Mensajes de Error

```c
void print_chdir_error(char *path, char *arg)
{
    ft_putstr_fd("minishell: cd: ", 2);
    
    // Casos especiales: mostrar el path real
    if (arg && !ft_strcmp(arg, "-"))
        ft_putstr_fd(path, 2);
    else if (arg && !ft_strcmp(arg, "~"))
        ft_putstr_fd(path, 2);
    else if (arg)
        ft_putstr_fd(arg, 2);
    else
        ft_putstr_fd("HOME", 2);
    
    // Mensaje según errno
    if (errno == EACCES)
        ft_putstr_fd(": Permission denied\n", 2);
    else if (errno == ENOTDIR)
        ft_putstr_fd(": Not a directory\n", 2);
    else
        ft_putstr_fd(": No such file or directory\n", 2);
}
```

**Ejemplo**:
```bash
cd ~  # Con HOME=/home/user
# Error: minishell: cd: /home/user: Permission denied
```

Muestra el path real, no el símbolo `~`.

### Exit Status

- `0`: Éxito
- `1`: Error al cambiar directorio
- `2`: Demasiados argumentos

---

## pwd

### Propósito
Muestra el directorio de trabajo actual.

### Sintaxis
```bash
pwd
```

No acepta opciones en minishell.

### Algoritmo

```
1. Intentar getcwd()
   ↓
   ¿Funciona?
   Sí → Imprimir y liberar
        exit_sts = 0
   ↓
2. Si falla, usar mini->pwd
   ↓
   ¿Existe mini->pwd?
   Sí → Imprimir
        exit_sts = 0
   ↓
3. Si tampoco existe:
   ↓
   Error "error retrieving current directory"
   exit_sts = 1
```

### Por qué mini->pwd

El shell mantiene un cache del directorio en `mini->pwd` que se actualiza en el prompt. Si el directorio físico fue borrado, `getcwd()` falla, pero podemos mostrar el path lógico.

### Implementación

```c
void builtin_pwd(t_mini *mini)
{
    char *cwd = getcwd(NULL, 0);
    
    if (cwd)
    {
        ft_putstr_fd(cwd, STDOUT_FILENO);
        ft_putstr_fd("\n", STDOUT_FILENO);
        free(cwd);
        mini->exit_sts = 0;
    }
    else if (mini->pwd)
    {
        ft_putstr_fd(mini->pwd, STDOUT_FILENO);
        ft_putstr_fd("\n", STDOUT_FILENO);
        mini->exit_sts = 0;
    }
    else
    {
        ft_putstr_fd("minishell: pwd: error retrieving current directory\n",
            STDERR_FILENO);
        mini->exit_sts = 1;
    }
}
```

### Exit Status

- `0`: Éxito
- `1`: No se pudo obtener el directorio

---

## export

### Propósito
Define variables de entorno o muestra todas las variables.

### Sintaxis
```bash
export [NOMBRE[=VALOR]]...
```

### Comportamientos

**Sin argumentos**:
```bash
export
```
Muestra todas las variables en formato `declare -x NOMBRE="VALOR"`.

**Con argumentos**:
```bash
export VAR=valor      # Define VAR con valor
export VAR            # Marca VAR como exportable (sin valor)
export VAR=           # Define VAR con valor vacío
```

### Validación de Nombres

**Reglas**:
1. Primer carácter debe ser letra o `_`
2. Resto pueden ser letras, números o `_`
3. No puede empezar con `=`

```
Válido:   VAR, _VAR, VAR123, VAR_NAME
Inválido: 123VAR, VAR-NAME, =VAR
```

### Algoritmo

```
1. ¿Sin argumentos?
   Sí → ft_putexport() y return
   ↓
2. Para cada argumento:
   ↓
   a) Validar nombre
      ↓
      ¿Empieza con '=' o está vacío?
      Sí → Error, continuar con siguiente
      ↓
      ¿Primer carácter es letra o '_'?
      No → Error, continuar
      ↓
      Para cada carácter hasta '=' o fin:
         ¿Es alfanumérico o '_'?
         No → Error, continuar
      ↓
   b) Si válido:
      ↓
      ¿Tiene '='?
      Sí → ft_split2() y ft_setenv()
      No → ft_setenv(nombre, NULL)
   ↓
3. exit_sts = 1 si hubo algún error, 0 si no
```

### Mostrar Variables (ft_putexport)

```c
void ft_putexport(char ***env)
{
    // Crear copia ordenada alfabéticamente
    char **abc_env = ft_sort_env(*env);
    
    // Para cada variable:
    for cada variable:
        // Saltar "_" (variable especial)
        if (empieza con "_=")
            continuar;
        
        // Formato según tenga '=' o no
        if (tiene '=')
            printf("declare -x NOMBRE=\"VALOR\"\n");
        else
            printf("declare -x NOMBRE\n");
}
```

**Ejemplo de salida**:
```
declare -x HOME="/home/user"
declare -x PATH="/usr/bin:/bin"
declare -x TEMP
```

### División de Argumento (ft_split2)

Divide en dos partes por el primer `=`:

```c
char **ft_split2(char *s, char c)
{
    // Encuentra el primer '='
    // Crea array de 3 elementos:
    // [0] = todo antes del '='
    // [1] = todo después del '='
    // [2] = NULL
}
```

**Ejemplo**:
```
Input:  "VAR=hello=world"
Output: ["VAR", "hello=world", NULL]
```

### Seteo de Variable (ft_setenv)

```c
void ft_setenv(char *name, char *value, char ***env)
{
    // Construir string "NAME=VALUE" o solo "NAME"
    char *var;
    if (value)
        var = ft_strjoin3(name, "=", value);
    else
        var = ft_strdup(name);
    
    // Buscar si existe
    for (i = 0; env[i]; i++)
    {
        if (coincide el nombre)
        {
            // Reemplazar
            free(env[i]);
            env[i] = var;
            return;
        }
    }
    
    // No existe, añadir al final
    // Expandir array
    // Añadir nueva variable
}
```

### Variable sin Valor

```bash
export VAR
```

**Resultado**: La variable existe en el entorno pero no tiene `=`:
```
En env: "VAR" (sin '=')
```

Aparece en `export` pero no en `env`.

### Exit Status

- `0`: Éxito
- `1`: Algún identificador no válido

---

## unset

### Propósito
Elimina variables de entorno.

### Sintaxis
```bash
unset NOMBRE...
```

### Algoritmo

```
Para cada argumento:
  ↓
  Para cada variable en env:
    ↓
    ¿El nombre coincide?
    Sí → Eliminar variable
         Desplazar resto del array
         Poner NULL al final
         break
```

### Implementación

```c
void builtin_unset(t_mini *mini)
{
    int i = 1;
    
    while (mini->cmds->tokens[i])  // Para cada argumento
    {
        int j = 0;
        int len = ft_strlen(mini->cmds->tokens[i]);
        
        while (mini->env[j])
        {
            // ¿Coincide el nombre?
            if (!ft_strncmp(mini->cmds->tokens[i], mini->env[j], len) &&
                (mini->env[j][len] == '\0' || mini->env[j][len] == '='))
            {
                // Eliminar
                free(mini->env[j]);
                
                // Desplazar resto
                while (mini->env[j + 1])
                {
                    mini->env[j] = mini->env[j + 1];
                    j++;
                }
                mini->env[j] = NULL;
                break;
            }
            j++;
        }
        i++;
    }
    
    mini->exit_sts = 0;
}
```

### Desplazamiento del Array

```
ANTES:
env: [HOME] [PATH] [USER] [NULL]
                     ↑
               Eliminar USER

DESPUÉS:
1. free(env[2])
2. env[2] = env[3]  → env[2] = NULL
3. env[3] = NULL

Resultado:
env: [HOME] [PATH] [NULL] [NULL]
                   ↑
             Nuevo final
```

### Exit Status
Siempre 0 (éxito).

---

## env

### Propósito
Muestra todas las variables de entorno exportadas.

### Sintaxis
```bash
env
```

### Comportamiento

**Sin argumentos**: Muestra todas las variables que tienen `=`.

**Con argumentos**: Error (minishell no implementa ejecutar comandos con env modificado).

### Algoritmo

```
1. ¿Hay argumentos?
   Sí → Error "No such file or directory"
        exit_sts = 127
        return
   ↓
2. Para cada variable en env:
   ↓
   ¿Tiene '='?
   Sí → Imprimir variable completa con newline
   No → Saltar (no exportada)
   ↓
3. exit_sts = 0
```

### Diferencia con export

**env**:
```
HOME=/home/user
PATH=/usr/bin
```

**export**:
```
declare -x HOME="/home/user"
declare -x PATH="/usr/bin"
declare -x VAR
```

`env` solo muestra variables con valor. `export` muestra todas.

### Implementación

```c
void builtin_env(t_mini *mini)
{
    if (mini->cmds->tokens[1])
    {
        ft_putstr_fd("minishell: env: '", STDERR_FILENO);
        ft_putstr_fd(mini->cmds->tokens[1], STDERR_FILENO);
        ft_putstr_fd("': No such file or directory\n", STDERR_FILENO);
        mini->exit_sts = 127;
        return;
    }
    
    int i = 0;
    while (mini->env[i])
    {
        if (ft_strchr(mini->env[i], '='))
        {
            ft_putstr_fd(mini->env[i], STDOUT_FILENO);
            ft_putstr_fd("\n", STDOUT_FILENO);
        }
        i++;
    }
    
    mini->exit_sts = 0;
}
```

### Exit Status

- `0`: Éxito
- `127`: Se pasaron argumentos

---

## exit

### Propósito
Termina el shell con un código de salida.

### Sintaxis
```bash
exit [n]
```

### Comportamientos

**Sin argumentos**: Sale con el último exit status.
**Con un argumento numérico**: Sale con ese código (módulo 256).
**Con argumento no numérico**: Error y sale con código 2.
**Con múltiples argumentos**: Error pero NO sale.

### Validación de Argumento

```c
int ft_atoi_exit(const char *s, bool *max_ex)
```

**Valida**:
1. Espacios iniciales
2. Signo opcional (+/-)
3. Solo dígitos después
4. Sin caracteres adicionales
5. Valor dentro de LLONG_MIN a LLONG_MAX

**Si inválido**: `*max_ex = true`.

### Cálculo del Código de Salida

```
Valor ingresado → Módulo 256

Ejemplos:
256  → 0
257  → 1
-1   → 255
-256 → 0
```

### Algoritmo

```
1. Imprimir "exit\n"
   ↓
2. ¿Hay argumento?
   No → Salir con mini->exit_sts actual
   ↓
3. Validar argumento con ft_atoi_exit()
   ↓
   ¿Válido?
   No → Error "numeric argument required"
        mini->exit_sts = 2
        return (NO sale)
   ↓
4. mini->exit_sts = valor módulo 256
   ↓
5. ¿Hay segundo argumento?
   Sí → Error "too many arguments"
        mini->exit_sts = 2
        return (NO sale)
   ↓
6. cleanup_mini()
   exit(mini->exit_sts)
```

### Implementación

```c
void builtin_exit(t_mini *mini)
{
    bool max_ex = false;
    
    write(STDOUT_FILENO, "exit\n", 5);
    
    if (mini->cmds->tokens[1])
    {
        mini->exit_sts = ft_atoi_exit(mini->cmds->tokens[1], &max_ex);
        
        if (max_ex == true)
        {
            write(STDERR_FILENO, "minishell: exit: ", 17);
            ft_putstr_fd(mini->cmds->tokens[1], STDERR_FILENO);
            write(STDERR_FILENO, ": numeric argument required\n", 28);
            mini->exit_sts = 2;
            return;
        }
    }
    
    if (mini->cmds->tokens[1] && mini->cmds->tokens[2])
    {
        ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
        mini->exit_sts = 2;
        return;
    }
    
    cleanup_mini(mini);
    exit(mini->exit_sts);
}
```

### Casos Especiales

**En pipeline**:
```bash
exit | echo hello
```

El `exit` se ejecuta en un hijo, termina ese proceso pero no el shell.

**Con argumento muy grande**:
```bash
exit 9223372036854775808  # Overflow
# Error: numeric argument required
```

### Exit Status

El código que se usa al salir, o:
- `2`: Argumento no numérico o demasiados argumentos

---

## Utilidades Compartidas

### ft_argc

```c
int ft_argc(char **argv)
{
    int i = 0;
    while (argv[i])
        i++;
    return i;
}
```

Cuenta los argumentos en un array de strings.

### get_local_env

```c
char *get_local_env(const char *name, char **env)
{
    int i = 0;
    int n_len = ft_strlen(name);
    
    while (env[i])
    {
        if (!ft_strncmp(env[i], name, n_len) && env[i][n_len] == '=')
            return (env[i] + n_len + 1);  // Retorna el valor
        i++;
    }
    
    return NULL;
}
```

**Busca una variable en el entorno y retorna su valor**.

**Ejemplo**:
```c
char *home = get_local_env("HOME", mini->env);
// Si env tiene "HOME=/home/user"
// Retorna: "/home/user"
```

### ft_setenv

Ya explicada en export. Usada por múltiples builtins para actualizar variables.

```c
void ft_setenv(char *name, char *value, char ***env)
```

**Parámetros**:
- `name`: Nombre de la variable
- `value`: Valor (puede ser NULL)
- `env`: Puntero al array de entorno

**Comportamiento**:
- Si existe: Reemplaza
- Si no existe: Añade al final (expandiendo el array)

---

## Interacción entre Builtins

### cd actualiza PWD y OLDPWD
```bash
cd /tmp
# Llama a ft_setenv("PWD", "/tmp", &mini->env)
# Llama a ft_setenv("OLDPWD", "/old/path", &mini->env)
```

### export puede afectar cd
```bash
export HOME=/new/home
cd ~
# Usa el nuevo valor de HOME
```

### unset puede romper cd
```bash
unset HOME
cd ~
# Error: minishell: cd: HOME not set
```

Pero si `mini->cd_home` tiene un valor cacheado, cd lo usa.

---

## Estructura en Memoria

Durante la ejecución de un builtin:

```
mini (t_mini)
  │
  ├─ env: ["HOME=/home/user", "PATH=/usr/bin", ..., NULL]
  │        ↑
  │        Los builtins leen y modifican este array
  │
  ├─ cmds: t_cmd
  │         │
  │         └─ tokens: ["cd", "/tmp", NULL]
  │                     ↑
  │                     Los builtins leen argumentos de aquí
  │
  ├─ exit_sts: Actualizado por cada builtin
  │
  └─ cd_home: Caché de HOME para cd
```

---

## Resumen de Exit Status

```
Builtin  | Éxito | Error
---------|-------|------------------
echo     |   0   | N/A
cd       |   0   | 1 (error), 2 (args)
pwd      |   0   | 1 (no se puede obtener)
export   |   0   | 1 (identificador inválido)
unset    |   0   | N/A
env      |   0   | 127 (con argumentos)
exit     |  n%256| 2 (arg inválido)
```

---

## Diagrama de Flujo General

```
Usuario escribe comando
        ↓
    [Parseo]
        ↓
mini->cmds->tokens[0] contiene nombre del comando
        ↓
    [Executor]
        ↓
¿Es builtin?
    │
    ├─ Sí ─→ [built_ins()]
    │         │
    │         └─→ Identifica cuál
    │              │
    │              ├─ echo    ──→ Imprime args
    │              ├─ cd      ──→ Cambia dir, actualiza env
    │              ├─ pwd     ──→ Imprime dir
    │              ├─ export  ──→ Modifica env
    │              ├─ unset   ──→ Elimina de env
    │              ├─ env     ──→ Imprime env
    │              └─ exit    ──→ Limpia y sale
    │                           │
    │                           └─→ Actualiza mini->exit_sts
    │
    └─ No ─→ [Comando externo]
             │
             └─→ fork + execve
```
