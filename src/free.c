#include "malloc.h"



int    munmap_if_free(void *ptr, t_zone  *zone)
{
    t_block *tmp_block = zone->blocks;

    while (tmp_block)
    {
        if (tmp_block->allocated == false)
        {
            return 0;
        }
        tmp_block = tmp_block->next;
    }

    int ret = munmap(ptr, zone->total_size);

    if (ret < 0)
    {
        printf("Error from munmap syscall\n");
        return -1;
    }

    return 0;
}

void    free(void *ptr)     // ptr is referencing to (void *)((char *)tmp_zone->blocks + sizeof(t_block))
{
    if (ptr == NULL)
    {
        return ;
    }

    t_zone  *ordered_zones = reverse_list(g_zones);



    if (munmap_if_free(ptr, ordered_zones) < 0)
    {
        return ;
    }
}


// Check if asked pointer correspond to an allocated address