/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_validate.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:43:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/30 17:43:00 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	print_export_error(char *token)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(token, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

static int	is_valid_start(char c)
{
	if (ft_isalpha(c) || c == '_')
		return (1);
	return (0);
}

static int	is_valid_char(char c)
{
	if (ft_isalnum(c) || c == '_')
		return (1);
	return (0);
}

static int	check_initial_errors(char *token)
{
	if (token[0] == '=' || token[0] == '\0')
	{
		print_export_error(token);
		return (1);
	}
	return (0);
}

int	validate_export_arg(char *token)
{
	int	j;

	if (check_initial_errors(token))
		return (1);
	j = 0;
	while (token[j] && token[j] != '=')
	{
		if (!is_valid_char(token[j]) || !is_valid_start(token[0]))
		{
			print_export_error(token);
			return (1);
		}
		j++;
	}
	return (0);
}
