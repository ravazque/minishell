/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_parts.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:51:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/30 17:51:00 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*build_path_section(const char *pwd, t_mini mini)
{
	char	*short_path;
	char	*colored;
	char	*tmp;

	short_path = get_short_path(pwd, mini);
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

char	*build_git_section(const char *branch)
{
	char	*git_part;
	char	*tmp;
	char	*colored;

	if (!branch)
		return (ft_strdup(""));
	tmp = ft_strjoin(" git:(", branch);
	if (!tmp)
		return (NULL);
	git_part = ft_strjoin(tmp, ")");
	free(tmp);
	if (!git_part)
		return (NULL);
	tmp = ft_strjoin(RL_RED, git_part);
	free(git_part);
	if (!tmp)
		return (NULL);
	colored = ft_strjoin(tmp, RL_RST);
	free(tmp);
	return (colored);
}

char	*join_all_parts(char *user_host, char *path, char *git)
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
