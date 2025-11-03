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

static void	shift_env_vars(char **env, int pos)
{
	int	k;

	k = pos;
	while (env[k + 1])
	{
		env[k] = env[k + 1];
		k++;
	}
	env[k] = NULL;
}

static void	unset_single_var(char *var_name, char **env)
{
	int	j;
	int	len;

	j = 0;
	len = ft_strlen(var_name);
	while (env[j])
	{
		if (!ft_strncmp(var_name, env[j], len))
		{
			if (env[j][len] == '\0' || env[j][len] == '=')
			{
				free(env[j]);
				shift_env_vars(env, j);
				break ;
			}
		}
		j++;
	}
}

void	builtin_unset(t_mini *mini)
{
	int	i;

	i = 0;
	while (ft_argc(mini->cmds->tokens) > ++i)
		unset_single_var(mini->cmds->tokens[i], mini->env);
	mini->exit_sts = 0;
}
