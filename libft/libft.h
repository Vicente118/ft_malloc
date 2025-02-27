/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarras <vdarras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:23:08 by vdarras           #+#    #+#             */
/*   Updated: 2024/04/11 17:33:58 by vdarras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

// INCLUDES

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>

// STRUCTURES
typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

// PROTOTYPES PARTIE 1
int			ft_isalpha(int c);
int			ft_isdigit(int c);
int			ft_isalnum(int c);
int			ft_isascii(int c);
int			ft_isprint(int c);
size_t		ft_strlen(const char *str);
void		*ft_memset(void *ptr, int value, size_t count);
void		ft_bzero(void *ptr, size_t count);
void		*ft_memcpy(void *destination, const void *source, size_t size);
void		*ft_memmove(void *destination, const void *source, size_t size);
size_t		ft_strlcpy(char *destination, const char *source, size_t size);
size_t		ft_strlcat(char *destination, const char *source, size_t size);
int			ft_toupper(int c);
int			ft_tolower(int c);
char		*ft_strchr(const char *str, int c);
char		*ft_strrchr(const char *str, int c);
int			ft_strncmp(const char *str1, const char *str2, size_t n);
void		*ft_memchr(const void *str, int c, size_t size);
int			ft_memcmp(const void *str1, const void *str2, size_t size);
char		*ft_strnstr(const char *str1, const char *str2, size_t n);
int			ft_atoi(const char *str);
void		*ft_calloc(size_t n, size_t size);
char		*ft_strdup(const char *source);

// PROTOTYPES PARTIE 2
char		*ft_substr(char const *s, unsigned int start, size_t len);
char		*ft_strjoin(char const *s1, char const *s2);
char		*ft_strtrim(char const *s1, char const *set);
char		**ft_split(char const *s, char c);
char		*ft_itoa(int n);
char		*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void		ft_striteri(char *s, void (*f)(unsigned int, char *));
void		ft_putchar_fd(char c, int fd);
void		ft_putstr_fd(char *s, int fd);
void		ft_putendl_fd(char *s, int fd);
void		ft_putnbr_fd(int n, int fd);

// PROTOTYPES BONUS
t_list		*ft_lstnew(void *content);
void		ft_lstadd_front(t_list **lst, t_list *new);
int			ft_lstsize(t_list *lst);
t_list		*ft_lstlast(t_list *lst);
void		ft_lstadd_back(t_list **lst, t_list *new);
void		ft_lstdelone(t_list *lst, void (*del)(void *));
void		ft_lstclear(t_list **lst, void (*del)(void *));
void		ft_lstiter(t_list *lst, void (*f)(void *));
t_list		*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

#endif