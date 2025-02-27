/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarras <vdarras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 14:24:05 by vdarras           #+#    #+#             */
/*   Updated: 2024/04/11 17:36:21 by vdarras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (lst && new)
	{
		new->next = *lst;
		*lst = new;
	}
}
/*
int	main()
{
	t_list	*head, *node_1, *node_2;

	head = malloc(sizeof(t_list));
	node_1 = malloc(sizeof(t_list));
	node_2 = malloc(sizeof(t_list));

	head->content = "hello";
	node_1->content = "world";
	node_2->content = "!";
	
	ft_lstadd_front(&head, node_1);
	ft_lstadd_front(&head, node_2);

	t_list	*current;
	
	current = node_2;
	while (current)
	{
		printf("%s\n", (char *)(current->content));
		current = current->next;
	}
}
*/