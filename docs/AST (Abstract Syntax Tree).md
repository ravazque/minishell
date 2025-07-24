## Integrar esto con tu código existente

### 1. **Añadir a `minishell.h`**

Copiar las estructuras y declaraciones de funciones del código de arriba a tu archivo `.h`.

### 2. **Reemplazar `main_tokenizer`**

Sustituir la función `main_tokenizer` actual por la nueva versión que incluye el AST.

### 3. **Crear nuevo archivo `ast.c`**

Poner todas las implementaciones de las funciones del AST en un nuevo archivo `ast.c`.

## Lo que **NO cambia**

- ✅ Lexer (`parse_input`, `process_char`, etc.) - **funciona perfecto**
- ✅ Enums (`t_token_type`) - **los reutilizamos**
- ✅ Built-ins (`builtin_cd`, `builtin_pwd`) - **los integramos**
- ✅ Estructura `t_data` - **la seguimos usando**

## Lo que se **añade**

- ➕ Estructuras del AST que usan los enums existentes
- ➕ Parser que convierte los tokens en AST
- ➕ Ejecutor que usa el AST para llamar a los built-ins

## Flujo actual vs nuevo

**Antes:**

```
Tokens → detective_conan() → ejecutar
```

**Ahora:**

```
Tokens → build_ast_from_tokens() → execute_ast() → tus built-ins
```

## Para probar

1. Integrar el código.
2. Pruebas con comandos simples.
3. Ver tanto los tokens como el AST impreso.
4. Los Built-ins deberían ejecutarse igual que antes.

