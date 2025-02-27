/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarras <vdarras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:28:11 by vdarras           #+#    #+#             */
/*   Updated: 2024/04/08 19:33:49 by vdarras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *str1, const char *str2, size_t n)
{
	size_t	i;
	int		length;
	char	*buff1;
	char	*buff2;

	if (*str2 == '\0')
		return ((char *) str1);
	buff1 = (char *) str1;
	buff2 = (char *) str2;
	i = 0;
	length = ft_strlen(buff2);
	while (buff1[i] != '\0' && (i + length) <= n)
	{
		if (ft_strncmp((str1 + i), str2, length) == 0)
			return ((char *)(buff1 + i));
		i++;
	}
	return (NULL);
}
/*
int main(void)
{
	const char *str1 = "Je suis un Humain";
	const char *str2 = "Huma";
	printf("%s",ft_strnstr(str1, str2, 15));
	return (0);
}
*/