/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 20:12:32 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/14 15:50:20 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	ft_lstsize(t_cmd *lst)
{
	int		count;
	t_cmd	*curr;

	count = 0;
	curr = lst;
	while (curr)
	{
		count++;
		curr = curr->next;
	}
	return (count);
}
