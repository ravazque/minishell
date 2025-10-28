
# Minishell

¡Un shell simple implementado en C que replica funcionalidades básicas de bash!

## 🔧 Compilación

```bash
make
```

## 🚀 Uso

```bash
# Modo interactivo
./minishell

# Modo comando
./minishell -c "comando"
```

## ✨ Características

- **Prompt personalizado** con usuario, hostname, ruta y branch de git
- **Historial** de comandos funcional
- **Expansión** de variables de entorno (`$VAR`, `$?`, `$0`, `$$` y `$_`)
- **Redirecciones y Heredoc**: `<`, `>`, `>>`, `<<`
- **Pipes** para encadenar comandos
- **Builtins**: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`
- **Señales**: Control de `Ctrl-C`, `Ctrl-D`, `Ctrl-\`
- **Comillas**: Manejo de `'` y `"` para proteger metacaracteres
