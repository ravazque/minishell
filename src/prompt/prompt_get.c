/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_get.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 05:00:39 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/21 18:14:29 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_username(t_mini mini)
{
	char	*user;

	user = get_local_env("USER", mini.env);
	if (!user)
		user = get_local_env("LOGNAME", mini.env);
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

static char	*try_home_replacement(const char *full_path, char *home)
{
	size_t	home_len;
	char	*short_path;

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
	return (NULL);
}

char	*get_short_path(const char *full_path, t_mini mini)
{
	char	*home;
	char	*result;

	if (!full_path)
		return (ft_strdup("~"));
	home = get_local_env("HOME", mini.env);
	if (home)
	{
		result = try_home_replacement(full_path, home);
		if (result)
			return (result);
	}
	return (ft_strdup(full_path));
}
