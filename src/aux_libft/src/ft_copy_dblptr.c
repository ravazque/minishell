/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_copy_dblptr.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 23:04:34 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/27 13:44:16 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

char	**ft_copy_dblptr(char **envp)
{
	int		i;
	char	**env;

	i = 0;
	while (envp[i])
		i++;
	env = malloc(sizeof(char *) * (i + 1));
	if (!env)
		return (malloc_error(), NULL);
	i = 0;
	while (envp[i])
	{
		env[i] = ft_strdup(envp[i]);
		i++;
	}
	env[i] = NULL;
	return (env);
}
