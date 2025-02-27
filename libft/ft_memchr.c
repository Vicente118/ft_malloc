/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarras <vdarras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:27:40 by vdarras           #+#    #+#             */
/*   Updated: 2024/04/11 15:02:53 by vdarras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *str, int c, size_t size)
{
	unsigned char	*buffer;

	buffer = (unsigned char *) str;
	while (size--)
	{
		if (*buffer == (unsigned char) c)
			return (buffer);
		buffer++;
	}
	return (NULL);
}
/*
int main(void)
{
	const char str[50] = "Bonjour je suis un humain";
	int c = 115;
	printf("%s", ft_memchr(str, c, 19));
	return (0);
}
*/