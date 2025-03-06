#include "src/ft_malloc.h"  // Ajout de l'en-tête
#include "libft/libft.h"
#include <stdalign.h>

void    *alloc_and_set_value(size_t size, int value)
{
    char    *str = malloc(size);
    if (!str)
    {
        printf("Malloc failed\n");
        return(NULL);
    }
    ft_memset(str, value, size);
    str[size] = '\0';   

    return str;
}

int main(int argc, char **argv)
{
    if (argc <= 1)
    {
        printf("Enter at least 1 argument to ./malloc\n");
        return 1;
    }

    int     A   = 'A';
    int     dot = '.';
    char    *str;

    if (!alloc_and_set_value(24444344, A))
    {
        return 1;
    }

    if (!alloc_and_set_value(84444344, A))
    {
        return 1;
    }

    if (!alloc_and_set_value(37443425, A))
    {
        return 1;
    }

    if (!alloc_and_set_value(444488347, A))
    {
        return 1;
    }

    show_alloc_mem();

    return 0;
} 

  