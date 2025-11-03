/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_git.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 18:30:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/30 18:30:00 by ravazque         ###   ########.fr       */
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

static char	*parse_git_ref(char *line)
{
	char	*last_slash;
	char	*branch;

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

char	*get_git_branch(const char *repo_path)
{
	char	*git_head_path;
	char	*branch;
	char	*line;

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
	branch = parse_git_ref(line);
	free(line);
	return (branch);
}
