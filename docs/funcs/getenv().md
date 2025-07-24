## 🧩 `getenv()`  
<small><code>#include &lt;stdlib.h&gt;</code></small>

---

### 🧾 Header
```c
#include <stdlib.h>
```

---

### 🧪 Prototipo
```c
char *getenv(const char *name);
```

---

### 📚 Descripción
Busca en el entorno del proceso la variable de entorno especificada por `name` y devuelve un puntero a la cadena de valor asociada. Las variables de entorno son pares nombre-valor que proporcionan información sobre el entorno en el que se ejecuta el programa.

La función devuelve un puntero al valor de la variable de entorno solicitada, o `NULL` si la variable no existe en el entorno.

---

### 🧰 Sintaxis y ejemplo
```c
char *home_dir = getenv("HOME");
if (home_dir)
{
    printf("Tu directorio home es: %s\n", home_dir);
}
else
{
    printf("Variable HOME no encontrada\n");
}
```

<summary>🔍 Ejemplo de uso en contexto (Búsqueda de comandos en PATH)</summary>

```c
#include <limits.h> /* for PATH_MAX */

char *find_command_in_path(const char *cmd)
{
    char *path_env, *path_copy, *dir, *result;
    char full_path[PATH_MAX];
    
    // Obtener la variable PATH del entorno
    path_env = getenv("PATH");
    if (!path_env)
        return (NULL);  // PATH no definido
    
    // Hacer una copia ya que strtok modifica la cadena
    path_copy = strdup(path_env);
    if (!path_copy)
        return (NULL);
    
    // Iterar por cada directorio en PATH
    dir = strtok(path_copy, ":");
    while (dir)
    {
        // Construir ruta completa
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, cmd);
        
        // Verificar si existe y es ejecutable
        if (access(full_path, X_OK) == 0)
        {
            result = strdup(full_path);
            free(path_copy);
            return (result);
        }
        
        dir = strtok(NULL, ":");
    }
    
    free(path_copy);
    return (NULL);  // Comando no encontrado
}
```

---

### 💡 Consejos de uso
- **No modifiques** la cadena devuelta por `getenv()`, ya que apunta directamente al entorno del proceso.
- Si necesitas modificar el valor, primero **realiza una copia** con `strdup()` o una función similar.
- Verifica siempre si `getenv()` devuelve `NULL` antes de usar el resultado.
- Para **establecer variables de entorno**, usa las funciones `setenv()` o `putenv()`.
- Ten en cuenta que `getenv()` no es seguro para hilos (thread-safe) en todas las implementaciones.

---

### 🔙 Retorno
Devuelve un puntero a la cadena de valor de la variable de entorno.  
Devuelve `NULL` si la variable no existe en el entorno.

---

### ⚠️ Errores
- No establece `errno` en caso de error (devolver `NULL` cuando la variable no existe).
- La función siempre será exitosa a menos que `name` sea `NULL`, en cuyo caso el comportamiento es indefinido.

---

### 🧭 Información adicional

<summary>📎 ¿Por qué se usa en Minishell?</summary>

En Minishell, `getenv()` es fundamental para diversas funcionalidades:

1. **Acceso a variables de entorno**: El shell necesita acceder constantemente a variables como `HOME`, `PATH`, `USER`, etc.

2. **Implementación del comando `cd`**: Para navegar al directorio home:

```c
int builtin_cd(char **args)
{
    char *target_dir;
    
    if (!args[1] || strcmp(args[1], "~") == 0)
    {
        target_dir = getenv("HOME");
        if (!target_dir)
        {
            fprintf(stderr, "cd: HOME no está definido\n");
            return (1);
        }
    }
    else
        target_dir = args[1];
    
    if (chdir(target_dir) != 0)
    {
        perror("cd");
        return (1);
    }
    return (0);
}
```

3. **Búsqueda de comandos**: Para buscar ejecutables en los directorios listados en la variable `PATH`:

```c
char **get_path_directories(void)
{
    char *path_env = getenv("PATH");
    if (!path_env)
        return (NULL);
    
    // Duplicar para no modificar el original
    path_env = strdup(path_env);
    if (!path_env)
        return (NULL);
    
    // Contar cuántos directorios hay
    int count = 1;
    for (int i = 0; path_env[i]; i++)
        if (path_env[i] == ':')
            count++;
    
    // Crear array de strings
    char **dirs = malloc(sizeof(char*) * (count + 1));
    if (!dirs)
    {
        free(path_env);
        return (NULL);
    }
    
    // Dividir PATH en directorios
    int i = 0;
    char *token = strtok(path_env, ":");
    while (token)
    {
        dirs[i++] = strdup(token);
        token = strtok(NULL, ":");
    }
    dirs[i] = NULL;
    
    free(path_env);
    return (dirs);
}
```

4. **Implementación de comandos internos**: Como `env` que muestra variables de entorno:

```c
int builtin_env(char **args)
{
    extern char **environ;
    int i;
    
    (void)args;  // Evitar advertencia de no uso
    
    for (i = 0; environ[i]; i++)
        printf("%s\n", environ[i]);
    
    return (0);
}
```

`getenv()` es una de las funciones más utilizadas en la implementación de un shell, ya que el entorno es fundamental para la operación del sistema y la ejecución de comandos.

---
