/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_print.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:40:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/30 17:40:00 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	print_var_name(char *var, int *j)
{
	while (var[*j] != '=')
	{
		printf("%c", var[*j]);
		(*j)++;
	}
}

static void	print_var_value(char *var, int *j)
{
	printf("%c\"", var[*j]);
	(*j)++;
	while (var[*j])
	{
		printf("%c", var[*j]);
		(*j)++;
	}
	printf("\"\n");
}

static void	print_single_export(char *var)
{
	int	j;

	if (ft_strncmp(var, "_=", 2) == 0)
		return ;
	j = 0;
	printf("declare -x ");
	if (ft_strchr(var, '='))
	{
		print_var_name(var, &j);
		print_var_value(var, &j);
	}
	else
		printf("%s\n", var);
}

void	ft_putexport(char ***env)
{
	int		i;
	char	**abc_env;

	i = 0;
	abc_env = ft_sort_env(*env);
	if (!abc_env)
		return (malloc_error());
	while (abc_env[i])
	{
		print_single_export(abc_env[i]);
		i++;
	}
	free_dblptr(abc_env);
}
