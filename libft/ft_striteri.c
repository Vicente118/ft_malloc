/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarras <vdarras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 14:28:38 by vdarras           #+#    #+#             */
/*   Updated: 2024/04/11 17:58:47 by vdarras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/*
void	ft_capitalize(unsigned int i, char *c)
{
	if (0 == i)
	{
		if (*c >= 97 && *c <= 122)
			*c -= 32;
	}
}
*/

void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	size_t	i;

	i = 0;
	if (!s || !f)
		return ;
	while (s[i])
	{
		(*f)(i, &s[i]);
		i++;
	}
}
/*
int	main(int argc, char **argv)
{
	void	(*f)(unsigned int, char *);

	f = ft_capitalize;
	ft_striteri(argv[1], f);
	printf("%s\n", argv[1]);
}
*/