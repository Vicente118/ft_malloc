/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarras <vdarras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:27:45 by vdarras           #+#    #+#             */
/*   Updated: 2024/04/08 17:48:22 by vdarras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *destination, const void *source, size_t size)
{
	size_t			i;
	unsigned char	*dest;
	unsigned char	*src;

	if (!destination && !source)
		return (NULL);
	dest = (unsigned char *) destination;
	src = (unsigned char *) source;
	i = 0;
	if (dest > src)
	{
		while (size-- > 0)
			dest[size] = src[size];
	}
	else
	{
		while (i < size)
		{
			dest[i] = src[i];
			i++;
		}
	}
	return (dest);
}
/*
int main (void)
{
	char    str[40] = "je suis un humain";
	char    *dest = ft_memmove(str, str + 4, 5);
	printf("%s", dest);
	return (0);
}
*/