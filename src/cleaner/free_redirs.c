/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_redirs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 04:27:48 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/19 04:29:01 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_redirs(t_redir *r)
{
	t_redir	*n;

	while (r)
	{
		n = r->next;
		if (r->target)
			free(r->target);
		free(r);
		r = n;
	}
}
