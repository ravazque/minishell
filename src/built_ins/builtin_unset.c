/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 22:01:29 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/08 00:55:14 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	is_valid_unset_identifier(char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static char	**create_new_env_without_var(char **env, int skip_index)
{
	char	**new_env;
	int		i;
	int		j;
	int		count;

	count = ft_envlen(env);
	new_env = malloc(sizeof(char *) * count);
	if (!new_env)
		return (NULL);
	i = 0;
	j = 0;
	while (i < count)
	{
		if (i != skip_index)
		{
			new_env[j] = ft_strdup(env[i]);
			if (!new_env[j])
			{
				free_dblptr(new_env);
				return (NULL);
			}
			j++;
		}
		i++;
	}
	new_env[j] = NULL;
	return (new_env);
}

static void	unset_variable(char *name, char ***env)
{
	int		i;
	int		len;
	char	**new_env;

	len = ft_strlen(name);
	i = 0;
	while ((*env)[i])
	{
		if (!ft_strncmp(name, (*env)[i], len)
			&& ((*env)[i][len] == '\0' || (*env)[i][len] == '='))
		{
			new_env = create_new_env_without_var(*env, i);
			if (!new_env)
				return ;
			free_dblptr(*env);
			*env = new_env;
			return ;
		}
		i++;
	}
}

void	builtin_unset(t_mini *mini)
{
	int	i;
	int	has_error;

	if (!mini->cmds->tokens[1])
	{
		mini->exit_sts = 0;
		return ;
	}
	i = 1;
	has_error = 0;
	while (mini->cmds->tokens[i])
	{
		if (!is_valid_unset_identifier(mini->cmds->tokens[i]))
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(mini->cmds->tokens[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			has_error = 1;
		}
		else
			unset_variable(mini->cmds->tokens[i], &mini->env);
		i++;
	}
	if (has_error)
		mini->exit_sts = 1;
	else
		mini->exit_sts = 0;
}
