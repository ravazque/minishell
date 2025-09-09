/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 20:10:17 by ravazque          #+#    #+#             */
/*   Updated: 2025/06/24 18:45:41 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*nwnde;

	nwnde = (t_list *)malloc(sizeof(t_list));
	if (!nwnde)
	{
		return (NULL);
	}
	nwnde->content = content;
	nwnde->next = NULL;
	return (nwnde);
}
