/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 19:20:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/30 18:02:21 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	free_prompt_parts(char *user_host, char *path, char *git)
{
	if (user_host)
		free(user_host);
	if (path)
		free(path);
	if (git)
		free(git);
}

static char	*get_branch_if_git(char *pwd)
{
	char	*branch;

	branch = NULL;
	if (is_git_repo(pwd))
		branch = get_git_branch(pwd);
	return (branch);
}

static char	*build_prompt_parts(t_mini *mini, char **user_host, char **path)
{
	char	*branch;
	char	*git_section;

	*user_host = build_user_host(*mini);
	*path = build_path_section(mini->pwd, *mini);
	branch = get_branch_if_git(mini->pwd);
	git_section = build_git_section(branch);
	if (branch)
		free(branch);
	return (git_section);
}

char	*prompt(t_mini *mini)
{
	char	*user_host;
	char	*path_section;
	char	*git_section;
	char	*final_prompt;

	if (!mini)
		return (NULL);
	if (mini->pwd)
		free(mini->pwd);
	mini->pwd = getcwd_or_pwd(*mini);
	if (!mini->pwd)
		return (ft_strdup("$ "));
	git_section = build_prompt_parts(mini, &user_host, &path_section);
	if (!user_host || !path_section || !git_section)
		return (free_prompt_parts(user_host, path_section, git_section),
			ft_strdup("$ "));
	final_prompt = join_all_parts(user_host, path_section, git_section);
	free_prompt_parts(user_host, path_section, git_section);
	if (!final_prompt)
		return (ft_strdup("$ "));
	// return (final_prompt);
	return (ft_strdup("mininshell $"));
}
