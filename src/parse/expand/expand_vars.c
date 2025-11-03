/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 18:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/03 18:00:00 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static char	*get_env_val(const char *key, char **env)
{
	int	i;
	int	j;

	if (!key || !env)
		return (NULL);
	j = ft_strlen(key);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], key, j) == 0 && env[i][j] == '=')
			return (ft_strdup(env[i] + j + 1));
		i++;
	}
	return (NULL);
}

static char	*get_exit_sts(int exit_status)
{
	char	*sts_str;

	sts_str = ft_itoa(exit_status);
	if (!sts_str)
		return (NULL);
	return (sts_str);
}

static char	*expand_special_var(const char *var, t_mini *mini)
{
	if (ft_strcmp(var, "?") == 0)
		return (get_exit_sts(mini->exit_sts));
	if (ft_strcmp(var, "$$") == 0)
		return (ft_strdup("80085"));
	if (ft_strcmp(var, "0") == 0)
	{
		if (mini->argv && mini->argv[0])
			return (ft_strdup(mini->argv[0]));
		return (ft_strdup("minishell"));
	}
	return (NULL);
}

char	*expand_var(const char *var, t_mini *mini)
{
	char	*val;

	if (!var)
		return (NULL);
	val = expand_special_var(var, mini);
	if (val)
		return (val);
	val = get_env_val(var, mini->env);
	if (val)
		return (val);
	return (get_env_val(var, mini->local_vars));
}

int	is_valid_var_chr(char c)
{
	if (ft_isalnum(c))
		return (1);
	if (c == '_')
		return (1);
	return (0);
}
