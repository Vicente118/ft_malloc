/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarras <vdarras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:27:42 by vdarras           #+#    #+#             */
/*   Updated: 2024/04/11 15:14:29 by vdarras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *str1, const void *str2, size_t size)
{
	unsigned char	*buff1;
	unsigned char	*buff2;

	if (size == 0)
		return (0);
	buff1 = (unsigned char *) str1;
	buff2 = (unsigned char *) str2;
	while (buff1 && *buff1 == *buff2 && size-- != 1)
	{
		buff1++;
		buff2++;
	}
	return (*buff1 - *buff2);
}
/*
int main(void)
{
	unsigned char str1[15] = "shtsh";
	unsigned char str2[15] = "tfjxfj";
	printf("%d", ft_memcmp(str1, str2, 0));
	return (0);
}
*/