/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarras <vdarras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:27:43 by vdarras           #+#    #+#             */
/*   Updated: 2024/04/11 17:32:32 by vdarras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *destination, const void *source, size_t size)
{
	size_t			i;
	unsigned char	*dst;
	unsigned char	*src;

	if (!destination && !source)
		return (NULL);
	dst = (unsigned char *) destination;
	src = (unsigned char *) source;
	i = 0;
	while (i < size)
	{
		dst[i] = src[i];
		i++;
	}
	return (destination);
}
/*
int main(void)
{
	char *str = "gsgsrfr";
	char str1[35];
	char *final = ft_memcpy(str1, str, sizeof(str));
	for(int i = 0; i < 7; i++)
		printf("%c", final[i]);
	return (0);
}
*/