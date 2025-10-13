/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ptrapero <ptrapero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 22:01:29 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/08 20:51:33 by ptrapero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	builtin_unset(t_mini *mini)
{
	int	i;
	int	j;
	int	len;

	i = 0;
	while (ft_argc(mini->cmds->tokens) > ++i)
	{
		j = 0;
		len = ft_strlen(mini->cmds->tokens[i]);
		while (mini->env[j])
		{
			if (!ft_strncmp(mini->cmds->tokens[i], mini->env[j], len)
				&& (mini->env[j][len] == '\0' || mini->env[j][len] == '='))
			{
				free(mini->env[j]);
				while (mini->env[j + 1])
				{
					mini->env[j] = mini->env[j + 1];
					j++;
				}
				mini->env[j] = NULL;
				break ;
			}
			j++;
		}
	}
}
