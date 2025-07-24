## 🧩 `closedir()`  
<small><code>#include &lt;dirent.h&gt;</code></small>

---

### 🧾 Header
```c
#include <dirent.h>
```

---

### 🧪 Prototipo
```c
int closedir(DIR *dirp);
```

---

### 📚 Descripción
Cierra el flujo de directorio asociado con el puntero `dirp` y libera los recursos asignados por `opendir()`. Después de llamar a `closedir()`, el puntero `dirp` ya no es válido y no debe ser utilizado.

---

### 🧰 Sintaxis y ejemplo
```c
DIR *dir = opendir("/home/user");
if (dir)
{
    // Operaciones con el directorio...
    closedir(dir);  // Cerrar el directorio cuando ya no se necesita
}
```

<summary>🔍 Ejemplo de uso en contexto (Procesamiento seguro de directorios)</summary>

```c
int count_files(const char *path)
{
    DIR *dir;
    struct dirent *entry;
    int count = 0;
    
    dir = opendir(path);
    if (!dir)
    {
        perror("opendir");
        return -1;
    }
    
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] != '.')  // Ignorar archivos ocultos
            count++;
    }
    
    closedir(dir);  // Liberar recursos
    return count;
}
```

---

### 💡 Consejos de uso
- **Siempre cierra** los directorios cuando ya no los necesites para evitar fugas de recursos.
- Es importante llamar a `closedir()` en todas las rutas de salida, incluyendo manejo de errores.
- Si olvidas cerrar un directorio, tu programa puede quedarse sin descriptores de archivo disponibles.

---

### 🔙 Retorno
Devuelve `0` si la operación fue exitosa.  
En caso de error, devuelve `-1` y establece `errno` apropiadamente.

---

### ⚠️ Errores
- `EBADF`: El argumento `dirp` no se refiere a un directorio abierto válido.
- Otros errores pueden ocurrir según la implementación del sistema.

---

### 🧭 Información adicional

<summary>📎 ¿Por qué se usa en Minishell?</summary>

En Minishell, `closedir()` es crucial para mantener un uso eficiente de los recursos del sistema. Cada vez que se abre un directorio (por ejemplo, para buscar comandos en `PATH`, implementar `ls`, o manejar comodines), debe cerrarse cuando ya no sea necesario.

Un patrón común en Minishell sería:

```c
int search_executable_in_path(const char *cmd)
{
    char **paths = get_paths_from_env();
    DIR *dir;
    struct dirent *entry;
    int i = 0;
    
    while (paths && paths[i])
    {
        dir = opendir(paths[i]);
        if (dir)
        {
            while ((entry = readdir(dir)) != NULL)
            {
                if (strcmp(entry->d_name, cmd) == 0)
                {
                    closedir(dir);  // ¡No olvidar cerrar antes de retornar!
                    return i;  // Encontrado en este directorio
                }
            }
            closedir(dir);  // Cerrar antes de pasar al siguiente directorio
        }
        i++;
    }
    return -1;  // No encontrado
}
```

No cerrar directorios correctamente puede llevar a fugas de recursos en el shell, especialmente en operaciones repetitivas como búsqueda de comandos o expansión de comodines.

---