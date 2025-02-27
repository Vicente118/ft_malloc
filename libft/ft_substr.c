/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarras <vdarras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:28:15 by vdarras           #+#    #+#             */
/*   Updated: 2024/04/08 17:38:06 by vdarras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*str;
	size_t	slen;
	size_t	i;

	if (!s)
		return (NULL);
	slen = len;
	if (ft_strlen(s) - start < len)
		slen = ft_strlen(s) - start;
	if (start > ft_strlen(s))
		return (ft_strdup(""));
	str = malloc(sizeof(char) * (slen + 1));
	if (!str)
		return (NULL);
	i = 0;
	while (*(s + start) && i < len && start < ft_strlen(s))
	{
		str[i] = *(char *)(s + start);
		i++;
		s++;
	}
	str[i] = '\0';
	return (str);
}
/*
int main(void)
{
	char const		*s = "Hello world";
	unsigned int	start = 2;
	size_t			len = 11;
	printf("%s", ft_substr(s,start,len));
	return (0);
}
*/
