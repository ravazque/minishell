/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tilde.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 15:53:10 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/05 16:05:42 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

char	*process_tilde_expansion(int *i, t_mini *mini)
{
	char	*cached;

	(*i)++;
	cached = get_home_cached(mini);
	if (!cached)
		return (ft_strdup("~"));
	return (ft_strdup(cached));
}

int	should_expand_tilde(const char *s, int i, int exp)
{
	if (s[i] != '~')
		return (0);
	if (!exp)
		return (0);
	if (i != 0)
		return (0);
	if (s[i + 1] && s[i + 1] != '/' && s[i + 1] != ' ')
		return (0);
	return (1);
}
