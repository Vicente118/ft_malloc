#include "src/malloc.h"
#include "libft/libft.h"
#include <stdlib.h>

void    *alloc_and_set_value(size_t size, int value)
{
    char    *str = malloc(size + 1);
    
    if (!str)
    {
        printf("Malloc failed\n");
        return(NULL);
    }

    ft_memset(str, value, size - 1);
    str[size] = '\0';

    return str;
}

int main(int argc, char **argv)
{
    int     A    = 'A';
    int     dot  = '.';
    char    *str;

    if (!(str = alloc_and_set_value(13254, A)))
    {
        return 1;
    }
    // if (!(str = alloc_and_set_value(111, A)))
    // {
    //     return 1;
    // }
    // if (!(str = alloc_and_set_value(13452, A)))
    // {
    //     return 1;
    // }
    // if (!(str = alloc_and_set_value(242, A)))
    // {
    //     return 1;
    // }

    show_alloc_mem();

    free(str);
    
    return 0;
}
