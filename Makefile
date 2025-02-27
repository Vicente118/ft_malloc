ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

LIBFT = libft/libft.a

NAME = libft_malloc_$(HOSTTYPE).so

LINK = libft_malloc.so

MAIN = main.c

EXEC = test

CC = gcc

CFLAGS = -Werror -Wall -Wextra -fPIC

RM = rm -rf

SRCS = src/ft_malloc.c

OBJS = $(SRCS:.c=.o)

all : $(NAME)

$(NAME) : $(OBJS) $(LIBFT)
	$(CC) -shared -o $(NAME) $(OBJS) -L./libft -lft
	$(RM) $(LINK)
	ln -s $(NAME) $(LINK)
	export LD_LIBRARY_PATH=.

%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(LIBFT) :
	make bonus -C libft

test :
	$(CC) $(MAIN) -o $(EXEC) -L. -lft_malloc -L./libft -lft

clean :
	$(RM) $(OBJS)

fclean : clean
	# make fclean -C libft
	$(RM) $(NAME) $(LINK) $(EXEC)

re : fclean all