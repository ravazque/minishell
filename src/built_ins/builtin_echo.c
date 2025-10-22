/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ptrapero <ptrapero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 11:49:58 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/22 20:58:16 by ptrapero         ###   ########.fr       */
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

void	builtin_echo(t_cmd *cmd)
{
	int	i;
	int	flag;

	i = 1;
	flag = 0;
	while (cmd->tokens[i] && ft_n_flag(cmd->tokens, i))
	{
		flag = 1;
		i++;
	}
	while (cmd->tokens[i])
	{
		ft_putstr_fd(cmd->tokens[i], STDOUT_FILENO);
		if (cmd->tokens[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (!flag)
		write(STDOUT_FILENO, "\n", 1);
}
