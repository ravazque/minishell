/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_bomb.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 00:30:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/06 13:04:18 by ravazque         ###   ########.fr       */
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

static int	validate_step_input(const char *input, int step)
{
	if (step == 0)
		return (check_apology(input));
	if (step == 1)
		return (check_confirmation(input));
	return (0);
}

static void	apology_loop(t_mini *mini)
{
	char	*input;
	int		step;

	step = 0;
	while (step < 2)
	{
		print_step_message(step);
		input = readline("> ");
		if (!input)
			fork_bomb_signal(mini);
		if (input && *input)
			add_history(input);
		if (validate_step_input(input, step))
			step++;
		else
			print_wrong_answer();
		free(input);
	}
	correct_message();
}

void	fork_bomb(t_mini *mini)
{
	ft_putstr_fd("\n", STDOUT_FILENO);
	ft_putstr_fd(RL_FRKBMB, STDOUT_FILENO);
	ft_putstr_fd(ERR_FORKBOMB, STDOUT_FILENO);
	ft_putstr_fd(RL_RST, STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	signal(SIGINT, SIG_IGN);
	apology_loop(mini);
	setup_interactive_signals();
	mini->exit_sts = 1;
}
