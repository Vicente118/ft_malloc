/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarras <vdarras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:27:58 by vdarras           #+#    #+#             */
/*   Updated: 2024/04/11 15:05:20 by vdarras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *source)
{
	char	*dest;
	size_t	i;

	i = 0;
	dest = (char *) malloc(ft_strlen((char *) source) * sizeof(char) + 1);
	if (!dest)
		return (NULL);
	while (source[i])
	{
		*(dest + i) = *(char *)(source + i);
		i++;
	}
	dest[i] = '\0';
	return (dest);
}
/*
int main (void)
{
	const char *src = "Je suis un humain";
	printf("%s", ft_strdup(src));
	return (0);
}
*/