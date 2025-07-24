## 🧩 `unlink()`  
<small><code>#include &lt;unistd.h&gt;</code></small>

---

### 🧾 Header
```c
#include <unistd.h>
```

---

### 🧪 Prototipo
```c
int unlink(const char *pathname);
```

---

### 📚 Descripción
La función `unlink()` **elimina un nombre del sistema de archivos**. Si el nombre eliminado era el último enlace al archivo y ningún proceso tiene el archivo abierto, el archivo se elimina y el espacio que ocupaba queda disponible.

Si el nombre era el último enlace al archivo, pero algún proceso aún lo tiene abierto, el archivo seguirá existiendo hasta que se cierre el último descriptor de archivo que lo referencia.

Esta función es la forma estándar de eliminar archivos en sistemas UNIX/Linux.

---

### 🧰 Sintaxis y ejemplo
```c
if (unlink("/tmp/tempfile.txt") == 0)
    printf("Archivo eliminado correctamente\n");
else
    perror("Error al eliminar el archivo");
```


<summary>🔍 Ejemplo de uso en contexto (Archivos temporales)</summary>

```c
int create_temp_file(void)
{
    char temp_name[] = "/tmp/minishell_temp_XXXXXX";
    int fd;
    
    // Crear un archivo temporal único
    fd = mkstemp(temp_name);
    if (fd == -1)
    {
        perror("mkstemp");
        return -1;
    }
    
    // Eliminar el nombre del archivo pero mantener el descriptor abierto
    // Esto asegura que cuando se cierre el descriptor, el archivo se elimine automáticamente
    if (unlink(temp_name) == -1)
    {
        perror("unlink");
        close(fd);
        return -1;
    }
    
    printf("Archivo temporal creado y configurado para autoeliminar\n");
    return fd;
}
```



---

### 💡 Consejos de uso
- **Eliminar archivos temporales**: Ideal para archivos que solo se necesitan durante la ejecución del programa.
- **Permisos necesarios**: Se requieren permisos de escritura en el directorio que contiene el archivo.
- **No elimina directorios**: Para eliminar directorios, use `rmdir()` (si están vacíos) o recursivamente elimine su contenido primero.
- **Enlaces simbólicos**: Si el archivo es un enlace simbólico, se elimina el enlace, no el archivo al que apunta.
- **Patrón común**: Con `unlink()` justo después de `open()`, se crea un archivo "anónimo" que desaparece cuando el programa termina, incluso si termina abruptamente.

---

### 🔙 Retorno
Devuelve `0` en caso de éxito.  
En caso de error, devuelve `-1` y establece la variable `errno`.

---

### ⚠️ Errores
- `EACCES`: Permiso denegado para escribir en el directorio.
- `EBUSY`: El archivo es un directorio en uso como punto de montaje o está siendo utilizado por el sistema.
- `EFAULT`: `pathname` apunta fuera del espacio de direcciones accesible.
- `EIO`: Error de entrada/salida.
- `EISDIR`: `pathname` es un directorio (en algunos sistemas, usar `rmdir()` en su lugar).
- `ELOOP`: Se encontraron demasiados enlaces simbólicos en la resolución de la ruta.
- `ENAMETOOLONG`: `pathname` es demasiado largo.
- `ENOENT`: El archivo no existe o el pathname es una cadena vacía.
- `ENOMEM`: Memoria insuficiente para completar la operación.
- `ENOTDIR`: Un componente de la ruta no es un directorio.
- `EPERM`: La operación no está permitida (por ejemplo, bit de inmutabilidad activado).
- `EROFS`: El sistema de archivos es de solo lectura.

---

### 🧭 Información adicional

<summary>📎 ¿Por qué se usa en Minishell?</summary>

En Minishell, `unlink()` puede ser útil para varias operaciones:

1. **Manejo de archivos temporales**: Cuando se necesita almacenar datos temporales para redireccionamientos complejos o para el procesamiento de comandos:

```c
int execute_command_with_temp(char **args, char *temp_input)
{
    // Crear un archivo temporal para el input
    char temp_path[] = "/tmp/minishell_input_XXXXXX";
    int temp_fd = mkstemp(temp_path);
    
    if (temp_fd == -1)
    {
        perror("mkstemp");
        return EXIT_FAILURE;
    }
    
    // Escribir el input temporal
    write(temp_fd, temp_input, strlen(temp_input));
    close(temp_fd);
    
    // Abrir para lectura
    temp_fd = open(temp_path, O_RDONLY);
    if (temp_fd == -1)
    {
        perror("open temp file");
        unlink(temp_path);  // Intentar eliminar el archivo
        return EXIT_FAILURE;
    }
    
    // Duplicar stdin para restaurarlo más tarde
    int stdin_backup = dup(STDIN_FILENO);
    
    // Redirigir stdin al archivo temporal
    dup2(temp_fd, STDIN_FILENO);
    close(temp_fd);
    
    // Ejecutar el comando
    int result = execute_command(args);
    
    // Restaurar stdin
    dup2(stdin_backup, STDIN_FILENO);
    close(stdin_backup);
    
    // Eliminar el archivo temporal
    unlink(temp_path);
    
    return result;
}
```

2. **Implementación del comando `rm`**: Si tu shell implementa un comando `rm` integrado:

```c
int builtin_rm(char **args)
{
    int i = 1;
    int ret = EXIT_SUCCESS;
    
    if (!args[1])
    {
        fprintf(stderr, "rm: operando faltante\n");
        return EXIT_FAILURE;
    }
    
    while (args[i])
    {
        if (unlink(args[i]) == -1)
        {
            perror(args[i]);
            ret = EXIT_FAILURE;
        }
        i++;
    }
    
    return ret;
}
```

3. **Limpieza de archivos de configuración temporales**: Al iniciar o finalizar el shell, puede ser necesario eliminar archivos temporales de sesiones anteriores:

```c
void cleanup_temp_files(void)
{
    // Eliminar el archivo de historial temporal si existe
    char history_path[PATH_MAX];
    snprintf(history_path, PATH_MAX, "/tmp/minishell_history_%d", getpid());
    
    if (access(history_path, F_OK) == 0)
        unlink(history_path);
    
    // Eliminar otros archivos temporales
    DIR *dir = opendir("/tmp");
    struct dirent *entry;
    
    if (dir)
    {
        while ((entry = readdir(dir)) != NULL)
        {
            // Buscar archivos que coincidan con nuestro patrón
            if (strncmp(entry->d_name, "minishell_", 10) == 0)
            {
                char full_path[PATH_MAX];
                snprintf(full_path, PATH_MAX, "/tmp/%s", entry->d_name);
                unlink(full_path);
            }
        }
        closedir(dir);
    }
}
```

La capacidad de `unlink()` para eliminar archivos de forma segura es crucial en un shell que puede necesitar crear y eliminar muchos archivos temporales durante su ejecución.