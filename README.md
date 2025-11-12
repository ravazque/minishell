
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
│  ├── colors.h                         # Color definitions for terminal output
│  └── minishell.h                      # Main header with prototypes and structures
├──┬ src/
│  ├── minishell.c                      # Main entry point
│  ├──┬ aux_libft/                      # Custom library functions
│  │  ├── include/                      # Library headers
│  │  └── src/                          # Library implementations
│  ├──┬ aux_minishell/                  # Auxiliary minishell functions
│  │  ├── error.c                       # Error handling
│  │  ├── init.c                        # Initialization routines
│  │  ├── loop.c                        # Main shell loop
│  │  ├── mshlvl.c                      # Shell level management
│  │  └── underscore.c                  # $_ variable handling
│  ├──┬ built_ins/                      # Built-in commands
│  │  ├── builtin_echo.c                # echo command
│  │  ├── builtin_exit.c                # exit command
│  │  ├── builtin_pwd.c                 # pwd command
│  │  ├── builtin_unset.c               # unset command
│  │  ├── builtins_core.c               # Core built-in dispatcher
│  │  ├── builtins_utils.c              # Built-in utilities
│  │  ├──┬ builtin_cd/                  # cd command implementation
│  │  │  ├── builtin_cd.c               # Main cd logic
│  │  │  ├── builtin_cd_path.c          # Path resolution
│  │  │  └── builtin_cd_utils.c         # cd utilities
│  │  ├──┬ builtin_env/                 # env command implementation
│  │  │  ├── builtin_env.c              # Main env logic
│  │  │  ├── builtin_env_cmd.c          # env command parsing
│  │  │  ├── builtin_env_exec.c         # env execution
│  │  │  ├── builtin_env_fork.c         # env fork handling
│  │  │  └── builtins_setenv.c          # Environment variable setter
│  │  └──┬ builtin_export/              # export command implementation
│  │     ├── builtin_export.c           # Main export logic
│  │     ├── builtin_export_print.c     # Print exported variables
│  │     ├── builtin_export_set.c       # Set exported variables
│  │     ├── builtin_export_utils.c     # Export utilities
│  │     └── builtin_export_validate.c  # Export validation
│  ├──┬ cleaner/                        # Memory cleanup functions
│  │  ├── cleanup_exec.c                # Execution cleanup
│  │  ├── cleanup.c                     # General cleanup
│  │  └── free_structs.c                # Structure deallocation
│  ├──┬ execution/                      # Command execution
│  │  ├──┬ executor/                    # Main executor
│  │  │  ├── exec_child.c               # Child process execution
│  │  │  ├── exec_env.c                 # Environment for execution
│  │  │  ├── exec_path.c                # Path resolution
│  │  │  ├── exec_pipes.c               # Pipe management
│  │  │  ├── exec_single.c              # Single command execution
│  │  │  ├── exec_utils.c               # Execution utilities
│  │  │  ├── exec_wait.c                # Process wait handling
│  │  │  └── executor.c                 # Main executor logic
│  │  ├──┬ heredocs/                    # Heredoc implementation
│  │  │  ├── heredocs_collect.c         # Line collection
│  │  │  ├── heredocs.c                 # Main heredoc logic
│  │  │  └── heredocs_handler.c         # Heredoc handlers
│  │  └──┬ redirs/                      # Redirection handling
│  │     └── redirections.c             # Redirection setup
│  ├──┬ fork_bomb/                      # Fork bomb detection
│  │  ├── fork_bomb.c                   # Fork bomb handler
│  │  └── fork_bomb_utils.c             # Detection utilities
│  ├──┬ parse/                          # Input parsing
│  │  ├── parse.c                       # Main parser
│  │  ├── parse_assign.c                # Assignment parsing
│  │  ├── parse_utils.c                 # Parser utilities
│  │  ├──┬ expand/                      # Variable expansion
│  │  │  ├── expand.c                   # Main expander
│  │  │  ├── expand_extract.c           # Variable extraction
│  │  │  ├── expand_redirs.c            # Redirection expansion
│  │  │  ├── expand_str.c               # String expansion
│  │  │  ├── expand_tilde.c             # Tilde expansion
│  │  │  ├── expand_utils.c             # Expansion utilities
│  │  │  ├── expand_vars.c              # Variable expansion
│  │  │  └──┬ expand_split/             # Word splitting
│  │  │     ├── expand_split.c          # Main word splitter
│  │  │     ├── expand_split_add.c      # Add split tokens
│  │  │     ├── expand_split_count.c    # Count split words
│  │  │     ├── expand_split_helpers.c  # Split helpers
│  │  │     ├── expand_split_utils.c    # Split utilities
│  │  │     └── expand_split_word.c     # Word extraction
│  │  ├──┬ lexer/                       # Lexical analysis
│  │  │  ├── lexer.c                    # Main lexer
│  │  │  ├── lexer_classify.c           # Token classification
│  │  │  ├── lexer_cmd_create.c         # Command creation
│  │  │  ├── lexer_pipe_split.c         # Pipe splitting
│  │  │  ├── lexer_pipe_valid.c         # Pipe validation
│  │  │  ├── lexer_quotes.c             # Quote handling
│  │  │  ├── lexer_redir.c              # Redirection tokens
│  │  │  ├── lexer_redir_proc.c         # Redirection processing
│  │  │  ├── lexer_token.c              # Token creation
│  │  │  ├── lexer_token_arr.c          # Token array conversion
│  │  │  └── lexer_utils.c              # Lexer utilities
│  │  └──┬ tokenizer/                   # Tokenization
│  │     ├── tokenizer.c                # Main tokenizer
│  │     ├── tokenizer_handlers.c       # Character handlers
│  │     ├── tokenizer_helpers.c        # Helper functions
│  │     ├── tokenizer_parts.c          # Token parts handling
│  │     └── tokenizer_utils.c          # Tokenizer utilities
│  ├──┬ prompt/                         # Prompt generation
│  │  ├── prompt.c                      # Main prompt builder
│  │  ├── prompt_git.c                  # Git branch detection
│  │  ├── prompt_path.c                 # Path formatting
│  │  └── prompt_utils.c                # Prompt utilities
│  └──┬ signals/                        # Signal handling
│     ├── signals.c                     # Main signal setup
│     └── signal_handlers.c             # Signal handlers
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

