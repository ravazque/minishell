/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 19:20:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/19 06:27:25 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*ms_getcwd_or_pwd(void)
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

static char	*ms_basename_dup(const char *path)
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
	return (ms_substr(path, start, len - start));
}

static char	*ms_rl_color_wrap(const char *s, const char *color)
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

static char	*ms_build_prompt(const char *shown)
{
	char	*colored;
	char	*prompt;

	colored = ms_rl_color_wrap(shown, RL_GRN);
	if (!colored)
		return (NULL);
	prompt = ft_strjoin(colored, " $ ");
	free(colored);
	return (prompt);
}

char	*build_prompt(t_mini *mini)
{
	char	*shown;
	char	*prompt;

	if (!mini)
		return (NULL);
	if (mini->pwd)
	{
		free(mini->pwd);
		mini->pwd = NULL;
	}
	mini->pwd = ms_getcwd_or_pwd();
	if (!mini->pwd)
		return (NULL);
	shown = ms_basename_dup(mini->pwd);
	if (!shown)
		return (NULL);
	prompt = ms_build_prompt(shown);
	free(shown);
	return (prompt);
}
