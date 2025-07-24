## 🧩 `access()`  
<small><code>#include &lt;unistd.h&gt;</code></small>

---

### 🧾 Header
```c
#include <unistd.h>
```

---

### 🧪 Prototipo
```c
int access(const char *pathname, int mode);
```

---

### 📚 Descripción
Comprueba si el proceso que llama tiene **permisos de acceso al archivo** especificado por `pathname`. Permite verificar la existencia de un archivo y los derechos de acceso (lectura, escritura, ejecución) sin tener que abrir realmente el archivo.

---

### 🧰 Sintaxis y ejemplo
```c
if (access("/bin/ls", X_OK) == 0)
    printf("El archivo existe y es ejecutable\n");
```

<summary>🔍 Ejemplo de uso en contexto (Verificar ejecutable en Minishell)</summary>

```c
char *find_executable(char *cmd, char **paths)
{
    char *full_path;
    int i;
    
    i = 0;
    while (paths[i])
    {
        full_path = ft_strjoin(paths[i], "/");
        full_path = ft_strjoin_free(full_path, cmd);
        
        if (access(full_path, F_OK | X_OK) == 0)
            return (full_path);  // Archivo existe y es ejecutable
            
        free(full_path);
        i++;
    }
    return (NULL);  // Comando no encontrado
}
```

---

### 💡 Consejos de uso
- Los modos de acceso son:
  - `F_OK`: Comprueba la **existencia** del archivo.
  - `R_OK`: Comprueba el **permiso de lectura**.
  - `W_OK`: Comprueba el **permiso de escritura**.
  - `X_OK`: Comprueba el **permiso de ejecución**.
- Estos modos pueden combinarse con el operador OR (`|`).
- Hay una **condición de carrera** inherente: el archivo podría cambiar de estado entre la verificación con `access()` y una operación posterior en él.
- Comprueba los **permisos reales del usuario**, no los permisos efectivos (que podrían estar modificados por setuid o setgid).

---

### 🔙 Retorno
Devuelve `0` si el archivo existe y los permisos solicitados se conceden.  
En caso de error, devuelve `-1` y establece `errno` adecuadamente.

---

### ⚠️ Errores
- `EACCES`: El archivo existe pero los permisos solicitados no se conceden.
- `ENOENT`: El archivo no existe o el pathname es un puntero nulo.
- `EROFS`: Se solicitó permiso de escritura para un archivo en un sistema de archivos de solo lectura.
- `EFAULT`: `pathname` apunta fuera del espacio de direcciones accesible.
- `EINVAL`: `mode` es inválido.
- `EIO`: Error de entrada/salida.
- `ELOOP`: Se encontraron demasiados enlaces simbólicos al resolver `pathname`.
- `ENAMETOOLONG`: `pathname` es demasiado largo.
- `ENOTDIR`: Un componente de `pathname` no es un directorio.

---

### 🧭 Información adicional

<summary>📎 ¿Por qué se usa en Minishell?</summary>

En Minishell, `access()` se utiliza principalmente para:

1. **Buscar y verificar comandos ejecutables** en las rutas del PATH:
```c
char *get_command_path(char *cmd, char **env)
{
    char **paths;
    char *path_env;
    char *exec_path;
    
    // Si el comando incluye una ruta (como ./a.out o /bin/ls)
    if (cmd[0] == '/' || cmd[0] == '.')
    {
        if (access(cmd, F_OK | X_OK) == 0)
            return (ft_strdup(cmd));
        return (NULL);
    }
    
    // Buscar en las rutas del PATH
    path_env = get_env_value("PATH", env);
    paths = ft_split(path_env, ':');
    exec_path = find_executable(cmd, paths);
    
    ft_free_array(paths);
    return (exec_path);
}
```

2. **Verificar permisos de archivos** para las redirecciones:
```c
int check_redirections(t_command *cmd)
{
    if (cmd->input_file && access(cmd->input_file, R_OK) != 0)
    {
        print_error(cmd->input_file, strerror(errno));
        return (0);
    }
    
    if (cmd->output_file && cmd->append == 0)
    {
        // Para > (truncar), solo necesitamos verificar si podemos escribir en el directorio
        // ya que el archivo será creado o truncado
    }
    
    return (1);
}
```

Estas verificaciones son cruciales para proporcionar mensajes de error informativos y evitar intentar ejecutar comandos o acceder a archivos que no existen o para los que el usuario no tiene los permisos necesarios.

---