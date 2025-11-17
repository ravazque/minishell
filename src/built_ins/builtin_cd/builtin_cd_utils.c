/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 19:30:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/05 15:32:52 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

char	*get_home_cached(t_mini *mini)
{
	char	*home;

	home = get_var_value("HOME", mini);
	if (home && home[0] != '\0')
	{
		if (!mini->cd_home || ft_strcmp(home, mini->cd_home) != 0)
		{
			if (mini->cd_home)
				free(mini->cd_home);
			mini->cd_home = ft_strdup(home);
			if (!mini->cd_home)
			{
				malloc_error();
				return (NULL);
			}
		}
		return (mini->cd_home);
	}
	if (mini->cd_home)
		return (mini->cd_home);
	return (NULL);
}

char	*handle_home_dir(t_mini *mini)
{
	char	*home;

	home = get_var_value("HOME", mini);
	return (update_home_cache(mini, home));
}

char	*handle_oldpwd_dir(t_mini *mini)
{
	char	*oldpwd;

	oldpwd = get_local_env("OLDPWD", mini->env);
	if (!oldpwd)
	{
		ft_putstr_fd(ERR_OLDPWD, 2);
		mini->exit_sts = 1;
		return (NULL);
	}
	return (oldpwd);
}

char	*update_home_cache(t_mini *mini, char *current)
{
	if (current && current[0] != '\0')
	{
		if (!mini->cd_home || ft_strcmp(current, mini->cd_home) != 0)
		{
			if (mini->cd_home)
				free(mini->cd_home);
			mini->cd_home = ft_strdup(current);
			if (!mini->cd_home)
			{
				malloc_error();
				return (NULL);
			}
		}
		return (mini->cd_home);
	}
	if (mini->cd_home)
		return (mini->cd_home);
	ft_putstr_fd(ERR_HOME, 2);
	mini->exit_sts = 1;
	return (NULL);
}

void	print_chdir_error(char *path, char *arg)
{
	ft_putstr_fd("minishell: cd: ", 2);
	if (arg && !ft_strcmp(arg, "-"))
		ft_putstr_fd(path, 2);
	else if (arg && !ft_strcmp(arg, "~"))
		ft_putstr_fd(path, 2);
	else if (arg)
		ft_putstr_fd(arg, 2);
	else
		ft_putstr_fd("HOME", 2);
	if (errno == EACCES)
		ft_putstr_fd(": Permission denied\n", 2);
	else if (errno == ENOTDIR)
		ft_putstr_fd(": Not a directory\n", 2);
	else
		ft_putstr_fd(": No such file or directory\n", 2);
}
