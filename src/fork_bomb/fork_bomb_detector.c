/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_bomb_detector.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 04:57:17 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/30 17:06:50 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	fork_bomb_signal(t_mini *mini)
{
	cleanup_mini(mini);
	write(STDOUT_FILENO, "exit...", 8);
	ft_putstr_fd(RL_BLD, STDOUT_FILENO);
	ft_putstr_fd(RL_RED, STDOUT_FILENO);
	write(STDOUT_FILENO, " COWARD!", 8);
	ft_putstr_fd(RL_RST, STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	exit(42);
}

static int	match_pattern(const char *input, const char *pattern)
{
	int	i;
	int	j;

	i = 0;
	while (input[i])
	{
		j = 0;
		while (pattern[j] && input[i + j] == pattern[j])
			j++;
		if (pattern[j] == '\0')
			return (1);
		i++;
	}
	return (0);
}

static const char	*get_pattern(int idx)
{
	if (idx == 0)
		return (":()|:|:&};:");
	if (idx == 1)
		return (":()|:|&};:");
	if (idx == 2)
		return (":((){:|:&};:");
	if (idx == 3)
		return ("(){:|:&};");
	return (NULL);
}

static int	check_pattern(const char *input, int idx)
{
	const char	*pattern;

	pattern = get_pattern(idx);
	if (!pattern)
		return (0);
	return (match_pattern(input, pattern));
}

int	is_fork_bomb(const char *input)
{
	int	i;

	if (!input)
		return (0);
	i = 0;
	while (i < 4)
	{
		if (check_pattern(input, i))
			return (1);
		i++;
	}
	return (0);
}
