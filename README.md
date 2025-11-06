
# MINISHELL

## 📖 About

"minishell" is a project at 42 Madrid that involves creating a simple shell that replicates basic functionalities of bash. This project introduces command parsing, process management, signal handling, and teaches how to implement a command-line interpreter in a systematic way.

The goal is to implement a shell program that can execute commands, manage environment variables, handle redirections and pipes, and provide a user-friendly interactive experience similar to bash.

## 🎯 Objectives

- Understanding and implementing command-line parsing and tokenization
- Learning about process creation and management with fork() and execve()
- Managing pipes and redirections for inter-process communication
- Implementing built-in shell commands
- Handling signals (SIGINT, SIGQUIT, EOF)
- Managing environment variables and shell variables
- Implementing command history functionality
- Handling quotes and special characters properly

## 📋 Function Overview

<details>
<summary><strong>minishell</strong></summary>

### Concepts

**Description:** A minimal shell implementation that mimics bash behavior
**Usage:** `./minishell` or `./minishell -c "command"`
**Behavior:** Reads and executes commands interactively or from arguments

```c
int main(int argc, char **argv, char **envp);
```

### Use of processes and signal handling
The implementation of **minishell** requires creating processes and handling signals. For this, several system calls are used:

The main system calls and functions are:

- **`fork()`** ➜ Creates a child process to execute commands.

```c
pid_t pid = fork();
```

- **`execve()`** ➜ Replaces the current process with a new program.

```c
execve(cmd_path, cmd_args, envp);
```

- **`pipe()`** ➜ Creates a pipe for communication between processes.

```c
int pipefd[2];
pipe(pipefd);
```

- **`dup2()`** ➜ Duplicates file descriptors for redirection.

```c
dup2(fd, STDIN_FILENO);
```

- **`signal()`** / **`sigaction()`** ➜ Sets up signal handlers for Ctrl-C, Ctrl-\, etc.

```c
signal(SIGINT, signal_handler);
```

- **`readline()`** ➜ Reads a line from the terminal with line editing capabilities.

```c
char *line = readline("minishell> ");
```

- **`add_history()`** ➜ Adds a command to the history list.

```c
add_history(line);
```

These functions are essential for implementing **minishell**, as they allow command execution, process management, and user interaction.

</details>

<details>
<summary><strong>Features and Behavior</strong></summary>

### Interactive Mode

- **Prompt**: Displays custom prompt with `user@hostname:path (git_branch) $`
- **Command execution**: Supports absolute paths, relative paths, and PATH resolution
- **History**: Full command history navigation with up/down arrows
- **Signals**: Proper handling of:
  - `Ctrl-C`: Interrupts current command (SIGINT)
  - `Ctrl-D`: Exits shell when line is empty (EOF)
  - `Ctrl-\`: Does nothing in interactive mode (SIGQUIT)

### Supported Features

**Environment Variables:**
- `$VAR` - Environment variable expansion
- `$?` - Exit status of last command
- `$0` - Shell name
- `$$` - Shell process ID
- `$_` - Last argument of previous command

**Redirections:**
- `<` - Input redirection
- `>` - Output redirection (truncate)
- `>>` - Output redirection (append)
- `<<` - Heredoc (read until delimiter)

**Pipes:**
- `|` - Connect output of one command to input of another

**Quotes:**
- `'single quotes'` - Preserve literal value of all characters
- `"double quotes"` - Preserve literal value except `$` and `\`

### Built-in Commands

- `echo [-n]` - Print arguments to stdout
- `cd [path]` - Change working directory
- `pwd` - Print working directory
- `export [var=value]` - Set environment variables
- `unset [var]` - Unset environment variables
- `env` - Display environment variables
- `exit [n]` - Exit shell with status code

### Command Mode

```bash
./minishell -c "command"
# Executes command and exits
```

</details>

## 🚀 Installation & Structure

<details>
<summary><strong>📥 Download & Compilation</strong></summary>

<br>

```bash
# Clone the repository
git clone https://github.com/ravazque/minishell.git
cd minishell

# Compile the program
make

# Clean object files
make clean

# Clean everything including executable
make fclean

# Recompile everything
make re

# Run the program
./minishell
```

<br>

</details>

<details>
<summary><strong>📁 Project Structure</strong></summary>

<br>

```
minishell/
├──┬ include/
│  ├── colors.h                         # Header file with colors
│  └── minishell.h                      # Header file with prototypes and structures
├──┬ src/
│  └── minishell.c                      # In the process of refactoring and delivery
├── Makefile                            # Compilation rules
└── README.md                           # Project documentation
```

<br>

</details>

## 💡 Key Learning Outcomes

The minishell project teaches advanced system programming and shell implementation concepts:

- **Process Management**: Understanding fork(), execve(), and wait() system calls
- **Command Parsing**: Learning to tokenize and parse complex command-line syntax
- **Signal Handling**: Proper management of SIGINT, SIGQUIT, and EOF signals
- **File Descriptor Management**: Deep knowledge of redirections and pipes
- **Environment Management**: Manipulating and maintaining environment variables
- **Error Handling**: Robust error checking and proper exit codes
- **Memory Management**: Preventing leaks in a complex interactive program

## ⚙️ Technical Specifications

- **Language**: C (C90 standard)
- **Compiler**: cc with flags `-Wall -Wextra -Werror`
- **System Calls**: fork(), execve(), pipe(), dup2(), waitpid(), signal()
- **External Functions**: readline(), add_history() (GNU Readline library)
- **Platform**: UNIX-like systems (Linux, macOS)
- **Memory Management**: Proper cleanup of all allocated memory and file descriptors
- **Process Handling**: Parent-child process synchronization and signal propagation
- **Exit Codes**: Proper exit status propagation and `$?` handling

---

> [!NOTE]
> This project serves as a comprehensive introduction to shell programming and demonstrates proficiency in process management parsing, and inter-process communication concepts.

