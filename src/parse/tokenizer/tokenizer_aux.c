/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_aux.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 03:21:54 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/28 16:11:01 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	is_space(int c)
{
	if (c == ' ' || c == '\t')
		return (1);
	return (0);
}

int	quotes_balanced(const char *s)
{
	int		sq;
	int		dq;
	size_t	i;

	if (!s)
		return (1);
	sq = 0;
	dq = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] == '\'' && dq == 0)
			sq = !sq;
		else if (s[i] == '\"' && sq == 0)
			dq = !dq;
		i++;
	}
	if (sq == 0 && dq == 0)
		return (1);
	return (0);
}

int	is_assignment(const char *str)
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
	if (str[i] == '=')
		return (1);
	return (0);
}
