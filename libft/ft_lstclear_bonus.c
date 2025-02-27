/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarras <vdarras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 14:24:09 by vdarras           #+#    #+#             */
/*   Updated: 2024/04/11 17:52:35 by vdarras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*element;
	t_list	*tmp;

	if (!lst || !del)
		return ;
	element = *lst;
	while (element)
	{
		tmp = element;
		element = element->next;
		del(tmp->content);
		free(tmp);
	}
	*lst = NULL;
}
