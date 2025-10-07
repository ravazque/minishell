/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_bomb_handler.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 00:30:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/07 16:03:42 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	check_apology(const char *input)
{
	if (!input)
		return (0);
	if (ft_strcmp(input, "Sorry!") == 0)
		return (1);
	return (0);
}

static int	check_confirmation(const char *input)
{
	if (!input)
		return (0);
	if (ft_strcmp(input, "I'm really sorry!") == 0)
		return (1);
	return (0);
}

static void	apology_loop(t_mini *mini)
{
	char	*input;
	int		step;

	step = 0;
	while (step < 2)
	{
		if (step == 0)
		{
			ft_putstr_fd(RL_RED, STDOUT_FILENO);
			ft_putstr_fd(MSG_APOLOGY, STDOUT_FILENO);
			ft_putstr_fd(RL_RST, STDOUT_FILENO);
		}
		else
		{
			ft_putstr_fd(RL_YEL, STDOUT_FILENO);
			ft_putstr_fd(MSG_CONFIRM, STDOUT_FILENO);
			ft_putstr_fd(RL_RST, STDOUT_FILENO);
		}
		input = readline("> ");
		if (!input)
		{
			cleanup_mini(mini);
			write(STDOUT_FILENO, "exit...", 8);
			ft_putstr_fd(RL_BLD, STDOUT_FILENO);
			ft_putstr_fd(RL_RED, STDOUT_FILENO);
			write(STDOUT_FILENO, " COWARD!", 8);
			ft_putstr_fd(RL_RST, STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
			exit(0);
		}
		if (input && *input)
			add_history(input);
		if (step == 0 && check_apology(input))
			step++;
		else if (step == 1 && check_confirmation(input))
			step++;
		else
		{
			ft_putstr_fd(RL_RED, STDOUT_FILENO);
			ft_putstr_fd(MSG_WRONG, STDOUT_FILENO);
			ft_putstr_fd(RL_RST, STDOUT_FILENO);
		}
		free(input);
	}
	ft_putstr_fd(BOLD_GREEN, STDOUT_FILENO);
	ft_putstr_fd(MSG_FORGIVEN, STDOUT_FILENO);
	ft_putstr_fd(RL_RST, STDOUT_FILENO);
}

void	handle_fork_bomb(t_mini *mini)
{
	ft_putstr_fd("\n", STDOUT_FILENO);
	ft_putstr_fd(RL_BLD, STDOUT_FILENO);
	ft_putstr_fd(RL_RED, STDOUT_FILENO);
	ft_putstr_fd(ERR_FORKBOMB, STDOUT_FILENO);
	ft_putstr_fd(RL_RST, STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	apology_loop(mini);
	mini->exit_sts = 1;
}
