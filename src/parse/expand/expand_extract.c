/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_extract.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 18:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/03 18:00:00 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static char	*handle_special_var(const char *str, int start, int *end)
{
	if (str[start] == '?')
	{
		*end = start + 1;
		return (ft_strdup("?"));
	}
	if (str[start] == '$')
	{
		*end = start + 1;
		return (ft_strdup("$$"));
	}
	if (str[start] == '0')
	{
		*end = start + 1;
		return (ft_strdup("0"));
	}
	return (NULL);
}

static char	*alloc_var(int len)
{
	char	*var;

	if (len == 0)
		return (NULL);
	var = (char *)malloc(len + 1);
	if (!var)
		return (NULL);
	return (var);
}

static void	copy_var(char *var, const char *str, int start, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		var[i] = str[start + i];
		i++;
	}
	var[len] = '\0';
}

static char	*extract_normal_var(const char *str, int start, int *end)
{
	char	*var;
	int		i;
	int		len;

	i = start;
	while (str[i] && is_valid_var_chr(str[i]))
		i++;
	*end = i;
	len = i - start;
	var = alloc_var(len);
	if (!var)
		return (NULL);
	copy_var(var, str, start, len);
	return (var);
}

char	*extract_var(const char *str, int start, int *end)
{
	char	*var;

	var = handle_special_var(str, start, end);
	if (var)
		return (var);
	return (extract_normal_var(str, start, end));
}
