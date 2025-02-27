/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarras <vdarras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:28:06 by vdarras           #+#    #+#             */
/*   Updated: 2024/04/08 15:24:02 by vdarras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *destination, const char *source, size_t dstsize)
{
	size_t	srclen;

	srclen = ft_strlen(source);
	if (dstsize == 0)
		return (srclen);
	while (*source && dstsize - 1 > 0)
	{
		*destination = *source;
		dstsize--;
		destination++;
		source++;
	}
	if (dstsize > 0)
		*destination = '\0';
	return (srclen);
}
/*
int main(void)
{
	char dest[4];
	char *src = "Je suis";
	size_t len = ft_strlcpy(dest, src, 4);
	printf("taille de source = %zu", len);
	return (0);
}
*/