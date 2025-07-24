## 🧩 `pipe()`  
<small><code>#include &lt;unistd.h&gt;</code></small>

---

### 🧾 Header
```c
#include <unistd.h>
```

---

### 🧪 Prototipo
```c
int pipe(int pipefd[2]);
```

---

### 📚 Descripción
Crea un **canal de comunicación unidireccional** (tubería) entre procesos. La función crea dos descriptores de archivo:
- `pipefd[0]` es el extremo de **lectura** de la tubería.
- `pipefd[1]` es el extremo de **escritura** de la tubería.

Los datos escritos en el extremo de escritura serán disponibles para leer desde el extremo de lectura en orden FIFO (First In, First Out).

---

### 🧰 Sintaxis y ejemplo
```c
int pipefd[2];
if (pipe(pipefd) == -1)
    perror("pipe error");
else
{
    // pipefd[0] -> extremo de lectura
    // pipefd[1] -> extremo de escritura
}
```


<summary>🔍 Ejemplo de uso en contexto (Comunicación entre procesos)</summary>

```c
int pipefd[2];
pid_t pid;

if (pipe(pipefd) == -1)
{
    perror("pipe");
    exit(EXIT_FAILURE);
}

pid = fork();
if (pid == -1)
{
    perror("fork");
    exit(EXIT_FAILURE);
}

if (pid == 0)  // Proceso hijo
{
    // Cierra el extremo de lectura ya que no lo necesita
    close(pipefd[0]);
    
    // Redirige la salida estándar al extremo de escritura de la tubería
    dup2(pipefd[1], STDOUT_FILENO);
    close(pipefd[1]);
    
    // Ejecuta el primer comando
    execlp("ls", "ls", "-l", NULL);
    
    perror("execlp ls");
    exit(EXIT_FAILURE);
}
else  // Proceso padre
{
    // Cierra el extremo de escritura ya que no lo necesita
    close(pipefd[1]);
    
    // Redirige el extremo de lectura de la tubería a la entrada estándar
    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]);
    
    // Ejecuta el segundo comando, recibiendo la entrada de la tubería
    execlp("grep", "grep", "txt", NULL);
    
    perror("execlp grep");
    exit(EXIT_FAILURE);
}
```



---

### 💡 Consejos de uso
- Las tuberías son **unidireccionales**, los datos solo fluyen del extremo de escritura al de lectura.
- Asegúrate de **cerrar los extremos no utilizados** de la tubería en cada proceso para evitar bloqueos y fugas de recursos.
- Las tuberías tienen un **tamaño limitado de buffer** (típicamente 65536 bytes). Si se llena, las escrituras adicionales se bloquearán hasta que haya espacio disponible.
- Cuando **todos los descriptores** de escritura se cierran, una lectura en el extremo de lectura devolverá EOF (fin de archivo).

---

### 🔙 Retorno
Devuelve `0` si la operación fue exitosa.  
En caso de error, devuelve `-1` y establece `errno` con el código de error correspondiente.

---

### ⚠️ Errores
- `EMFILE`: El proceso ha alcanzado el límite de descriptores de archivo abiertos.
- `ENFILE`: El sistema ha alcanzado el límite total de descriptores de archivo abiertos.
- `EFAULT`: El array `pipefd` no es válido.

---

### 🧭 Información adicional

<summary>📎 ¿Por qué se usa en Minishell?</summary>

En Minishell, `pipe()` es fundamental para implementar el operador de tubería `|` que permite conectar la salida de un comando con la entrada de otro. Por ejemplo, en el comando `ls -l | grep txt`, se utiliza una tubería para:

1. Capturar la salida de `ls -l`
2. Enviar esa salida como entrada al comando `grep txt`

Una implementación básica podría ser:

```c
void execute_pipe(t_command *cmd1, t_command *cmd2)
{
    int pipefd[2];
    pid_t pid1, pid2;
    
    if (pipe(pipefd) == -1)
    {
        perror("minishell: pipe");
        return;
    }
    
    pid1 = fork();
    if (pid1 == 0)  // Primer hijo (ejecuta cmd1)
    {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        execute_command(cmd1);
        exit(EXIT_FAILURE);  // Por si falla la ejecución
    }
    
    pid2 = fork();
    if (pid2 == 0)  // Segundo hijo (ejecuta cmd2)
    {
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        execute_command(cmd2);
        exit(EXIT_FAILURE);  // Por si falla la ejecución
    }
    
    // Padre cierra ambos extremos de la tubería
    close(pipefd[0]);
    close(pipefd[1]);
    
    // Espera a que terminen ambos procesos
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}
```

Esta función ejecutaría dos comandos conectados por una tubería, permitiendo que los datos fluyan del primero al segundo.



---