/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_build.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:50:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/30 17:50:00 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*get_user_host_str(t_mini mini)
{
	char	*user;
	char	*host;
	char	*user_host;
	char	*tmp;

	user = get_username(mini);
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
	return (user_host);
}

static char	*colorize_user_host(char *user_host)
{
	char	*tmp;
	char	*colored;

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

char	*build_user_host(t_mini mini)
{
	char	*user_host;

	user_host = get_user_host_str(mini);
	if (!user_host)
		return (NULL);
	return (colorize_user_host(user_host));
}
