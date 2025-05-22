ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

LIBFT = libft/libft.a

NAME = libft_malloc_$(HOSTTYPE).so

LINK = libft_malloc.so

MAIN = main.c

EXEC = malloc
EXEC_SYS = malloc_sys

CC = gcc

CFLAGS = -fPIC -shared -g #-Wall -Wextra -Werror

RM = rm -rf

SRCS =	src/malloc.c  \
		src/free.c    \
		src/realloc.c \
		src/utils.c   

OBJS = $(SRCS:.c=.o)

all : $(NAME)

$(NAME) : $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -L./libft -lft
	$(RM) $(LINK)
	ln -s $(NAME) $(LINK)
	export LD_LIBRARY_PATH=.

%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(LIBFT) :
	make bonus -C libft

exec :
	$(CC) $(MAIN) -g -o $(EXEC) -L. -lft_malloc -L./libft -lft -lpthread

exec_sys :
	$(CC) -g $(MAIN) -o $(EXEC_SYS) -L./libft -lft -lpthread

clean :
	$(RM) $(OBJS)

fclean : clean
	# make fclean -C libft
	$(RM) $(NAME) $(LINK) $(EXEC)

re : fclean all exec