/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_get.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 05:00:39 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/03 05:02:04 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_git_branch(const char *repo_path)
{
	char	*git_head_path;
	char	*branch;
	char	line[256];
	FILE	*f;
	char	*last_slash;

	git_head_path = ft_strjoin(repo_path, "/.git/HEAD");
	if (!git_head_path)
		return (NULL);
	if (!can_access_path(git_head_path))
		return (free(git_head_path), NULL);
	f = fopen(git_head_path, "r");
	free(git_head_path);
	if (!f)
		return (NULL);
	if (!fgets(line, sizeof(line), f))
		return (fclose(f), NULL);
	fclose(f);
	line[ft_strcspn(line, "\n")] = '\0';
	branch = NULL;
	if (ft_strncmp(line, "ref: ", 5) == 0)
	{
		last_slash = ft_strrchr(line + 5, '/');
		if (last_slash)
			branch = ft_strdup(last_slash + 1);
		else
			branch = ft_strdup(line + 5);
	}
	else
	{
		if (ft_strlen(line) > 7)
			branch = ft_substr(line, 0, 7);
		else
			branch = ft_strdup(line);
	}
	return (branch);
}

char	*get_username(void)
{
	char	*user;

	user = getenv("USER");
	if (!user)
		user = getenv("LOGNAME");
	if (!user)
		return (ft_strdup("user"));
	return (ft_strdup(user));
}

char	*get_hostname(void)
{
	char	hostname[256];

	if (gethostname(hostname, sizeof(hostname)) == 0)
		return (ft_strdup(hostname));
	return (ft_strdup("localhost"));
}

char	*get_short_path(const char *full_path)
{
	char	*home;
	char	*short_path;
	size_t	home_len;

	if (!full_path)
		return (ft_strdup("~"));
	home = getenv("HOME");
	if (home)
	{
		home_len = ft_strlen(home);
		if (ft_strncmp(full_path, home, home_len) == 0)
		{
			if (full_path[home_len] == '\0')
				return (ft_strdup("~"));
			if (full_path[home_len] == '/')
			{
				short_path = ft_strjoin("~", full_path + home_len);
				if (!short_path)
					return (ft_strdup(full_path));
				return (short_path);
			}
		}
	}
	return (ft_strdup(full_path));
}
