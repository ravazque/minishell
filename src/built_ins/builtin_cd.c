/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 04:43:21 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/29 19:47:35 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*cd_getenv(char **env, const char *key)
{
	size_t	klen;
	int		i;

	if (!env || !key)
		return (NULL);
	klen = ft_strlen(key);
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], key, klen) && env[i][klen] == '=')
			return (env[i] + klen + 1);
		i++;
	}
	return (NULL);
}

void	builtin_cd(t_mini *mini)
{
	char	*target;
	char	*oldpwd;
	char	*pwd;

	if (!mini->cmds->tokens[1])
	{
		target = cd_getenv(mini->env, "HOME");
		if (!target || !*target)
		{
			ft_putstr_fd("minishell: cd: path not provided\n", 2);
			return ;
		}
	}
	else if (mini->cmds->tokens[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		mini->exit_sts = 2;
		return ;
	}
	oldpwd = getcwd(NULL, 0);
	if (chdir(mini->cmds->tokens[1]) < 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(mini->cmds->tokens[1], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		mini->exit_sts = 1;
		return ;
	}
	pwd = getcwd(NULL, 0);
	free(oldpwd);
	free(pwd);
	mini->exit_sts = 0;
}

// ===[ "cd -" "cd" + errores con el propio "cd -" ]===

// static char	*cd_getenv(char **env, const char *key)
// {
// 	size_t	klen;
// 	int		i;

// 	if (!env || !key)
// 		return (NULL);
// 	klen = ft_strlen(key);
// 	i = 0;
// 	while (env[i])
// 	{
// 		if (!ft_strncmp(env[i], key, klen) && env[i][klen] == '=')
// 			return (env[i] + klen + 1);
// 		i++;
// 	}
// 	return (NULL);
// }

// static int	count_env_vars(char **env)
// {
// 	int	count;

// 	count = 0;
// 	while (env[count])
// 		count++;
// 	return (count);
// }

// static char	**realloc_env(char **env, int new_size)
// {
// 	char	**new_env;
// 	int		i;

// 	new_env = malloc(sizeof(char *) * (new_size + 1));
// 	if (!new_env)
// 		return (NULL);
// 	i = 0;
// 	while (env[i])
// 	{
// 		new_env[i] = env[i];
// 		i++;
// 	}
// 	new_env[i] = NULL;
// 	free(env);
// 	return (new_env);
// }

// static void	set_env_var(char ***env, const char *key, const char *value)
// {
// 	char	*new_var;
// 	char	*temp;
// 	int		i;
// 	size_t	klen;
// 	int		count;

// 	if (!env || !*env || !key || !value)
// 		return ;
// 	klen = ft_strlen(key);
// 	temp = ft_strjoin(key, "=");
// 	if (!temp)
// 		return ;
// 	new_var = ft_strjoin(temp, value);
// 	free(temp);
// 	if (!new_var)
// 		return ;
// 	i = 0;
// 	while ((*env)[i])
// 	{
// 		if (!ft_strncmp((*env)[i], key, klen) && (*env)[i][klen] == '=')
// 		{
// 			free((*env)[i]);
// 			(*env)[i] = new_var;
// 			return ;
// 		}
// 		i++;
// 	}
// 	count = count_env_vars(*env);
// 	*env = realloc_env(*env, count + 1);
// 	if (!*env)
// 	{
// 		free(new_var);
// 		return ;
// 	}
// 	(*env)[count] = new_var;
// 	(*env)[count + 1] = NULL;
// }

// static int	handle_cd_minus(t_mini *mini, char **target)
// {
// 	char	*oldpwd_env;

// 	oldpwd_env = cd_getenv(mini->env, "OLDPWD");
// 	if (!oldpwd_env || !*oldpwd_env)
// 	{
// 		ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
// 		mini->exit_sts = 1;
// 		return (1);
// 	}
// 	*target = oldpwd_env;
// 	ft_putstr_fd(*target, 1);
// 	ft_putstr_fd("\n", 1);
// 	return (0);
// }

// static int	handle_cd_home(t_mini *mini, char **target)
// {
// 	*target = cd_getenv(mini->env, "HOME");
// 	if (!*target || !**target)
// 	{
// 		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
// 		mini->exit_sts = 1;
// 		return (1);
// 	}
// 	return (0);
// }

// static int	determine_target(t_mini *mini, char **target)
// {
// 	if (mini->cmds->tokens[1] && mini->cmds->tokens[2])
// 	{
// 		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
// 		mini->exit_sts = 1;
// 		return (1);
// 	}
// 	if (!mini->cmds->tokens[1])
// 		return (handle_cd_home(mini, target));
// 	if (mini->cmds->tokens[1][0] == '-' && !mini->cmds->tokens[1][1])
// 		return (handle_cd_minus(mini, target));
// 	*target = mini->cmds->tokens[1];
// 	return (0);
// }

// void	builtin_cd(t_mini *mini)
// {
// 	char	*target;
// 	char	*oldpwd;
// 	char	*pwd;

// 	if (determine_target(mini, &target))
// 		return ;
// 	oldpwd = getcwd(NULL, 0);
// 	if (chdir(target) < 0)
// 	{
// 		ft_putstr_fd("minishell: cd: ", 2);
// 		ft_putstr_fd(target, 2);
// 		ft_putstr_fd(": No such file or directory\n", 2);
// 		mini->exit_sts = 1;
// 		free(oldpwd);
// 		return ;
// 	}
// 	pwd = getcwd(NULL, 0);
// 	if (oldpwd)
// 		set_env_var(&(mini->env), "OLDPWD", oldpwd);
// 	if (pwd)
// 		set_env_var(&(mini->env), "PWD", pwd);
// 	free(oldpwd);
// 	free(pwd);
// 	mini->exit_sts = 0;
// }

// void	unset_oldpwd(char **env)
// {
// 	size_t	klen;
// 	int		i;
// 	int		j;

// 	if (!env)
// 		return ;
// 	klen = ft_strlen("OLDPWD");
// 	i = 0;
// 	while (env[i])
// 	{
// 		if (!ft_strncmp(env[i], "OLDPWD", klen) && env[i][klen] == '=')
// 		{
// 			free(env[i]);
// 			j = i;
// 			while (env[j])
// 			{
// 				env[j] = env[j + 1];
// 				j++;
// 			}
// 			return ;
// 		}
// 		i++;
// 	}
// }
