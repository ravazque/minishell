/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mshlvl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 17:00:44 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/30 17:15:44 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	calculate_level(char *level_str)
{
	int	level;

	level = ft_atoi(level_str);
	if (level < 0)
		return (0);
	if (level == INT_MAX - 1)
	{
		ft_putstr_fd("minishell: warning: shell level (", STDERR_FILENO);
		ft_putnbr_fd(INT_MAX, STDERR_FILENO);
		ft_putstr_fd(") too high, resetting to 1\n", STDERR_FILENO);
		return (1);
	}
	if (level == INT_MAX)
		return (0);
	return (level + 1);
}

static int	get_mshlvl_value(char **env)
{
	char	*mshlvl_str;

	mshlvl_str = get_local_env("MSHLVL", env);
	if (mshlvl_str)
		return (calculate_level(mshlvl_str));
	return (1);
}

void	setup_mshlvl(t_mini *mini)
{
	int		level;
	char	*level_str;

	level = get_mshlvl_value(mini->env);
	level_str = ft_itoa(level);
	if (!level_str)
	{
		ft_putstr_fd(INIT_ERR_MSHLVL, STDERR_FILENO);
		cleanup_mini(mini);
		exit(1);
	}
	ft_setenv("MSHLVL", level_str, &(mini->env));
	free(level_str);
}
