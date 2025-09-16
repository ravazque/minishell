/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 01:37:51 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/16 01:42:33 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

static char	*ms_abbrev_home(char *cwd)
{
	char	*home;
	char	*res;

	home = getenv("HOME");
	if (!home)
		return (cwd);
	if (ft_strncmp(cwd, home, ft_strlen(home)) != 0)
		return (cwd);
	res = ft_strjoin("~", cwd + ft_strlen(home));
	free(cwd);
	return (res);
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

char	*ms_make_prompt(void)
{
	char	*cwd;
	char	*shown;
	char	*prompt;

	cwd = ms_getcwd_or_pwd();
	if (!cwd)
		return (NULL);
	shown = ms_abbrev_home(cwd);
	if (!shown)
		return (NULL);
	prompt = ms_build_prompt(shown);
	free(shown);
	return (prompt);
}
