
## 🧩 `opendir()`  
<small><code>#include &lt;dirent.h&gt;</code></small>

---

### 🧾 Header
```c
#include <dirent.h>
```

---

### 🧪 Prototipo
```c
DIR *opendir(const char *name);
```

---

### 📚 Descripción
La función `opendir()` abre un directorio y devuelve un puntero a una estructura `DIR` que puede ser utilizada posteriormente por funciones como `readdir()` o `closedir()` para leer su contenido.

---

### 🧰 Sintaxis y ejemplo
```c
DIR *dir = opendir("/home/user");
if (dir != NULL) {
    // Leer con readdir()
}
```

<summary>🔍 Ejemplo de uso en contexto</summary>

```c
#include <dirent.h>
#include <stdio.h>

int main(void) {
    DIR *dir = opendir(".");
    if (dir == NULL) {
        perror("opendir");
        return 1;
    }
    closedir(dir);
    return 0;
}
```


---

### 💡 Consejos de uso
- Siempre verificar si `opendir()` devuelve `NULL` antes de operar.
- Cierra el directorio con `closedir()` cuando termines.

---

### 🔙 Retorno
Devuelve un puntero a `DIR` si tiene éxito. Si hay error, devuelve `NULL` y establece `errno`.

---

### ⚠️ Errores
- `ENOENT`: el directorio no existe.
- `EACCES`: no tienes permiso para acceder al directorio.

---

### 🧭 Información adicional
<details>
<summary>📎 ¿Por qué se usa en Minishell?</summary>

Se puede usar para listar archivos de un directorio (como en un posible comando `ls`) o para verificar que una ruta dada realmente corresponde a un directorio.

</details>

---
