/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 17:10:07 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/16 17:34:57 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

bool	built_ins(t_mini mini, bool *ex)
{
	if (!mini.cmds || !mini.cmds->args || !mini.cmds->args[0])
		return (false);
	if (ft_strcmp(mini.cmds->args[0], "exit") == 0)
		return (*ex = true, true);
	else if (ft_strcmp(mini.cmds->args[0], "echo") == 0)
		return (printf("Paula's built-in\n"), *ex = false, true);
	else if (ft_strcmp(mini.cmds->args[0], "cd") == 0)
		return (printf("Paula's built-in\n"), *ex = false, true);
	else if (ft_strcmp(mini.cmds->args[0], "pwd") == 0)
		return (printf("Paula's built-in\n"), *ex = false, true);
	else if (ft_strcmp(mini.cmds->args[0], "export") == 0)
		return (printf("Paula's built-in\n"), *ex = false, true);
	else if (ft_strcmp(mini.cmds->args[0], "unset") == 0)
		return (printf("Paula's built-in\n"), *ex = false, true);
	else if (ft_strcmp(mini.cmds->args[0], "env") == 0)
		return (printf("Paula's built-in\n"), *ex = false, true);
	else
		return (false);
}
