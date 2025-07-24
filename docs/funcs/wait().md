## 🧩 `wait4()`  
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
pid_t wait4(pid_t pid, int *status, int options, struct rusage *rusage);
```

---

### 📚 Descripción
La función `wait4()` **suspende la ejecución** del proceso llamante hasta que un proceso hijo específico cambie de estado. Es la función de espera más versátil del sistema UNIX, combinando las características de `waitpid()` y `wait3()`.

Permite:
1. Esperar a un **hijo específico** usando su PID.
2. Especificar **opciones** para modificar el comportamiento de espera.
3. Obtener **información detallada sobre los recursos** utilizados por el proceso hijo.

---

### 🧰 Sintaxis y ejemplo
```c
int status;
struct rusage usage;
pid_t child_pid = 1234;  // PID del proceso específico a esperar

// Espera a un proceso específico
pid_t ret = wait4(child_pid, &status, 0, &usage);

// Espera a cualquier proceso hijo (como wait3)
pid_t ret = wait4(-1, &status, 0, &usage);

// Espera no bloqueante
pid_t ret = wait4(child_pid, &status, WNOHANG, &usage);
```


<summary>🔍 Ejemplo de uso en contexto (Espera específica)</summary>

```c
pid_t pid1 = fork();
if (pid1 == 0)
{
    // Primer hijo
    sleep(2);
    exit(42);
}

pid_t pid2 = fork();
if (pid2 == 0)
{
    // Segundo hijo
    sleep(1);
    exit(84);
}

// Padre: espera específicamente al segundo hijo primero
int status;
struct rusage usage;

// Espera al hijo con PID pid2
wait4(pid2, &status, 0, &usage);
printf("Hijo 2 (PID %d) terminó con estado: %d\n", 
       pid2, WEXITSTATUS(status));
printf("Uso de CPU: %ld.%06ld segundos\n", 
       usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);

// Luego espera al primer hijo
wait4(pid1, &status, 0, &usage);
printf("Hijo 1 (PID %d) terminó con estado: %d\n", 
       pid1, WEXITSTATUS(status));
```



---

### 💡 Consejos de uso
- El parámetro `pid` determina qué procesos son esperados:
  - **`pid > 0`**: Espera al hijo con ese PID específico.
  - **`pid == -1`**: Espera a cualquier hijo (igual que `wait3()`).
  - **`pid == 0`**: Espera a cualquier hijo del mismo grupo de procesos.
  - **`pid < -1`**: Espera a cualquier hijo cuyo grupo de procesos sea igual al valor absoluto de `pid`.
- Las opciones comunes para el parámetro `options` incluyen:
  - **`WNOHANG`**: No bloquea si no hay hijos terminados.
  - **`WUNTRACED`**: También retorna si un hijo ha sido detenido.
  - **`WCONTINUED`**: También retorna si un hijo detenido ha sido continuado.
- Si no necesitas información de recursos, puedes pasar `NULL` para el parámetro `rusage`.
- Para examinar el parámetro `status`, usa las mismas macros que con `wait()` y `waitpid()`.

---

### 🔙 Retorno
Devuelve el **PID del proceso hijo** que cambió de estado en caso de éxito.  
Devuelve `0` si se usó `WNOHANG` y los hijos especificados existen pero ninguno ha cambiado de estado.  
En caso de error, devuelve `-1` y establece la variable `errno`.

---

### ⚠️ Errores
- `ECHILD`: No existe ningún hijo que coincida con `pid` o el proceso llamante no tiene procesos hijos.
- `EINTR`: La llamada fue interrumpida por una señal.
- `EINVAL`: Se proporcionó una opción inválida en el parámetro `options`.

---

### 🧭 Información adicional

<summary>📎 ¿Por qué se usa en Minishell?</summary>

En Minishell, `wait4()` puede ser muy útil para implementar características avanzadas de control de procesos, como:

1. **Manejo de múltiples procesos en pipeline**: Cuando se ejecuta un comando como `cmd1 | cmd2 | cmd3`, cada comando se ejecuta en un proceso separado. Con `wait4()`, el shell puede esperar específicamente a cada proceso en el orden deseado:

```c
void execute_pipeline(t_command *pipeline, int cmd_count)
{
    pid_t pids[MAX_PIPELINE];
    
    // Crear todos los procesos del pipeline
    for (int i = 0; i < cmd_count; i++)
        pids[i] = create_process_for_command(&pipeline[i]);
    
    // Esperar a que todos terminen y recoger su estado
    for (int i = 0; i < cmd_count; i++)
    {
        int status;
        struct rusage usage;
        
        wait4(pids[i], &status, 0, &usage);
        
        // Si es el último comando, guardar su estado de salida
        if (i == cmd_count - 1 && WIFEXITED(status))
            g_shell.last_exit_status = WEXITSTATUS(status);
            
        // Opcionalmente, registrar estadísticas de uso
        log_command_usage(pipeline[i].cmd, &usage);
    }
}
```

2. **Implementación de trabajos en segundo plano**: Cuando el usuario ejecuta un comando con `&` al final (como `sleep 10 &`), `wait4()` con la opción `WNOHANG` permite al shell verificar periódicamente si los procesos en segundo plano han terminado sin bloquearse:

```c
void check_background_jobs(void)
{
    for (int i = 0; i < g_shell.bg_job_count; i++)
    {
        int status;
        struct rusage usage;
        
        pid_t result = wait4(g_shell.bg_jobs[i].pid, &status, WNOHANG, &usage);
        
        if (result > 0)
        {
            // El proceso ha terminado
            printf("[%d]+ Done\t%s\n", i + 1, g_shell.bg_jobs[i].cmd);
            remove_job_from_list(i);
            i--;  // Ajustar el índice después de eliminar
        }
    }
}
```

Esta combinación de especificidad del PID y la capacidad de recopilar información de recursos hace que `wait4()` sea la función de espera más potente para implementar un shell completo.