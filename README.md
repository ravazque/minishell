
# Minishell

Un shell simple implementado en C que replica funcionalidades básicas de bash.

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
- **Expansión** de variables de entorno (`$VAR`, `$?` `$$` `$0` `$_`)
- **Redirecciones**: `<`, `>`, `>>`, `<<`
- **Pipes y Heredoc** para encadenar comandos
- **Builtins**: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit` y variables locales
- **Señales**: Control de `Ctrl-C`, `Ctrl-D`, `Ctrl-\`
- **Comillas**: Manejo de `'` y `"` para proteger metacaracteres

## 📚 Documentación Técnica

Para entender la arquitectura interna del proyecto:

- **[Parse](docs/parse.md)** - Tokenización, lexer y expansión
- **[Executor](docs/executor.md)** - Ejecución de comandos y pipelines
- **[Builtins](docs/builtins.md)** - Comandos internos implementados
