## 🧩 `wait()`  
<small><code>#include &lt;sys/wait.h&gt;</code></small>

---

### 🧾 Header
```c
#include <sys/wait.h>
```

---

### 🧪 Prototipo
```c
pid_t wait(int *status);
```

---

### 📚 Descripción
La función `wait()` **suspende la ejecución** del proceso llamante hasta que uno de sus procesos hijos termine. Si un proceso hijo ya ha terminado en el momento de la llamada (un "zombie"), la función retorna inmediatamente. Permite al proceso padre **recoger información sobre la terminación** del proceso hijo y liberar los recursos del sistema que aún estén asignados al proceso hijo.

---

### 🧰 Sintaxis y ejemplo
```c
int status;
pid_t child_pid;

child_pid = wait(&status);  // Espera a que cualquier hijo termine
```


<summary>🔍 Ejemplo de uso en contexto (Fork y Wait)</summary>

```c
pid_t pid = fork();

if (pid == -1)
{
    perror("fork error");
    exit(EXIT_FAILURE);
}
else if (pid == 0)
{
    // Código del proceso hijo
    printf("Proceso hijo ejecutando (PID: %d)\n", getpid());
    exit(EXIT_SUCCESS);
}
else
{
    // Código del proceso padre
    int status;
    pid_t terminated_pid = wait(&status);
    
    printf("Proceso hijo %d ha terminado\n", terminated_pid);
    if (WIFEXITED(status))
        printf("Código de salida: %d\n", WEXITSTATUS(status));
}
```



---

### 💡 Consejos de uso
- Utilizar las **macros** definidas en `<sys/wait.h>` para examinar el valor del parámetro `status`:
  - **`WIFEXITED(status)`**: Verdadero si el hijo terminó normalmente.
  - **`WEXITSTATUS(status)`**: Devuelve el código de salida del hijo (si terminó normalmente).
  - **`WIFSIGNALED(status)`**: Verdadero si el hijo terminó debido a una señal.
  - **`WTERMSIG(status)`**: Devuelve el número de la señal que causó la terminación.
- Si hay **múltiples hijos** ejecutándose simultáneamente, `wait()` esperará a cualquiera de ellos.
- Puedes pasar `NULL` como argumento para `status` si no necesitas la información de terminación.

---

### 🔙 Retorno
Devuelve el **PID del proceso hijo terminado** en caso de éxito.  
En caso de error, devuelve `-1` y establece la variable `errno`.

---

### ⚠️ Errores
- `ECHILD`: El proceso llamante no tiene procesos hijos.
- `EINTR`: La llamada fue interrumpida por una señal.
- Si el proceso no tiene hijos, `wait()` retornará inmediatamente con un error.

---

### 🧭 Información adicional

<summary>📎 ¿Por qué se usa en Minishell?</summary>

En Minishell, `wait()` es crucial para implementar correctamente los comandos en segundo plano y para manejar la ejecución de procesos externos mediante `exec`. Cuando se ejecuta un comando externo, como por ejemplo:

```
ls -la
```

El shell suele crear un proceso hijo para ejecutar ese comando y luego espera a que termine antes de mostrar un nuevo prompt. El código sería algo así:

```c
pid_t pid = fork();
if (pid == 0)
{
    // Proceso hijo
    execve("/bin/ls", args, env);
    exit(EXIT_FAILURE);  // Solo se ejecuta si execve falla
}
else
{
    // Proceso padre (shell)
    int status;
    wait(&status);  // Espera a que termine el hijo
    
    // Procesa el estado de salida y muestra un nuevo prompt
}
```

Esto garantiza que los comandos se ejecuten secuencialmente y que el shell pueda recoger y procesar el código de salida del último comando ejecutado (almacenado comúnmente en la variable `$?`).



---