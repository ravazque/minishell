/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 11:49:58 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/18 17:11:58 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	ft_n_flag(char **argv, int i)
{
	int	j;

	j = 0;
	while (argv[i] && argv[i][0] == '-' && argv[i][1] == 'n')
	{
		j = 2;
		while (argv[i][j] == 'n')
			j++;
		if (argv[i][j] == '\0')
			return (1);
		else
			return (0);
	}
	return (0);
}

void	builtin_echo(t_mini mini)
{
	int	i;
	int	flag;

	i = 1;
	flag = 0;
	while (mini.cmds->args[i] && ft_n_flag(mini.cmds->args, i))
	{
		flag = 1;
		i++;
	}
	while (mini.cmds->args[i])
	{
		ft_putstr_fd(mini.cmds->args[i], STDOUT_FILENO);
		if (mini.cmds->args[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (!flag)
		write(STDOUT_FILENO, "\n", 1);
}
