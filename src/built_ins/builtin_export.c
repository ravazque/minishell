/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 01:30:15 by ptrapero          #+#    #+#             */
/*   Updated: 2025/10/13 16:18:57 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**ft_sort_env(char **env)
{
	char	**abc_env;
	int		i;
	char	*temp;

	abc_env = ft_copy_dblptr(env);
	if (!abc_env)
		return (NULL);
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

char	**ft_split2(char *s, char c)
{
	char	**matrix;
	int		i;
	int		j;

	i = 0;
	j = 0;
	matrix = malloc(sizeof(char *) * 3);
	if (!matrix)
		return (NULL);
	while (s[j] && s[j] != c)
		j++;
	matrix[0] = ft_substr(s, i, j);
	if (!matrix[0])
		return (free(matrix), NULL);
	j++;
	while (s[i])
		i++;
	matrix[1] = ft_substr(s, j, (i - j));
	if (!matrix[1])
		return (free(matrix[0]), free(matrix), NULL);
	matrix[2] = NULL;
	return (matrix);
}

void	ft_putexport(char ***env)
{
	int		i;
	int		j;
	char	**abc_env;

	i = 0;
	abc_env = ft_sort_env(*env);
	if (!abc_env)
		return (malloc_error());
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

static int	var_exists_with_value(char *name, char **env)
{
	int		i;
	int		n_len;

	if (!name || !env)
		return (0);
	n_len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], name, n_len) && env[i][n_len] == '=')
			return (1);
		i++;
	}
	return (0);
}

void	ft_setexport(char **argv, char ***env, int i, int flag)
{
	char	**args;

	if (flag == 0)
	{
		if (ft_strchr(argv[i], '='))
		{
			args = ft_split2(argv[i], '=');
			if (!args)
				return (malloc_error());
			if (args[1])
				ft_setenv(args[0], args[1], env);
			else if (!args[1])
				ft_setenv(args[0], "", env);
			free_dblptr(args);
		}
		else if (!ft_strchr(argv[i], '='))
		{
			if (!var_exists_with_value(argv[i], *env))
				ft_setenv(argv[i], NULL, env);
		}
	}
}

void	builtin_export(t_mini *mini)
{
	int	i;
	int	j;
	int	flag;
	int	has_error;

	if (ft_argc(mini->cmds->tokens) == 1)
	{
		ft_putexport(&mini->env);
		mini->exit_sts = 0;
		return ;
	}
	i = 1;
	has_error = 0;
	while (ft_argc(mini->cmds->tokens) > i)
	{
		j = 0;
		flag = 0;
		if (mini->cmds->tokens[i][0] == '=' || mini->cmds->tokens[i][0] == '\0')
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(mini->cmds->tokens[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			flag = 1;
			has_error = 1;
		}
		while (mini->cmds->tokens[i][j] && mini->cmds->tokens[i][j] != '=' && flag == 0)
		{
			if (!((ft_isalnum(mini->cmds->tokens[i][j]) || (mini->cmds->tokens[i][j] == '_'))
			&& (ft_isalpha(mini->cmds->tokens[i][0]) || (mini->cmds->tokens[i][0] == '_'))))
			{
				ft_putstr_fd("minishell: export: `", 2);
				ft_putstr_fd(mini->cmds->tokens[i], 2);
				ft_putstr_fd("': not a valid identifier\n", 2);
				flag = 1;
				has_error = 1;
			}
			j++;
		}
		ft_setexport(mini->cmds->tokens, &mini->env, i, flag);
		i++;
	}
	if (has_error)
		mini->exit_sts = 1;
	else
		mini->exit_sts = 0;
}
