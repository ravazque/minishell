/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 20:12:30 by ravazque          #+#    #+#             */
/*   Updated: 2025/06/24 18:45:36 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*nwlst;
	t_list	*nwnde;
	void	*nwcntn;

	nwlst = NULL;
	while (lst != NULL)
	{
		nwcntn = f(lst->content);
		if (nwcntn == NULL)
		{
			ft_lstclear(&nwlst, del);
			return (NULL);
		}
		nwnde = ft_lstnew(nwcntn);
		if (nwnde == NULL)
		{
			del(nwcntn);
			ft_lstclear(&nwlst, del);
			return (NULL);
		}
		ft_lstadd_back(&nwlst, nwnde);
		lst = lst->next;
	}
	return (nwlst);
}
