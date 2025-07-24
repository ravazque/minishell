## 🧩 `getcwd()`  
<small><code>#include &lt;unistd.h&gt;</code></small>

---

### 🧾 Header
```c
#include <unistd.h>
```

---

### 🧪 Prototipo
```c
char *getcwd(char *buf, size_t size);
```

---

### 📚 Descripción
La función `getcwd()` **obtiene el directorio de trabajo actual** (current working directory) del proceso. Copia la ruta absoluta del directorio actual en el buffer proporcionado por el usuario, asegurándose de que no exceda el tamaño especificado.

El nombre `getcwd` proviene de "**get current working directory**" y es una función fundamental para la navegación de directorios en aplicaciones que interactúan con el sistema de archivos.

---

### 🧰 Sintaxis y ejemplo
```c
char buffer[PATH_MAX];

if (getcwd(buffer, sizeof(buffer)) != NULL)
    printf("Directorio actual: %s\n", buffer);
else
    perror("getcwd() error");
```


<summary>🔍 Ejemplo de uso en contexto (Implementación del comando PWD)</summary>

```c
int cmd_pwd(void)
{
    char cwd[PATH_MAX];
    
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("getcwd");
        return EXIT_FAILURE;
    }
    
    printf("%s\n", cwd);
    return EXIT_SUCCESS;
}
```



---

### 💡 Consejos de uso
- **Tamaño del buffer**: Es recomendable usar `PATH_MAX` como tamaño del buffer (definido en `<limits.h>`), que representa la longitud máxima de una ruta en el sistema.
- **Asignación dinámica**: Si se pasa `NULL` como buffer y un tamaño mayor que cero, `getcwd()` asignará dinámicamente memoria para almacenar el directorio actual. Esta memoria debe ser liberada después con `free()`.
- **Verificar el retorno**: Siempre verificar que la función no haya retornado `NULL` antes de utilizar el buffer.
- **No confiar en `./` y `../`**: El resultado de `getcwd()` siempre es una ruta absoluta, sin referencias relativas.

---

### 🔙 Retorno
Devuelve un **puntero al buffer** con la ruta en caso de éxito.  
En caso de error, devuelve `NULL` y establece la variable `errno`.

---

### ⚠️ Errores
- `EACCES`: Permiso denegado para leer o buscar en un componente del nombre de la ruta.
- `EINVAL`: El tamaño proporcionado es cero y el buffer no es NULL.
- `ENOENT`: El directorio actual ya no existe (por ejemplo, fue eliminado por otro proceso).
- `ERANGE`: El tamaño del buffer es demasiado pequeño para almacenar la ruta completa.
- `ENOMEM`: Memoria insuficiente disponible (cuando se solicita asignación dinámica).

---

### 🧭 Información adicional

<summary>📎 ¿Por qué se usa en Minishell?</summary>

En Minishell, `getcwd()` es esencial para varias funcionalidades:

1. **Implementación del comando `pwd`**: El comando `pwd` (print working directory) simplemente muestra el directorio actual, lo que se logra directamente con una llamada a `getcwd()`.

2. **Actualización del prompt**: Muchos shells muestran el directorio actual en el prompt. `getcwd()` permite obtener esta información:

```c
void update_prompt(void)
{
    char cwd[PATH_MAX];
    char *prompt;
    
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        strcpy(cwd, "(error)");
    
    // Obtener solo el nombre del directorio base
    char *basename = strrchr(cwd, '/');
    basename = (basename && *(basename + 1)) ? basename + 1 : cwd;
    
    // Crear prompt con el formato: usuario@host:directorio$
    prompt = ft_strjoin3(getenv("USER"), ":", basename);
    prompt = ft_strjoin(prompt, "$ ");
    
    // Usar el prompt en readline
    rl_set_prompt(prompt);
    free(prompt);
}
```

3. **Implementación de rutas relativas**: Cuando el usuario ingresa comandos con rutas relativas, `getcwd()` permite convertirlas a rutas absolutas:

```c
char *convert_to_absolute_path(const char *relative_path)
{
    char cwd[PATH_MAX];
    char *absolute_path;
    
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return NULL;
    
    // Si la ruta no comienza con '/', es relativa
    if (relative_path[0] != '/')
    {
        absolute_path = ft_strjoin(cwd, "/");
        absolute_path = ft_strjoin(absolute_path, relative_path);
    }
    else
        absolute_path = ft_strdup(relative_path);
    
    return absolute_path;
}
```

4. **Manejo del comando `cd`**: Después de cambiar de directorio con `chdir()`, `getcwd()` se usa para obtener la ruta actualizada y actualizar variables de entorno como `PWD`:

```c
int cmd_cd(char *path)
{
    char cwd[PATH_MAX];
    char old_cwd[PATH_MAX];
    
    // Guardar directorio actual antes de cambiar
    if (getcwd(old_cwd, sizeof(old_cwd)) == NULL)
    {
        perror("getcwd");
        return EXIT_FAILURE;
    }
    
    // Cambiar de directorio
    if (chdir(path) != 0)
    {
        perror("cd");
        return EXIT_FAILURE;
    }
    
    // Obtener y actualizar el nuevo directorio actual
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        setenv("PWD", cwd, 1);
    
    return EXIT_SUCCESS;
}
```



---