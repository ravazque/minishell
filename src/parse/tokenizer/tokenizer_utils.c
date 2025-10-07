/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 00:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/08 00:54:13 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

t_token_part	*create_token_part_global(const char *content, int is_sq, int is_dq)
{
	t_token_part	*part;

	if (!content)
		content = "";
	part = (t_token_part *)malloc(sizeof(t_token_part));
	if (!part)
		return (NULL);
	part->content = ft_strdup(content);
	if (!part->content)
	{
		free(part);
		return (NULL);
	}
	part->is_squote = is_sq;
	part->is_dquote = is_dq;
	part->next = NULL;
	return (part);
}
