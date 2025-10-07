/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_copy_dblptr.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 23:04:34 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/07 18:34:16 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	cleanup_partial_copy(char **env, int count)
{
	int	j;

	j = 0;
	while (j < count)
	{
		free(env[j]);
		j++;
	}
	free(env);
}

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
		if (!env[i])
		{
			cleanup_partial_copy(env, i);
			return (malloc_error(), NULL);
		}
		i++;
	}
	env[i] = NULL;
	return (env);
}
