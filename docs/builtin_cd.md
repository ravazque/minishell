
# Minishell - Built-in "cd"

## 1. Búsqueda de Variables de Entorno

### builtin_cd_original.c (INCORRECTO)

```c
path = getenv("HOME");
// ...
path = getenv("OLDPWD");
```

**Problema**: Usa `getenv()` del sistema que busca en el entorno global del proceso, no en el entorno local de minishell.

### builtin_cd.c (CORRECTO)

```c
path = get_localenv("HOME", mini->env);
// ...
path = get_localenv("OLDPWD", mini->env);
```

**Solución**: Implementa `get_localenv()` que busca en `mini->env`:

```c
static char	*get_localenv(const char *name, char **env)
{
	int		i;
	int		n_len;

	if (!name || !env)
		return (NULL);
	n_len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], name, n_len) && env[i][n_len] == '=')
			return (env[i] + n_len + 1);
		i++;
	}
	return (NULL);
}
```

---

## 2. Bug Crítico en ft_setenv()

### builtin_cd_original.c (BUG GRAVE)

```c
i = 0;
while ((*env)[i] && !ft_strncmp((*env)[i], var, ft_strlen(name) + 1))
	i++;
if (!ft_strncmp((*env)[i], var, ft_strlen(name) + 1))
{
	free((*env)[i]);
	(*env)[i] = var;
	return ;
}
```

**Problemas**:

- El `while` busca variables que **NO coinciden** (por el `!`)
- Luego verifica si coincide (lógica contradictoria)
- Compara con `var` (que incluye el valor) en lugar de solo el `name`
- No verifica el carácter `=` correctamente

### builtin_cd.c (CORRECTO)

```c
n_len = ft_strlen(name);
i = 0;
while ((*env)[i])
{
	if (!ft_strncmp((*env)[i], name, n_len) && (*env)[i][n_len] == '=')
	{
		free((*env)[i]);
		(*env)[i] = var;
		return ;
	}
	i++;
}
```

**Solución**:

- Recorre todas las variables
- Compara solo el nombre (no el valor)
- Verifica explícitamente que después del nombre viene `=`
- Lógica correcta y clara

---

## 3. Gestión del Exit Status

### builtin_cd.c (CORRECTO)

```c
// En ft_cd_path():
return (ft_putstr_fd(ERR_HOME, 2), mini->exit_sts = 1, NULL);
return (ft_putstr_fd(ERR_OLDPWD, 2), mini->exit_sts = 1, NULL);
return (mini->exit_sts = 1, NULL); // Para too many arguments

// En builtin_cd():
if (chdir(path) < 0)
{
	// ...
	mini->exit_sts = 1;  // ERROR AL CAMBIAR DE DIRECTORIO
	return ;
}
// ...
mini->exit_sts = 0;  // ÉXITO
```

**Mejora**: Establece consistentemente el estado de salida en todos los casos.

---

## 4. Protección contra NULL de getcwd()

### builtin_cd_original.c (PELIGROSO)

```c
oldpwd = getcwd(NULL, 0);
// ...
pwd = getcwd(NULL, 0);
ft_setenv("OLDPWD", oldpwd, &(mini->env));
ft_setenv("PWD", pwd, &(mini->env));
free(oldpwd);
free(pwd);
```

**Problema**: Si `getcwd()` falla (retorna NULL), se pasa NULL a `ft_setenv()`.

### builtin_cd.c (SEGURO)

```c
oldpwd = getcwd(NULL, 0);
// ...
pwd = getcwd(NULL, 0);
if (oldpwd)
	ft_setenv("OLDPWD", oldpwd, &(mini->env));
if (pwd)
	ft_setenv("PWD", pwd, &(mini->env));
free(oldpwd);
free(pwd);
```

**Solución**: Verifica que los punteros no sean NULL antes de usarlos.

---

