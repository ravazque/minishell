/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 19:20:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/03 00:19:37 by ravazque         ###   ########.fr       */
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

static int	can_access_path(const char *path)
{
	if (access(path, F_OK) != 0)
		return (0);
	if (access(path, R_OK) != 0)
		return (0);
	return (1);
}

static char	*get_git_branch(const char *repo_path)
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

static char	*get_username(void)
{
	char	*user;

	user = getenv("USER");
	if (!user)
		user = getenv("LOGNAME");
	if (!user)
		return (ft_strdup("user"));
	return (ft_strdup(user));
}

static char	*get_hostname(void)
{
	char	hostname[256];

	if (gethostname(hostname, sizeof(hostname)) == 0)
		return (ft_strdup(hostname));
	return (ft_strdup("localhost"));
}

static char	*get_short_path(const char *full_path)
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
				return (short_path);
			}
		}
	}
	return (ft_strdup(full_path));
}

static char	*build_user_host(void)
{
	char	*user;
	char	*host;
	char	*user_host;
	char	*tmp;
	char	*colored;

	user = get_username();
	host = get_hostname();
	if (!user || !host)
	{
		if (user)
			free(user);
		if (host)
			free(host);
		return (NULL);
	}
	tmp = ft_strjoin(user, "@");
	free(user);
	if (!tmp)
		return (free(host), NULL);
	user_host = ft_strjoin(tmp, host);
	free(tmp);
	free(host);
	if (!user_host)
		return (NULL);
	tmp = ft_strjoin(RL_GRN, "[");
	if (!tmp)
		return (free(user_host), NULL);
	colored = ft_strjoin(tmp, user_host);
	free(tmp);
	free(user_host);
	if (!colored)
		return (NULL);
	tmp = ft_strjoin(colored, "]");
	free(colored);
	if (!tmp)
		return (NULL);
	colored = ft_strjoin(tmp, RL_RST);
	free(tmp);
	return (colored);
}

static char	*build_path_section(const char *pwd)
{
	char	*short_path;
	char	*colored;
	char	*tmp;

	short_path = get_short_path(pwd);
	if (!short_path)
		return (NULL);
	tmp = ft_strjoin(RL_BLU, short_path);
	free(short_path);
	if (!tmp)
		return (NULL);
	colored = ft_strjoin(tmp, RL_RST);
	free(tmp);
	return (colored);
}

static char	*build_git_section(const char *branch)
{
	char	*git_part;
	char	*tmp;
	char	*colored;

	if (!branch)
		return (ft_strdup(""));
	tmp = ft_strjoin(" git:(", branch);
	if (!tmp)
		return (ft_strdup(""));
	git_part = ft_strjoin(tmp, ")");
	free(tmp);
	if (!git_part)
		return (ft_strdup(""));
	tmp = ft_strjoin(RL_RED, git_part);
	free(git_part);
	if (!tmp)
		return (ft_strdup(""));
	colored = ft_strjoin(tmp, RL_RST);
	free(tmp);
	if (!colored)
		return (ft_strdup(""));
	return (colored);
}

static char	*join_all_parts(char *user_host, char *path, char *git)
{
	char	*tmp1;
	char	*tmp2;
	char	*result;

	tmp1 = ft_strjoin(user_host, " ");
	if (!tmp1)
		return (NULL);
	tmp2 = ft_strjoin(tmp1, path);
	free(tmp1);
	if (!tmp2)
		return (NULL);
	tmp1 = ft_strjoin(tmp2, git);
	free(tmp2);
	if (!tmp1)
		return (NULL);
	result = ft_strjoin(tmp1, " $ ");
	free(tmp1);
	return (result);
}

char	*prompt(t_mini *mini)
{
	char	*user_host;
	char	*path_section;
	char	*git_section;
	char	*branch;
	char	*final_prompt;

	if (!mini)
		return (NULL);
	if (mini->pwd)
		free(mini->pwd);
	mini->pwd = getcwd_or_pwd();
	if (!mini->pwd)
		return (NULL);
	user_host = build_user_host();
	path_section = build_path_section(mini->pwd);
	branch = NULL;
	if (is_git_repo(mini->pwd))
		branch = get_git_branch(mini->pwd);
	git_section = build_git_section(branch);
	if (branch)
		free(branch);
	if (!user_host || !path_section || !git_section)
	{
		if (user_host)
			free(user_host);
		if (path_section)
			free(path_section);
		if (git_section)
			free(git_section);
		return (ft_strdup("$ "));
	}
	final_prompt = join_all_parts(user_host, path_section, git_section);
	free(user_host);
	free(path_section);
	free(git_section);
	if (!final_prompt)
		return (ft_strdup("$ "));
	return (final_prompt);
}
