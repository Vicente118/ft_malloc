/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarras <vdarras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:27:57 by vdarras           #+#    #+#             */
/*   Updated: 2024/04/11 18:39:53 by vdarras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *str, int c)
{
	while (*str)
	{
		if (*str == (char) c)
		{
			return ((char *) str);
		}
		str++;
	}
	if (!*str && (char)c == '\0')
	{
		return ((char *) str);
	}
	return (NULL);
}
/*
int main(void)
{
	const char *str = "teste";
	int c = '\0';
	printf("%p", ft_strchr(str, c));
	return (0);
}
*/