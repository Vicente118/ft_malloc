/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarras <vdarras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:27:23 by vdarras           #+#    #+#             */
/*   Updated: 2024/04/11 15:02:23 by vdarras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *ptr, size_t count)
{
	size_t	i;

	i = 0;
	while (i < count)
	{
		*(unsigned char *)(ptr + i) = 0;
		i++;
	}
}
/*
int main(void)
{	
	int tab[15];
	ft_bzero(tab, 15);
	printf("%d", tab[1]);
	return (0);
}
*/