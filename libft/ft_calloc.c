/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarras <vdarras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:27:25 by vdarras           #+#    #+#             */
/*   Updated: 2024/04/11 18:35:52 by vdarras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t n, size_t size)
{
	void	*ptr;
	size_t	buffer;

	buffer = n * size;
	if (size != 0 && (buffer / size) != n)
		return (NULL);
	ptr = malloc(buffer);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, buffer);
	return (ptr);
}
/*
int main(void)
{
	int *ptr = ft_calloc(0, 10);
	int *ptr1 = calloc(0, 10);
	printf("Le mien  : [%c]\nPas le miens [%c]" , ptr[0], ptr1[0]);

	return (0);
}
*/