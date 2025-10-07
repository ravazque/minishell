/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 22:01:29 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/08 00:39:27 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// void	ft_unsetclean(char **argv, char ***env)
// {
// 	if (!ft_strncmp(argv[i], (*env)[j], ft_strlen(argv[i]))
// 		&& ((*env)[j][ft_strlen(argv[i])] == '='
// 			|| (*env)[j][ft_strlen(argv[i])] == '\0'))//buscar en el entorno que esa variable tenga despues = || \0
// 	{
// 		free((*env)[j]);
// 		while ((*env)[j + 1])
// 		{
// 			(*env)[j] = (*env)[j + 1];
// 			j++;
// 		}
// 		(*env)[j] = NULL;
// 		break ;
// 	}
// }

void	builtin_unset(t_mini *mini)
{
	int	i;
	int	j;
	int	len;

	i = 0;
	while (ft_argc(mini->cmds->tokens) > ++i)
	{
		//printf("%s\n", mini->cmds->tokens[i]);
		j = 0;
		len = ft_strlen(mini->cmds->tokens[i]);
		//printf("%i\n", ft_argc(mini->env));
		while (mini->env[j])
		{
			//printf("%i\n", j);
			//printf("env->%s c->%c\n", mini->env[j], mini->env[j][len]);
			if (!ft_strncmp(mini->cmds->tokens[i], mini->env[j], len)
				&& (mini->env[j][len] == '\0' || mini->env[j][len] == '='))
			{
				//printf("3\n");
				free(mini->env[j]);
				while ((*mini->env)[j + 1])
				{
					(*mini->env)[j] = (*mini->env)[j + 1];
					j++;
				}
				mini->env[j] = NULL;
				break ;
			}
			j++;
		}
	}
}
