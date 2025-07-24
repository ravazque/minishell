## 🧩 `chdir()`  
<small><code>#include &lt;unistd.h&gt;</code></small>

---

### 🧾 Header
```c
#include <unistd.h>
```

---

### 🧪 Prototipo
```c
int chdir(const char *path);
```

---

### 📚 Descripción
Cambia el **directorio de trabajo actual** del proceso en ejecución. Esta función modifica la ruta del directorio en el que se encuentra el proceso, afectando a las operaciones relativas de archivos y directorios.

Es esencial para implementar el comando `cd` (change directory) en un shell y para navegar por el sistema de archivos.

---

### 🧰 Sintaxis y ejemplo
```c
chdir("/home/user");  // Cambia al directorio /home/user
```

<summary>🔍 Ejemplo de uso en contexto (Comando cd)</summary>

```c
int	cmd_cd(char *path)
{
    char	*home;
    
    // Si no hay argumento, ir al directorio HOME
    if (!path)
    {
        home = getenv("HOME");
        if (!home)
        {
            fprintf(stderr, "cd: HOME not set\n");
            return (1);
        }
        path = home;
    }
    
    // Cambia al directorio especificado
    if (chdir(path) == -1)
    {
        perror("cd");
        return (1);
    }
    
    return (0);
}
```

---

### 💡 Consejos de uso
- **Solo cambia el directorio del proceso** actual. Los procesos hijos heredarán este directorio, pero los procesos padre o hermanos no se ven afectados.
- Para implementar el comando `cd` completamente, hay que considerar casos especiales:
  - `cd` sin argumentos: Ir al directorio HOME
  - `cd -`: Ir al directorio anterior (OLDPWD)
  - `cd ~`: Expandir al directorio HOME
- Después de cambiar el directorio, es común **actualizar las variables de entorno** como `PWD` y `OLDPWD` para mantener la consistencia.
- Para obtener el directorio actual después de `chdir()`, usa `getcwd()`.

---

### 🔙 Retorno
Devuelve `0` si la operación fue exitosa.  
En caso de error, devuelve `-1` y establece `errno`.

---

### ⚠️ Errores
- `EACCES`: Permiso denegado para acceder al directorio.
- `ENOENT`: El directorio no existe.
- `ENOTDIR`: El componente del path no es un directorio.
- `ENAMETOOLONG`: El path es demasiado largo.
- `ELOOP`: Demasiados enlaces simbólicos al resolver el path.

---

### 🧭 Información adicional

<summary>📎 ¿Por qué se usa en Minishell?</summary>

En Minishell, `chdir()` es fundamental para implementar el comando builtin `cd`, que permite al usuario navegar por el sistema de archivos:

1. **Implementación del comando cd**: Es la función principal que realiza el cambio de directorio.

```c
int	builtin_cd(char **args)
{
    char	current_dir[PATH_MAX];
    char	*target_dir;
    
    // Guarda el directorio actual para actualizar OLDPWD después
    if (getcwd(current_dir, PATH_MAX) == NULL)
    {
        perror("minishell: cd");
        return (1);
    }
    
    // Determina el directorio objetivo
    if (!args[1] || strcmp(args[1], "~") == 0)
        target_dir = getenv("HOME");
    else if (strcmp(args[1], "-") == 0)
        target_dir = getenv("OLDPWD");
    else
        target_dir = args[1];
    
    // Cambia al directorio objetivo
    if (chdir(target_dir) == -1)
    {
        perror("minishell: cd");
        return (1);
    }
    
    // Actualiza las variables de entorno
    setenv("OLDPWD", current_dir, 1);
    
    if (getcwd(current_dir, PATH_MAX) != NULL)
        setenv("PWD", current_dir, 1);
    
    return (0);
}
```

2. **Manejo de casos especiales**: Como `cd -` para volver al directorio anterior, o `cd` sin argumentos para ir al HOME.

3. **Actualización del entorno**: Después de un `chdir()` exitoso, el shell debe actualizar las variables de entorno `PWD` y `OLDPWD`.

Esta implementación proporciona una experiencia de navegación similar a la de shells estándar como bash.

---