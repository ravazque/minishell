/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_path.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 19:30:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/22 19:32:06 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static char	*resolve_parent_dir(char *pwd)
{
	char	*parent;
	char	*last_slash;

	parent = ft_strdup(pwd);
	if (!parent)
		return (NULL);
	last_slash = ft_strrchr(parent, '/');
	if (!last_slash || last_slash == parent)
	{
		free(parent);
		return (ft_strdup("/"));
	}
	*last_slash = '\0';
	return (parent);
}

char	*get_parent_from_pwd(t_mini *mini)
{
	char	*pwd;

	pwd = get_local_env("PWD", mini->env);
	if (!pwd)
		return (NULL);
	return (resolve_parent_dir(pwd));
}

static char	*build_path_component(char *base, char *component)
{
	char	*temp;
	char	*result;
	size_t	len;

	if (!base || !component)
		return (NULL);
	len = ft_strlen(base);
	if (len == 0)
		return (ft_strdup(component));
	if (base[len - 1] == '/')
		return (ft_strjoin(base, component));
	temp = ft_strjoin(base, "/");
	if (!temp)
		return (NULL);
	result = ft_strjoin(temp, component);
	free(temp);
	return (result);
}

static char	*calculate_logical_pwd(char *current, char *path)
{
	if (!current)
		return (NULL);
	if (path[0] == '/')
		return (ft_strdup(path));
	if (!ft_strcmp(path, "."))
		return (ft_strdup(current));
	return (build_path_component(current, path));
}

char	*get_new_pwd(char *oldpwd, char *path, char *arg)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (pwd)
		return (pwd);
	if (!oldpwd)
		return (NULL);
	if (needs_free(arg))
		return (ft_strdup(path));
	if (arg)
		return (calculate_logical_pwd(oldpwd, arg));
	return (NULL);
}
