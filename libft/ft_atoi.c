/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarras <vdarras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:27:18 by vdarras           #+#    #+#             */
/*   Updated: 2024/04/12 16:22:59 by vdarras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	skip(char *str)
{
	size_t	i;

	i = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	return (i);
}

int	ft_atoi(const char *str)
{
	long int	res;
	long int	buff;
	int			signe;

	res = 0;
	buff = 0;
	signe = 1;
	str += skip((char *)str);
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			signe *= -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		res = res * 10 + (*str - '0');
		if (buff > res && signe > 0)
			return (-1);
		else if (buff > res && signe < 0)
			return (0);
		str++;
		buff = res;
	}
	return (res * signe);
}
/*
int main(int argc, char **argv)
{
	char *s = "\t\v\f\r\n \f-06050";
    // const char *str = "\t  \n 8166";
	printf("%d\n",ft_atoi(s));
	printf("%d\n",atoi(s));
	// if (argc == 2) {
	// 	printf("le mien: [%d]\n", ft_atoi(argv[1]));
	// 	printf("le pas mien: [%d]\n", atoi(argv[1]));
	// }
	// else {
	// 	printf("le mien: [%d]\n", ft_atoi(str));
	// 	printf("le pas mien: [%d]\n", atoi(str));
	// }
    return (0);
}
*/