#include "src/malloc.h"
#include "libft/libft.h"
#include <stdlib.h>

void    *alloc_and_check(char *src)
{
    size_t len = ft_strlen(src);

    char    *str = malloc(len + 1);
    
    if (!str)
    {
        printf("Malloc failed\n");
        return(NULL);
    }

    ft_strlcpy(str, src, len + 1);

    return str;
}

int main(int argc, char **argv)
{
    char    *str;
    char    *str2;
    char    *str3;

    if (!(str = alloc_and_check("Hello World !")))
    {
        return 1;
    }
    if (!(str2 = alloc_and_check("Printable Characters")))
    {
        return 1;
    }
    if (!(str3 = alloc_and_check("XOXOOXOXOXXOXOXOXOXOXOXOXOXOXO")))
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
    ft_putstr_fd("\n\n", 1);
    show_alloc_mem_ex();

    free(str);
    free(str2);
    free(str3);

    return 0;
}
