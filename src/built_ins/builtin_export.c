/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 01:30:15 by ptrapero          #+#    #+#             */
/*   Updated: 2025/10/08 01:11:16 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	**ft_sort_env(char **env)
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

static char	**ft_split2(char *s, char c)
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
	i = j;
	while (s[i])
		i++;
	matrix[1] = ft_substr(s, j, (i - j));
	if (!matrix[1])
		return (free(matrix[0]), free(matrix), NULL);
	matrix[2] = NULL;
	return (matrix);
}

static void	ft_putexport(char ***env)
{
	int		i;
	int		j;
	char	**abc_env;

	abc_env = ft_sort_env(*env);
	if (!abc_env)
		return ;
	i = 0;
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

static int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	ft_setexport(char *arg, char ***env)
{
	char	**args;

	if (!is_valid_identifier(arg))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (1);
	}
	if (ft_strchr(arg, '='))
	{
		args = ft_split2(arg, '=');
		if (!args)
			return (1);
		if (args[1])
			ft_setenv(args[0], args[1], env);
		else
			ft_setenv(args[0], "", env);
		free_dblptr(args);
	}
	else
		ft_setenv(arg, NULL, env);
	return (0);
}

void	builtin_export(t_mini *mini)
{
	int	i;

	if (ft_argc(mini->cmds->tokens) == 1)
	{
		ft_putexport(&mini->env);
		mini->exit_sts = 0;
		return ;
	}
	i = 1;
	mini->exit_sts = 0;
	while (mini->cmds->tokens[i])
	{
		mini->exit_sts = ft_setexport(mini->cmds->tokens[i], &mini->env);
		i++;
	}
}
