/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarras <vdarras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:27:47 by vdarras           #+#    #+#             */
/*   Updated: 2024/04/08 18:53:17 by vdarras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *ptr, int value, size_t count)
{
	size_t	i;

	i = 0;
	while (i < count)
	{
		*(unsigned char *)(ptr + i) = (unsigned char) value;
		i++;
	}
	return (ptr);
}
/*
int main(void) 
{
	char array[] = "abcde";
	size_t size = sizeof(char) * 5;
	int length;
	for(length = 0; length < 5; length++) 
	{
		printf( "%c ", array[length]);
	}
	printf("\n");
	ft_memset(array, 122, size);
	for(length = 0; length < 5; length++) 
	{
		printf("%c ", array[length]);
	}
	printf("\n");
	return (0);
}
*/