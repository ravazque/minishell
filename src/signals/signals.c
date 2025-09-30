/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 01:53:41 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/30 16:13:17 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

volatile sig_atomic_t	g_signal_received = 0;

static void	handle_sigint(int sig)
{
	(void)sig;
	g_signal_received = SIGINT;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	setup_interactive_signals(void)
{
	struct sigaction	sa_int;

	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_handler = handle_sigint;
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	ft_signal(t_mini *mini)
{
	if (!mini->input)
	{
		cleanup_mini(mini);
		write(STDOUT_FILENO, "exit\n", 5);
		exit(0);
	}
	if (g_signal_received == SIGINT)
	{
		mini->exit_sts = 130;
		g_signal_received = 0;
	}
}
