## 🧩 `lstat()`  
<small><code>#include &lt;sys/stat.h&gt;</code></small>

---

### 🧾 Header
```c
#include <sys/stat.h>
#include <unistd.h>
```

---

### 🧪 Prototipo
```c
int lstat(const char *pathname, struct stat *statbuf);
```

---

### 📚 Descripción
La función `lstat()` **obtiene información** sobre un archivo referido por el pathname dado, **sin seguir enlaces simbólicos**. Es similar a `stat()`, pero cuando el archivo es un enlace simbólico, `lstat()` devuelve información sobre el enlace mismo, mientras que `stat()` devuelve información sobre el archivo al que apunta el enlace.

Esta característica hace que `lstat()` sea especialmente útil para herramientas que necesitan distinguir entre enlaces simbólicos y los archivos a los que apuntan.

---

### 🧰 Sintaxis y ejemplo
```c
struct stat file_stat;

if (lstat("/path/to/file", &file_stat) == 0)
{
    printf("Modo: %o\n", file_stat.st_mode);
    printf("Tamaño: %ld bytes\n", file_stat.st_size);
}
else
    perror("lstat error");
```


<summary>🔍 Ejemplo de uso en contexto (Detección de enlaces simbólicos)</summary>

```c
void print_file_info(const char *path)
{
    struct stat file_stat;
    
    if (lstat(path, &file_stat) == -1)
    {
        perror("lstat");
        return;
    }
    
    printf("Archivo: %s\n", path);
    
    // Comprobar si es un enlace simbólico
    if (S_ISLNK(file_stat.st_mode))
    {
        char link_target[PATH_MAX];
        ssize_t len = readlink(path, link_target, sizeof(link_target) - 1);
        
        if (len != -1)
        {
            link_target[len] = '\0';
            printf("Es un enlace simbólico -> %s\n", link_target);
            
            // Obtener información del archivo al que apunta
            struct stat target_stat;
            if (stat(path, &target_stat) == 0)
                printf("Tamaño del archivo objetivo: %ld bytes\n", target_stat.st_size);
        }
    }
    else
    {
        printf("Tipo: %s\n", S_ISDIR(file_stat.st_mode) ? "Directorio" : "Archivo regular");
        printf("Tamaño: %ld bytes\n", file_stat.st_size);
    }
}
```



---

### 💡 Consejos de uso
- **Comprobación de tipos de archivo**: Usar las macros definidas en `<sys/stat.h>` para determinar el tipo de archivo:
  - `S_ISREG(m)`: Archivo regular
  - `S_ISDIR(m)`: Directorio
  - `S_ISCHR(m)`: Dispositivo de caracteres
  - `S_ISBLK(m)`: Dispositivo de bloques
  - `S_ISFIFO(m)`: FIFO (named pipe)
  - `S_ISLNK(m)`: Enlace simbólico
  - `S_ISSOCK(m)`: Socket
- **Permisos de archivo**: El campo `st_mode` también contiene información sobre los permisos, que se pueden verificar con operaciones de bit.
- **Diferencias con `stat()`**: Usar `lstat()` cuando se necesite información específica sobre enlaces simbólicos, y `stat()` cuando se necesite información sobre el archivo al que apunta un enlace.
- **Tiempo de acceso**: La estructura `stat` incluye varios campos de tiempo como `st_atime` (último acceso), `st_mtime` (última modificación) y `st_ctime` (último cambio de estado).

---

### 🔙 Retorno
Devuelve `0` en caso de éxito.  
En caso de error, devuelve `-1` y establece la variable `errno`.

---

### ⚠️ Errores
- `EACCES`: Permiso denegado para buscar en algún componente del nombre de la ruta.
- `EBADF`: El descriptor de archivo es inválido.
- `EFAULT`: Puntero a buffer en una dirección inválida.
- `ELOOP`: Demasiados enlaces simbólicos al resolver la ruta.
- `ENAMETOOLONG`: El nombre de la ruta es demasiado largo.
- `ENOENT`: Un componente de la ruta no existe o es un enlace simbólico roto.
- `ENOMEM`: Memoria insuficiente para completar la operación.
- `ENOTDIR`: Un componente de la ruta que debería ser un directorio no lo es.

---

### 🧭 Información adicional

<summary>📎 ¿Por qué se usa en Minishell?</summary>

En Minishell, `lstat()` puede ser útil para varias funcionalidades:

1. **Implementación del comando `ls`**: Para mostrar información detallada sobre archivos, incluyendo si son enlaces simbólicos:

```c
void simple_ls(char *path)
{
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;
    char full_path[PATH_MAX];
    
    if ((dir = opendir(path)) == NULL)
    {
        perror("opendir");
        return;
    }
    
    while ((entry = readdir(dir)) != NULL)
    {
        // Ignorar entradas ocultas
        if (entry->d_name[0] == '.' && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
            continue;
        
        // Construir ruta completa
        snprintf(full_path, PATH_MAX, "%s/%s", path, entry->d_name);
        
        // Obtener información del archivo
        if (lstat(full_path, &file_stat) == -1)
            continue;
        
        // Mostrar tipo de archivo (para enlaces simbólicos, usar "@")
        if (S_ISLNK(file_stat.st_mode))
            printf("@ ");
        else if (S_ISDIR(file_stat.st_mode))
            printf("/ ");
        else if (file_stat.st_mode & S_IXUSR)
            printf("* ");
        else
            printf("  ");
        
        printf("%s\n", entry->d_name);
    }
    
    closedir(dir);
}
```

2. **Verificación de ejecutables**: Antes de intentar ejecutar un comando, el shell debe verificar si el archivo existe y tiene permisos de ejecución:

```c
int is_executable(const char *path)
{
    struct stat file_stat;
    
    // Verificar si el archivo existe
    if (lstat(path, &file_stat) == -1)
        return 0;
    
    // Si es un enlace simbólico, verificar el archivo real
    if (S_ISLNK(file_stat.st_mode))
    {
        if (stat(path, &file_stat) == -1)
            return 0;
    }
    
    // Verificar si es un archivo regular y tiene permiso de ejecución
    return S_ISREG(file_stat.st_mode) && (file_stat.st_mode & S_IXUSR);
}
```

3. **Implementación de operadores de redireccionamiento**: Al redirigir entrada/salida a archivos, es útil verificar primero si existen y son del tipo correcto:

```c
int validate_redirection(const char *file, int is_input)
{
    struct stat file_stat;
    
    if (lstat(file, &file_stat) == -1)
    {
        if (is_input || errno != ENOENT)
        {
            perror(file);
            return 0;
        }
        return 1;  // Para salida, no importa si no existe
    }
    
    // Para entrada, verificar que sea un archivo regular o FIFO
    if (is_input && !S_ISREG(file_stat.st_mode) && !S_ISFIFO(file_stat.st_mode))
    {
        fprintf(stderr, "%s: No es un archivo regular o FIFO\n", file);
        return 0;
    }
    
    return 1;
}
```



---