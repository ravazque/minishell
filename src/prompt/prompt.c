/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 19:20:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/03 05:01:06 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
