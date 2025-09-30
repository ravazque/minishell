/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:10:03 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/30 17:12:05 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

char	*getcwd_or_pwd(void)
{
	char	*cwd;
	char	*pwd;

	cwd = getcwd(NULL, 0);
	if (cwd)
		return (cwd);
	pwd = getenv("PWD");
	if (pwd)
		return (ft_strdup(pwd));
	return (ft_strdup("?"));
}
