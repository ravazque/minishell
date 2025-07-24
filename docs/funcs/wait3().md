## 🧩 `wait3()`  
<small><code>#include &lt;sys/wait.h&gt;</code></small>

---

### 🧾 Header
```c
#include <sys/wait.h>
#include <sys/resource.h>  // Para la estructura rusage
#include <sys/time.h>      // También requerido en algunos sistemas
```

---

### 🧪 Prototipo
```c
pid_t wait3(int *status, int options, struct rusage *rusage);
```

---

### 📚 Descripción
La función `wait3()` **suspende la ejecución** del proceso llamante hasta que uno de sus procesos hijos termine, similar a `wait()`, pero con dos capacidades adicionales:
1. Permite especificar **opciones** para modificar el comportamiento de espera.
2. Proporciona **información de uso de recursos** sobre el proceso hijo terminado.

Es una extensión de BSD que combina las funcionalidades de `waitpid()` y la capacidad de recopilar información sobre el uso de recursos.

---

### 🧰 Sintaxis y ejemplo
```c
int status;
struct rusage usage;
pid_t child_pid;

child_pid = wait3(&status, 0, &usage);  // Espera bloqueante
// O bien
child_pid = wait3(&status, WNOHANG, &usage);  // No bloqueante
```


<summary>🔍 Ejemplo de uso en contexto (Con información de recursos)</summary>

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
    for (int i = 0; i < 1000000; i++)
        ; // Simulación de trabajo
    exit(EXIT_SUCCESS);
}
else
{
    // Código del proceso padre
    int status;
    struct rusage usage;
    
    pid_t terminated_pid = wait3(&status, 0, &usage);
    
    printf("Proceso hijo %d terminado\n", terminated_pid);
    printf("Tiempo de usuario: %ld.%06ld segundos\n", 
           usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
    printf("Tiempo de sistema: %ld.%06ld segundos\n", 
           usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);
}
```



---

### 💡 Consejos de uso
- La estructura `rusage` proporciona información sobre el tiempo de CPU, uso de memoria y otros recursos del proceso hijo.
- Opciones comunes para el parámetro `options`:
  - **`WNOHANG`**: No bloquea si no hay hijos terminados disponibles (retorna 0 inmediatamente).
  - **`WUNTRACED`**: También retorna si un hijo ha sido detenido (pero no terminado).
  - **`WCONTINUED`**: También retorna si un hijo detenido ha sido continuado.
- Se pueden combinar opciones usando el operador OR (`|`).
- Si no necesitas información de recursos, puedes pasar `NULL` para el parámetro `rusage`.

---

### 🔙 Retorno
Devuelve el **PID del proceso hijo terminado** en caso de éxito.  
Devuelve `0` si se usó `WNOHANG` y no hay hijos terminados disponibles.  
En caso de error, devuelve `-1` y establece la variable `errno`.

---

### ⚠️ Errores
- `ECHILD`: El proceso llamante no tiene procesos hijos.
- `EINTR`: La llamada fue interrumpida por una señal.
- `EINVAL`: Se proporcionó una opción inválida en el parámetro `options`.

---

### 🧭 Información adicional

<summary>📎 ¿Por qué se usa en Minishell?</summary>

En Minishell, `wait3()` puede utilizarse cuando se necesita información detallada sobre los recursos utilizados por los comandos ejecutados. Por ejemplo, podría utilizarse para implementar un comando de tiempo incorporado similar a `/usr/bin/time` que muestra estadísticas de uso.

```c
void execute_with_timing(char **args)
{
    pid_t pid = fork();
	
    if (pid == 0)
    {
        // Proceso hijo
        execve(args[0], args, environ);
        perror("execve");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        // Proceso padre
        int status;
        struct rusage usage;
		
        wait3(&status, 0, &usage);
		
        // Informar sobre el tiempo utilizado
        fprintf(stderr, "real: %ld.%06ld user: %ld.%06ld sys: %ld.%06ld\n",
               usage.ru_utime.tv_sec + usage.ru_stime.tv_sec,
               usage.ru_utime.tv_usec + usage.ru_stime.tv_usec,
               usage.ru_utime.tv_sec, usage.ru_utime.tv_usec,
               usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);
    }
}
```

Esta función podría ser útil para implementar un comando `time` interno o para depurar el rendimiento de los comandos ejecutados en el shell.

Otra aplicación en Minishell sería monitorizar y limitar el uso de recursos de comandos ejecutados, especialmente en un entorno donde se necesita control sobre la ejecución de procesos.

---

### 📝 Nota importante
La función `wait3()` se considera obsoleta en algunos sistemas modernos, y se recomienda usar `wait4()` en su lugar, que proporciona las mismas capacidades con mayor flexibilidad.