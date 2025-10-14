/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_get.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 05:00:39 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/14 20:43:30 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*read_git_head(const char *git_head_path)
{
	int		fd;
	char	buffer[256];
	int		bytes_read;
	char	*result;

	fd = open(git_head_path, O_RDONLY);
	if (fd < 0)
		return (NULL);
	bytes_read = read(fd, buffer, sizeof(buffer) - 1);
	close(fd);
	if (bytes_read <= 0)
		return (NULL);
	buffer[bytes_read] = '\0';
	result = ft_strdup(buffer);
	return (result);
}

char	*get_git_branch(const char *repo_path)
{
	char	*git_head_path;
	char	*branch;
	char	*line;
	char	*last_slash;

	git_head_path = ft_strjoin(repo_path, "/.git/HEAD");
	if (!git_head_path)
		return (NULL);
	if (!can_access_path(git_head_path))
		return (free(git_head_path), NULL);
	line = read_git_head(git_head_path);
	free(git_head_path);
	if (!line)
		return (NULL);
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
	free(line);
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
	int		fd;
	char	buffer[256];
	int		bytes_read;
	int		i;

	fd = open("/etc/hostname", O_RDONLY);
	if (fd < 0)
		return (ft_strdup("localhost"));
	bytes_read = read(fd, buffer, sizeof(buffer) - 1);
	close(fd);
	if (bytes_read <= 0)
		return (ft_strdup("localhost"));
	buffer[bytes_read] = '\0';
	i = 0;
	while (buffer[i] && buffer[i] != '\n' && buffer[i] != '.')
		i++;
	buffer[i] = '\0';
	return (ft_strdup(buffer));
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
