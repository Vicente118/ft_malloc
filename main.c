#include "src/malloc.h"
#include "libft/libft.h"

void    *alloc_and_set_value(size_t size, int value)
{
    char    *str = malloc(size);
    
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
    if (argc <= 1)
    {
        printf("Enter at least 1 argument to ./malloc\n");
        return 1;
    }

    int     A   = 'A';
    int     dot = '.';
    char    *str;
   
    if (!(str = alloc_and_set_value(1, A)))
    {
        return 1;
    }

    // if (!(alloc_and_set_value(12, A)))
    // {
    //     return 1;
    // }

    // if (!(alloc_and_set_value(25, A)))
    // {
    //     return 1;
    // }

    show_alloc_mem();

    free(str);
    
    return 0;
} 

  