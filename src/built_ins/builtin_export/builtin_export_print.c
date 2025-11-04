/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_print.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:40:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/04 01:16:49 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	print_var_name(char *var, int *j)
{
	while (var[*j] != '=')
	{
		ft_putchar_fd(var[*j], STDOUT_FILENO);
		(*j)++;
	}
}

static void	print_var_value(char *var, int *j)
{
	ft_putchar_fd(var[*j], STDOUT_FILENO);
	ft_putchar_fd('"', STDOUT_FILENO);
	(*j)++;
	while (var[*j])
	{
		ft_putchar_fd(var[*j], STDOUT_FILENO);
		(*j)++;
	}
	ft_putstr_fd("\"\n", STDOUT_FILENO);
}

static void	print_single_export(char *var)
{
	int	j;

	if (ft_strncmp(var, "_=", 2) == 0)
		return ;
	j = 0;
	ft_putstr_fd("declare -x ", STDOUT_FILENO);
	if (ft_strchr(var, '='))
	{
		print_var_name(var, &j);
		print_var_value(var, &j);
	}
	else
	{
		ft_putstr_fd(var, STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
	}
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
