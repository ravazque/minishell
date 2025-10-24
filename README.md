
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

---

Creación de archivos como caracteres delimitadores del tokenizer
no se crean los archivos si falla la ejecución de las redirecciones (comporbar con bash)
leaks con el heredoc y los buitins
cerramos demás una vez los fd por cada heredoc en ejecucción
faltan variables locales
ajusta $_ para que tenga un pseudo-path
ejecutar con "env -i" (sin enviroment)
ajustar, en el clusetr, los exit status a la versión de bash de los ordenadores

---
