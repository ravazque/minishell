
## 🧩 `stat()`  
<small><code>#include &lt;sys/stat.h&gt;<br>#include &lt;unistd.h&gt;</code></small>

---

### 🧾 Header
```c
#include <sys/stat.h>
#include <unistd.h>
```

---

### 🧪 Prototipo
```c
int stat(const char *pathname, struct stat *statbuf);
```

---

### 📚 Descripción
La función `stat()` obtiene información del archivo o directorio especificado y la guarda en una estructura `stat`. Esta información incluye tipo de archivo, tamaño, permisos, número de inodos, entre otros.

---

### 🧰 Sintaxis y ejemplo
```c
struct stat info;
if (stat("archivo.txt", &info) == 0) {
    // El archivo existe y se puede acceder a su info
}
```


<summary>🔍 Ejemplo de uso en contexto</summary>

```c
#include <sys/stat.h>
#include <stdio.h>

int main(void) {
    struct stat s;
    if (stat("archivo.txt", &s) == 0)
        printf("Tamaño: %ld bytes\n", s.st_size);
    else
        perror("stat");
    return 0;
}
```


---

### 💡 Consejos de uso
- Útil para verificar si un archivo existe antes de operar sobre él.
- Puede ayudar a distinguir entre archivos normales, directorios y enlaces.

---

### 🔙 Retorno
Devuelve `0` si tuvo éxito. Devuelve `-1` y establece `errno` si falla.

---

### ⚠️ Errores
- Falla si el archivo no existe o si no se tienen permisos para acceder a él.

---

### 🧭 Información adicional
<details>
<summary>📎 ¿Por qué se usa en Minishell?</summary>

Minishell usa `stat()` para verificar si un comando es un archivo ejecutable válido, o si una ruta ingresada corresponde a un directorio antes de usarla en `cd`, por ejemplo.

</details>

---
