/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 04:43:21 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/14 19:58:23 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	check_multiple_args(t_mini *mini)
{
	if (mini->cmds->tokens[1] && mini->cmds->tokens[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		mini->exit_sts = 2;
		return (1);
	}
	return (0);
}

static char	*get_target_path(t_mini *mini, char *arg)
{
	char	*path;

	if (!arg)
		return (handle_home_dir(mini));
	if (!ft_strcmp(arg, "-"))
		return (handle_oldpwd_dir(mini));
	if (!ft_strcmp(arg, "~"))
	{
		path = get_home_cached(mini);
		if (!path)
		{
			ft_putstr_fd(ERR_HOME, 2);
			mini->exit_sts = 1;
			return (NULL);
		}
		return (path);
	}
	if (!ft_strcmp(arg, ".."))
		return (get_parent_from_pwd(mini));
	return (arg);
}

static int	change_directory(t_mini *mini, char *path, char *arg)
{
	if (chdir(path) < 0)
	{
		print_chdir_error(path, arg);
		mini->exit_sts = 1;
		if (needs_free(arg))
			free(path);
		return (0);
	}
	if (arg && !ft_strcmp(arg, "-"))
	{
		ft_putstr_fd(path, STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
	}
	return (1);
}

static void	update_environment(t_mini *mini, char *oldpwd, char *pwd)
{
	if (oldpwd)
		ft_setenv("OLDPWD", oldpwd, &(mini->env));
	if (pwd)
	{
		ft_setenv("PWD", pwd, &(mini->env));
		free(pwd);
	}
}

void	builtin_cd(t_mini *mini)
{
	char	*path;
	char	*arg;
	char	*oldpwd;
	char	*pwd;

	if (check_multiple_args(mini))
		return ;
	arg = mini->cmds->tokens[1];
	path = get_target_path(mini, arg);
	if (!path)
		return ;
	oldpwd = get_local_env("PWD", mini->env);
	if (!change_directory(mini, path, arg))
		return ;
	pwd = get_new_pwd(oldpwd, path, arg);
	if (needs_free(arg))
		free(path);
	update_environment(mini, oldpwd, pwd);
	mini->exit_sts = 0;
}

// If you enter the program without an environment using `env -i`, for `cd` to
// work, you need to first use `cd .` to set the current directory, you need
// you need to declare `$PWD` in the environment.

// Be careful when creating relative paths in a variable and passing it as a
// parameter, for example, `..` will redirect you to the `\` directory.
