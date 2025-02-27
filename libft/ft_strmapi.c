/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarras <vdarras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 14:29:19 by vdarras           #+#    #+#             */
/*   Updated: 2024/04/05 16:58:21 by vdarras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
char change_char(unsigned int nb, char c)
{
	return (c + nb);
}
*/

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char				*new_s;
	unsigned int		i;

	i = 0;
	if (!s || !f)
		return (NULL);
	new_s = ft_strdup(s);
	if (!new_s)
		return (NULL);
	while (new_s[i])
	{
		new_s[i] = (*f)(i, new_s[i]);
		i++;
	}
	new_s[i] = '\0';
	return (new_s);
}
/*
int main(void)
{
	char const s[18] = "Je suis un humain";
	printf("%s", ft_strmapi(s, &change_char));
	return (0);
}
*/