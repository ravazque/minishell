/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:10:03 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/21 17:30:38 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

size_t	ft_strcspn(const char *s, const char *reject)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (s[i])
	{
		j = 0;
		while (reject[j])
		{
			if (s[i] == reject[j])
				return (i);
			j++;
		}
		i++;
	}
	return (i);
}

int	is_git_repo(const char *path)
{
	char		*git_path;
	struct stat	st;
	int			result;

	if (!path)
		return (0);
	git_path = ft_strjoin(path, "/.git");
	if (!git_path)
		return (0);
	result = 0;
	if (stat(git_path, &st) == 0)
	{
		if (S_ISDIR(st.st_mode))
			result = 1;
	}
	free(git_path);
	return (result);
}

static int	pwd_matches_cwd(const char *pwd, const char *cwd)
{
	struct stat	pwd_stat;
	struct stat	cwd_stat;

	if (!pwd || !cwd)
		return (0);
	if (stat(pwd, &pwd_stat) != 0)
		return (0);
	if (stat(cwd, &cwd_stat) != 0)
		return (0);
	return (pwd_stat.st_ino == cwd_stat.st_ino
		&& pwd_stat.st_dev == cwd_stat.st_dev);
}

char	*getcwd_or_pwd(t_mini mini)
{
	char	*cwd;
	char	*pwd;

	pwd = get_local_env("PWD", mini.env);
	cwd = getcwd(NULL, 0);
	if (pwd && cwd && pwd_matches_cwd(pwd, cwd))
	{
		free(cwd);
		return (ft_strdup(pwd));
	}
	if (pwd && !cwd)
		return (ft_strdup(pwd));
	if (cwd)
		return (cwd);
	if (pwd)
		return (ft_strdup(pwd));
	return (ft_strdup("?"));
}

int	can_access_path(const char *path)
{
	if (access(path, F_OK) != 0)
		return (0);
	if (access(path, R_OK) != 0)
		return (0);
	return (1);
}
