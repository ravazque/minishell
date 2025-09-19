/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_tokens.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 04:26:43 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/19 04:27:16 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_tokens(t_token *tk)
{
	t_token	*nx;

	while (tk)
	{
		nx = tk->next;
		if (tk->raw)
			free(tk->raw);
		free(tk);
		tk = nx;
	}
}
