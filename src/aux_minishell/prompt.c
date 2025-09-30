/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 19:20:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/30 17:14:14 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*basename_dup(const char *path)
{
	size_t	len;
	size_t	i;
	size_t	start;

	if (!path)
		return (ft_strdup("?"));
	len = 0;
	while (path[len])
		len++;
	while (len > 1 && path[len - 1] == '/')
		len--;
	start = 0;
	if (len > 0)
	{
		i = len;
		while (i > 0 && path[i - 1] != '/')
			i--;
		start = i;
	}
	if (len == 1 && path[0] == '/')
		return (ft_strdup("/"));
	return (ft_substr(path, start, len - start));
}

static char	*add_git_indicator(char *base)
{
	char	*with_git;
	char	*tmp;
	char	*tmp2;

	tmp = ft_strjoin(RL_YEL, " [git]");
	if (!tmp)
		return (base);
	tmp2 = ft_strjoin(tmp, RL_RST);
	free(tmp);
	if (!tmp2)
		return (base);
	with_git = ft_strjoin(base, tmp2);
	free(tmp2);
	if (!with_git)
		return (base);
	free(base);
	return (with_git);
}

static char	*rl_color_wrap(const char *s, const char *color)
{
	char	*tmp1;
	char	*tmp2;

	tmp1 = ft_strjoin(color, s);
	if (!tmp1)
		return (NULL);
	tmp2 = ft_strjoin(tmp1, RL_RST);
	free(tmp1);
	return (tmp2);
}

static char	*build_prompt(const char *shown, int has_git)
{
	char	*colored;
	char	*with_git;
	char	*prompt;

	colored = rl_color_wrap(shown, RL_CYN);
	if (!colored)
		return (NULL);
	if (has_git)
	{
		with_git = add_git_indicator(colored);
		prompt = ft_strjoin(with_git, " $ ");
		free(with_git);
	}
	else
		prompt = ft_strjoin(colored, " $ ");
	if (colored && !has_git)
		free(colored);
	return (prompt);
}

char	*prompt(t_mini *mini)
{
	char	*shown;
	char	*prompt;
	int		has_git;

	if (!mini)
		return (NULL);
	if (mini->pwd)
	{
		free(mini->pwd);
		mini->pwd = NULL;
	}
	mini->pwd = getcwd_or_pwd();
	if (!mini->pwd)
		return (NULL);
	shown = basename_dup(mini->pwd);
	if (!shown)
		return (NULL);
	has_git = is_git_repo(mini->pwd);
	prompt = build_prompt(shown, has_git);
	free(shown);
	return (prompt);
}

// static char	*basename_dup(const char *path)
// {
// 	if (!path)
// 		return (ft_strdup("?"));
// 	else
// 		return (ft_strdup(path));
// }
