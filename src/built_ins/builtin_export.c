/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 01:30:15 by ptrapero          #+#    #+#             */
/*   Updated: 2025/10/07 23:32:40 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**ft_sort_env(char **env)//f() que te ordena env alfabeticamente
{
	char	**abc_env;
	int		i;
	char	*temp;

	abc_env = ft_copy_dblptr(env);
	i = 0;
	while (abc_env[i + 1])
	{
		if (ft_strcmp(abc_env[i], abc_env[i + 1]) > 0)
		{
			temp = abc_env[i];
			abc_env[i] = abc_env[i + 1];
			abc_env[i + 1] = temp;
			i = 0;
		}
		else
			i++;
	}
	return (abc_env);
}

char	**ft_split2(char *s, char c)//te hace un split con solo 2 posiciones (+ null)
{
	char	**matrix;
	int		i;
	int		j;

	i = 0;
	j = 0;
	matrix = malloc(sizeof(char *) * 2 + 1);
	if (!matrix)
		return (NULL);
	while (s[j] && s[j] != c)
		j++;
	matrix[0] = ft_substr(s, i, j);
	if (!matrix[0])
		return (free_dblptr(matrix), NULL);
	j++;
	while (s[i])
		i++;
	matrix[1] = ft_substr(s, j, (i - j));
	if (!matrix[1])
		return (free_dblptr(matrix), NULL);
	matrix[2] = NULL;
	return (matrix);
}

	//NAME=VALUE
	//si var existe -> la actualiza
	//si var no existe -> la crea
	// contiene value

	//NAME=
	//existe -> cambia su valor a la cadena vacia ""
	//no existe -> la crea con valor como arriba y nombre en env es con =
	// value vacío

	//NAME
	//existe -> conserva el valor
	//no existe -> la crea sin valor y sin =
	// value como estaba

void	ft_putexport(char ***env)
{
	int		i;
	int		j;
	char	**abc_env;

	i = 0;
	abc_env = ft_sort_env(*env);
	while (abc_env[i])
	{
		j = 0;
		printf("declare -x ");
		if (ft_strchr(abc_env[i], '='))
		{
			while (abc_env[i][j] != '=')
				printf("%c", abc_env[i][j++]);
			printf("%c\"", abc_env[i][j++]);
			while (abc_env[i][j])
				printf("%c", abc_env[i][j++]);
			printf("\"\n");
		}
		else
			printf("%s\n", abc_env[i]);
		i++;
	}
	free_dblptr(abc_env);
}

void	ft_setexport(char **argv, char ***env, int i, int flag)
{
	char	**args;

	if (flag == 0)
	{
		if (ft_strchr(argv[i], '='))
		{
			args = ft_split2(argv[i], '=');
			if (args[1])//NAME=VALUE
				ft_setenv(args[0], args[1], env);
			else if (!args[1])//NAME=
				ft_setenv(args[0], "", env);
			free_dblptr(args);
		}
		else if (!ft_strchr(argv[i], '='))//NAME
			ft_setenv(argv[i], NULL, env);
	}
}

void	builtin_export(t_mini *mini)
{
	int	i;
	int	j;
	int	flag;

	if (ft_argc(mini->cmds->tokens) == 1)//!mini->cmds->tokens[1])
	{
		ft_putexport(&mini->env);
		return ;
	}
	i = 1;
	while (ft_argc(mini->cmds->tokens) > i)
	{
		j = 0;
		flag = 0;
		while (mini->cmds->tokens[i][j] && mini->cmds->tokens[i][j] != '=' && flag == 0)
		{
			if (!((ft_isalnum(mini->cmds->tokens[i][j]) || (mini->cmds->tokens[i][j] == '_'))//si en algun momento [j] no es letra/numero/_
			&& (ft_isalpha(mini->cmds->tokens[i][0]) || (mini->cmds->tokens[i][0] == '_'))))// && [0] no es letra/_
			{
				ft_putstr_fd("minishell: export: `", 2);
				ft_putstr_fd(mini->cmds->tokens[i], 2);
				ft_putstr_fd("': not a valid identifier\n", 2);
				flag = 1;
			}
			j++;
		}
		ft_setexport(mini->cmds->tokens, &mini->env, i, flag);
		i++;
	}
}
