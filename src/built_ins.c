/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 17:10:07 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/15 17:10:55 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

bool	built_ins(t_mini mini)
{
	char	**built_in;

	built_in = ft_split(mini.input, ' ');
	if (ft_strcmp_ns(built_in[0], "exit") == 0)
		return (true);
	else if (ft_strcmp_ns(built_in[0], "exit") == 0)
		return (true);
	else if (ft_strcmp_ns(built_in[0], "echo") == 0)
		return (true);
	else if (ft_strcmp_ns(built_in[0], "cd") == 0)
		return (true);
	else if (ft_strcmp_ns(built_in[0], "pwd") == 0)
		return (true);
	else if (ft_strcmp_ns(built_in[0], "export") == 0)
		return (true);
	else if (ft_strcmp_ns(built_in[0], "unset") == 0)
		return (true);
	else if (ft_strcmp_ns(built_in[0], "env") == 0)
		return (true);
	else
		return (false);
}
