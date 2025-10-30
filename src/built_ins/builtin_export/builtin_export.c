/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 01:30:15 by ptrapero          #+#    #+#             */
/*   Updated: 2025/10/23 17:31:09 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	process_single_export(t_mini *mini, int i, int *has_error)
{
	t_export_ctx	ctx;
	int				flag;

	ctx.env = &mini->env;
	ctx.local_vars = &mini->local_vars;
	flag = validate_export_arg(mini->cmds->tokens[i]);
	if (flag)
		*has_error = 1;
	ft_setexport(mini->cmds->tokens[i], &ctx, flag);
}

void	builtin_export(t_mini *mini)
{
	int	i;
	int	has_error;

	if (ft_argc(mini->cmds->tokens) == 1)
	{
		ft_putexport(&mini->env);
		mini->exit_sts = 0;
		return ;
	}
	i = 1;
	has_error = 0;
	while (mini->cmds->tokens[i])
	{
		process_single_export(mini, i, &has_error);
		i++;
	}
	if (has_error)
		mini->exit_sts = 1;
	else
		mini->exit_sts = 0;
}
