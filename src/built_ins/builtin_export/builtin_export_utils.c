/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:41:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/30 17:41:00 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

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

int	var_exists_with_value(char *name, char **env)
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
