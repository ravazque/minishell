/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_set.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:42:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/30 17:42:00 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	export_local_to_env(char *name, t_export_ctx *ctx)
{
	char	*local_val;

	local_val = get_local_env(name, *ctx->local_vars);
	if (local_val)
		ft_setenv(name, local_val, ctx->env);
	else if (!var_exists_with_value(name, *ctx->env))
		ft_setenv(name, NULL, ctx->env);
}

static void	handle_with_equal(char *arg, char ***env)
{
	char	**args;

	args = ft_split2(arg, '=');
	if (!args)
		return (malloc_error());
	if (args[1])
		ft_setenv(args[0], args[1], env);
	else if (!args[1])
		ft_setenv(args[0], "", env);
	free_dblptr(args);
}

void	ft_setexport(char *arg, t_export_ctx *ctx, int flag)
{
	if (flag == 0)
	{
		if (ft_strchr(arg, '='))
			handle_with_equal(arg, ctx->env);
		else if (!ft_strchr(arg, '='))
			export_local_to_env(arg, ctx);
	}
}
