/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarras <vdarras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:27:55 by vdarras           #+#    #+#             */
/*   Updated: 2024/04/02 18:40:23 by vdarras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static size_t	count_words(char const *s, char c)
{
	size_t	words;
	size_t	indice;

	words = 0;
	while (*s)
	{
		indice = 0;
		while (*s == c && *s)
			s++;
		while (*s != c && *s)
		{
			if (indice == 0)
			{
				words++;
				indice = 1;
			}
			s++;
		}
	}
	return (words);
}

static char	*each_word(char *str, char charset)
{
	int		len_word;
	int		i;
	char	*word;

	i = 0;
	while (str[i] && str[i] != charset)
		i++;
	len_word = i;
	i = 0;
	word = (char *)malloc(sizeof(char) * (len_word + 1));
	if (!word)
		return (NULL);
	while (i < len_word)
	{
		word[i] = str[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

static char	*word_in_string(char *word, char c, int i, char **string)
{
	string[i] = each_word(word, c);
	if (!string[i])
	{
		while (i >= 0)
		{
			free(string[i]);
			i--;
		}
		return (NULL);
	}
	return (string[i]);
}

static int	ft_set_word(char **strings, char *str, int i, char c)
{
	strings[i] = word_in_string(str, c, i, strings);
	if (!strings[i])
	{
		free(strings);
		return (0);
	}
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	**strings;
	char	*str;
	int		i;

	i = 0;
	str = (char *)s;
	if (!str)
		return (NULL);
	strings = (char **)malloc(sizeof(char *) * (count_words(str, c) + 1));
	if (!strings)
		return (NULL);
	while (*str != '\0')
	{
		while (*str && *str == c)
			str++;
		if (*str)
		{
			if (ft_set_word(strings, str, i, c) == 0)
				return (NULL);
			i++;
		}
		while (*str && *str != c)
			str++;
	}
	strings[i] = 0;
	return (strings);
}
/*
int main(void)
{
	char c = ' ';
	char const *s = "   Je  suis  un    humain   ";
	char	**array = ft_split(s, c);
	printf("%s\n%s", array[0], array[1]);
	return (0);
}
*/