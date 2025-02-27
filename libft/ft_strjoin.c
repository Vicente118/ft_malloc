/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarras <vdarras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:28:01 by vdarras           #+#    #+#             */
/*   Updated: 2024/04/05 14:29:09 by vdarras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*result;
	size_t	len;
	size_t	i;

	i = 0;
	if (!s1 || !s2)
		return (NULL);
	len = (ft_strlen(s1) + ft_strlen(s2));
	result = malloc((len + 1) * sizeof(char));
	if (!result)
		return (NULL);
	while (i < len)
	{
		if (i < ft_strlen(s1))
			result[i] = *(const char *)(s1 + i);
		if (i >= ft_strlen(s1))
			result[i] = *(char *)(s2 + i - ft_strlen(s1));
		i++;
	}
	result[i] = '\0';
	return (result);
}

// int main(void)
// {
// 	char const *s1 = "jaziotj^z";
// 	char const *s2 = "fghfth";
// 	printf("%s", ft_strjoin(s1,s2));
// 	return (0);
// }
