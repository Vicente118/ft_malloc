/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarras <vdarras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 13:16:13 by vdarras           #+#    #+#             */
/*   Updated: 2024/04/12 16:41:58 by vdarras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new;
	t_list	*node;
	void	*old;

	if (!lst || !f || !del)
		return (NULL);
	new = NULL;
	while (lst)
	{
		old = (f(lst->content));
		node = ft_lstnew(old);
		if (!node)
		{
			del(old);
			ft_lstclear(&new, del);
			return (NULL);
		}
		ft_lstadd_back((&new), node);
		lst = lst->next;
	}
	return (new);
}
