/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_redirs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 04:27:48 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/23 15:40:07 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_redirs(t_redir *r)
{
	t_redir	*n;

	while (r)
	{
		n = r->next;
		if (r->fd > 2)
			close(r->fd);
		if (r->hd_data)
			free_heredoc_data(r->hd_data);
		if (r->target)
			free(r->target);
		free(r);
		r = n;
	}
}