```c
/* ************************************************************************** */
/*                    AST INTEGRATION FOR YOUR MINISHELL                     */
/* ************************************************************************** */

// AÑADIR AL minishell.h - Nuevos enums para el AST
typedef enum e_ast_node_type
{
	AST_COMMAND,     // Comando simple: ls -la
	AST_PIPELINE,    // Pipeline: cmd1 | cmd2  
	AST_AND,         // Operador &&
	AST_OR,          // Operador ||
	AST_SUBSHELL     // Subshell o agrupación ()
}	t_ast_node_type;

// AÑADIR AL minishell.h - Estructura para redirecciones en el AST
typedef struct s_ast_redirect
{
	t_token_type			type;      // Usa tu enum existente: TOKEN_REDIR_IN, TOKEN_REDIR_OUT, etc.
	char					*file;     // Archivo o delimitador
	int						fd;       // File descriptor (opcional)
	struct s_ast_redirect	*next;    // Lista enlazada de redirecciones
}	t_ast_redirect;

// AÑADIR AL minishell.h - Nodo principal del AST
typedef struct s_ast_node
{
	t_ast_node_type		type;
	
	// Para comandos simples (AST_COMMAND)
	char				**args;       // Array de argumentos ["ls", "-la", NULL]
	t_ast_redirect		*redirects;  // Lista de redirecciones
	
	// Para nodos binarios (AST_PIPELINE, AST_AND, AST_OR)
	struct s_ast_node	*left;       // Hijo izquierdo
	struct s_ast_node	*right;      // Hijo derecho
	
	// Para subshells (AST_SUBSHELL)
	struct s_ast_node	*child;      // Contenido del subshell
}	t_ast_node;

/* ************************************************************************** */
/*                         FUNCIONES DEL AST (NUEVAS)                        */
/* ************************************************************************** */

// Funciones para crear nodos del AST
t_ast_node		*create_command_node(char **args, t_ast_redirect *redirects);
t_ast_node		*create_pipeline_node(t_ast_node *left, t_ast_node *right);
t_ast_node		*create_and_node(t_ast_node *left, t_ast_node *right);
t_ast_node		*create_or_node(t_ast_node *left, t_ast_node *right);
t_ast_node		*create_subshell_node(t_ast_node *child);

// Parser del AST (convierte tus tokens en AST)
t_ast_node		*build_ast_from_tokens(t_token *token_list);

// Funciones auxiliares del AST
void			free_ast(t_ast_node *node);
void			print_ast(t_ast_node *node, int depth);
t_ast_redirect	*create_redirect_node(t_token_type type, char *file);

// Ejecutor del AST
int				execute_ast(t_data *data, t_ast_node *ast);

/* ************************************************************************** */
/*                         IMPLEMENTACIONES                                   */
/* ************************************************************************** */

// Crear un nodo de comando simple
t_ast_node *create_command_node(char **args, t_ast_redirect *redirects)
{
	t_ast_node *node = malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	
	node->type = AST_COMMAND;
	node->args = args;
	node->redirects = redirects;
	node->left = NULL;
	node->right = NULL;
	node->child = NULL;
	
	return (node);
}

// Crear un nodo de pipeline
t_ast_node *create_pipeline_node(t_ast_node *left, t_ast_node *right)
{
	t_ast_node *node = malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	
	node->type = AST_PIPELINE;
	node->args = NULL;
	node->redirects = NULL;
	node->left = left;
	node->right = right;
	node->child = NULL;
	
	return (node);
}

// Crear nodo de redirección
t_ast_redirect *create_redirect_node(t_token_type type, char *file)
{
	t_ast_redirect *redirect = malloc(sizeof(t_ast_redirect));
	if (!redirect)
		return (NULL);
	
	redirect->type = type;  // TOKEN_REDIR_IN, TOKEN_REDIR_OUT, etc.
	redirect->file = strdup(file);
	redirect->fd = -1;
	redirect->next = NULL;
	
	return (redirect);
}

// Parser principal del AST - ESTA ES LA FUNCIÓN CLAVE
t_ast_node *build_ast_from_tokens(t_token *token_list)
{
	// Esta función reemplazará tu detective_conan actual
	// Construye el AST y luego lo ejecuta
	
	if (!token_list)
		return (NULL);
	
	// Por ahora, implementación simple para comandos básicos
	// TODO: Expandir para pipelines, &&, ||
	
	// Contar argumentos
	t_token *current = token_list;
	int arg_count = 0;
	t_ast_redirect *redirects = NULL;
	t_ast_redirect *last_redirect = NULL;
	
	// Primera pasada: contar argumentos y detectar redirecciones
	while (current && current->type != TOKEN_EOF)
	{
		if (current->type == TOKEN_WORD)
			arg_count++;
		else if (current->type == TOKEN_REDIR_IN || 
				 current->type == TOKEN_REDIR_OUT ||
				 current->type == TOKEN_APPEND || 
				 current->type == TOKEN_HEREDOC)
		{
			// Crear redirección
			t_ast_redirect *redirect = create_redirect_node(current->type, current->next->value);
			if (!redirects)
				redirects = redirect;
			else
				last_redirect->next = redirect;
			last_redirect = redirect;
			
			current = current->next; // Saltar el archivo
		}
		current = current->next;
	}
	
	if (arg_count == 0)
		return (NULL);
	
	// Crear array de argumentos
	char **args = malloc(sizeof(char*) * (arg_count + 1));
	if (!args)
		return (NULL);
	
	// Segunda pasada: llenar argumentos
	current = token_list;
	int i = 0;
	while (current && current->type != TOKEN_EOF && i < arg_count)
	{
		if (current->type == TOKEN_WORD)
		{
			args[i] = strdup(current->value);
			i++;
		}
		else if (current->type == TOKEN_REDIR_IN || 
				 current->type == TOKEN_REDIR_OUT ||
				 current->type == TOKEN_APPEND || 
				 current->type == TOKEN_HEREDOC)
		{
			current = current->next; // Saltar el archivo
		}
		current = current->next;
	}
	args[i] = NULL;
	
	return (create_command_node(args, redirects));
}

// NUEVA función que reemplaza detective_conan
int execute_ast(t_data *data, t_ast_node *ast)
{
	if (!data || !ast)
		return (-1);
	
	if (ast->type == AST_COMMAND)
	{
		// Ejecutar comando simple
		if (!ast->args || !ast->args[0])
			return (-1);
		
		// Usar tu detective_conan existente pero con el AST
		if (ft_strcmp(ast->args[0], "env") == 0)
			return (env_update(data, "_=", "env"), env_printer(data), 1);
		else if (ft_strcmp(ast->args[0], "pwd") == 0)
			return (env_update(data, "_=", "pwd"), builtin_pwd(), 1);
		else if (ft_strcmp(ast->args[0], "cd") == 0)
		{
			char *path = (ast->args[1]) ? ast->args[1] : NULL;
			return (env_update(data, "_=", "cd"), builtin_cd(data, path), 1);
		}
		
		// Comando no encontrado
		return (write(2, "Command not found\n", 18), -1);
	}
	else if (ast->type == AST_PIPELINE)
	{
		// TODO: Implementar pipelines
		printf("Pipeline detectado - pendiente de implementar\n");
		return (0);
	}
	else if (ast->type == AST_AND)
	{
		// TODO: Implementar &&
		printf("Operador && detectado - pendiente de implementar\n");
		return (0);
	}
	else if (ast->type == AST_OR)
	{
		// TODO: Implementar ||
		printf("Operador || detectado - pendiente de implementar\n");
		return (0);
	}
	
	return (-1);
}

// Función de debug para visualizar el AST
void print_ast(t_ast_node *node, int depth)
{
	int i;
	
	if (!node)
		return;
	
	// Indentación
	for (i = 0; i < depth; i++)
		printf("  ");
	
	switch (node->type)
	{
		case AST_COMMAND:
			printf("COMMAND: ");
			if (node->args)
			{
				i = 0;
				while (node->args[i])
				{
					printf("%s ", node->args[i]);
					i++;
				}
			}
			printf("\n");
			
			// Imprimir redirecciones
			t_ast_redirect *redir = node->redirects;
			while (redir)
			{
				for (int j = 0; j < depth + 1; j++)
					printf("  ");
				printf("REDIRECT: type=%d, file=%s\n", redir->type, redir->file);
				redir = redir->next;
			}
			break;
			
		case AST_PIPELINE:
			printf("PIPELINE:\n");
			print_ast(node->left, depth + 1);
			print_ast(node->right, depth + 1);
			break;
			
		case AST_AND:
			printf("AND:\n");
			print_ast(node->left, depth + 1);
			print_ast(node->right, depth + 1);
			break;
			
		case AST_OR:
			printf("OR:\n");
			print_ast(node->left, depth + 1);
			print_ast(node->right, depth + 1);
			break;
			
		case AST_SUBSHELL:
			printf("SUBSHELL:\n");
			print_ast(node->child, depth + 1);
			break;
	}
}

// Liberar memoria del AST
void free_ast(t_ast_node *node)
{
	if (!node)
		return;
	
	// Liberar argumentos
	if (node->args)
	{
		int i = 0;
		while (node->args[i])
		{
			free(node->args[i]);
			i++;
		}
		free(node->args);
	}
	
	// Liberar redirecciones
	t_ast_redirect *redir = node->redirects;
	while (redir)
	{
		t_ast_redirect *next = redir->next;
		free(redir->file);
		free(redir);
		redir = next;
	}
	
	// Liberar hijos recursivamente
	free_ast(node->left);
	free_ast(node->right);
	free_ast(node->child);
	
	free(node);
}

/* ************************************************************************** */
/*                    MODIFICACIÓN DE TU main_tokenizer                      */
/* ************************************************************************** */

// REEMPLAZAR tu main_tokenizer actual con esta versión:
int main_tokenizer(t_data *data, char *line)
{
	t_token *tok_list;
	t_ast_node *ast;
	
	// Tu lexer existente (sin cambios)
	tok_list = parse_input(line);
	if (!tok_list)
		return (-1);
	
	// NUEVO: Construir AST desde los tokens
	ast = build_ast_from_tokens(tok_list);
	if (!ast)
	{
		free_tokens(tok_list);
		return (-1);
	}
	
	// Debug: imprimir tokens y AST
	printf("=== TOKENS ===\n");
	print_tokens(tok_list);
	printf("\n=== AST ===\n");
	print_ast(ast, 0);
	printf("\n");
	
	// NUEVO: Ejecutar usando el AST
	int result = execute_ast(data, ast);
	
	// Limpiar memoria
	free_tokens(tok_list);
	free_ast(ast);
	
	return (result);
}
```

