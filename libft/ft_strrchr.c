/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarras <vdarras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:28:12 by vdarras           #+#    #+#             */
/*   Updated: 2024/04/11 18:41:45 by vdarras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *str, int c)
{
	char	*buffer;
	int		i;

	buffer = NULL;
	i = ft_strlen(str);
	if (!*(str + i) && (char)c == '\0')
	{
		return ((char *)str + i);
	}
	while (*str)
	{
		if (*str == (char) c)
			buffer = ((char *) str);
		str++;
	}
	if (buffer)
		return ((char *) buffer);
	return (NULL);
}
/*
int main(void)
{
	const char *str = "Bonjour je suis un humain";
	int c = 115;
	printf("%s", ft_strrchr(str, c));
	return (0);
}
*/