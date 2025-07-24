## 🧩 `readdir()`  
<small><code>#include &lt;dirent.h&gt;</code></small>

---

### 🧾 Header
```c
#include <dirent.h>
```

---

### 🧪 Prototipo
```c
struct dirent *readdir(DIR *dirp);
```

---

### 📚 Descripción
Lee la siguiente entrada del directorio especificado por el puntero `dirp`. Cada llamada a `readdir()` devuelve un puntero a una estructura `dirent` que representa la siguiente entrada en el flujo de directorio.

La estructura `dirent` contiene al menos los siguientes campos:
- `ino_t d_ino`: Número de inodo del archivo
- `char d_name[]`: Nombre del archivo (terminado en NULL)

---

### 🧰 Sintaxis y ejemplo
```c
DIR *dir = opendir("/home/user");
struct dirent *entry;

while ((entry = readdir(dir)) != NULL)
{
    printf("%s\n", entry->d_name);  // Imprime nombre de cada archivo
}
closedir(dir);
```

<summary>🔍 Ejemplo de uso en contexto (Listar archivos en un directorio)</summary>

```c
void list_directory(const char *path)
{
    DIR *dir;
    struct dirent *entry;
    
    if (!(dir = opendir(path)))
    {
        perror("opendir");
        return;
    }
    
    while ((entry = readdir(dir)) != NULL)
    {
        printf("%s\n", entry->d_name);
    }
    
    closedir(dir);
}
```

---

### 💡 Consejos de uso
- **No modifiques los datos** devueltos por `readdir()`, ya que apuntan a datos estáticos que serán sobrescritos en la siguiente llamada.
- La función devuelve `NULL` cuando llega al final del directorio o cuando ocurre un error. Para distinguir entre estos dos casos, se recomienda usar `errno`.
- Recuerda que `readdir()` también devuelve entradas para `.` (directorio actual) y `..` (directorio padre). Si no deseas procesar estos, debes filtrarlos explícitamente.

---

### 🔙 Retorno
Devuelve un puntero a una estructura `dirent` que representa la siguiente entrada de directorio.  
Devuelve `NULL` al final del directorio o si ocurre un error.

---

### ⚠️ Errores
- Si `dirp` no es un puntero de directorio válido, el comportamiento es indefinido.
- Si ocurre un error, `errno` puede establecerse a:
  - `EBADF`: El descriptor de archivo en `dirp` no es válido.
  - `EOVERFLOW`: Un valor es demasiado grande para ser almacenado en la estructura de datos.

---

### 🧭 Información adicional

<summary>📎 ¿Por qué se usa en Minishell?</summary>

En un shell como Minishell, la función `readdir()` es útil para:

1. Implementar comandos como `ls` que listan los contenidos de directorios.
2. Buscar archivos ejecutables en los directorios de `PATH` cuando se ejecuta un comando.
3. Implementar la expansión de comodines (`*`, `?`) en rutas de archivos.

Ejemplo de cómo podría usarse para implementar una versión simple de `ls`:

```c
int mini_ls(const char *path)
{
    DIR *dir;
    struct dirent *entry;
    
    if (!(dir = opendir(path ? path : ".")))
    {
        perror("ls");
        return (1);
    }
    
    while ((entry = readdir(dir)) != NULL)
    {
        // Omitir archivos ocultos
        if (entry->d_name[0] != '.')
            printf("%s  ", entry->d_name);
    }
    printf("\n");
    
    closedir(dir);
    return (0);
}
```

---