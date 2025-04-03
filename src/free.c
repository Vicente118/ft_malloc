#include "malloc.h"

pthread_mutex_t g_free_mutex = PTHREAD_MUTEX_INITIALIZER;

static int    erase_block(void *ptr, t_zone *zone)
{
    t_block *tmp_block = zone->blocks;

    if (ptr == (void *)((char *)tmp_block + sizeof(t_block)))
    {
        if (tmp_block->next != NULL)
        {
            zone->blocks    = tmp_block->next;
            tmp_block->next = NULL;
        }
    }    

    while (ptr != (void *)((char *)tmp_block + sizeof(t_block)))
    {
        if (tmp_block->next == NULL)
        {
            return -1;
        }
        tmp_block = tmp_block->next;
    }

    return 0;
}

static int    munmap_if_free(t_zone  *zone)
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

    int ret = munmap(zone, zone->total_size);

    if (ret < 0)
    {
        printf("Error from munmap syscall\n");
        return -1;
    }

    return 0;
}

void    free(void *ptr)     // ptr is referencing to (void *)((char *)tmp_zone->blocks + sizeof(t_block))
{
    pthread_mutex_lock(&g_free_mutex);

    if (ptr == NULL)
    {
        pthread_mutex_unlock(&g_free_mutex);
        return ;
    }

    t_zone  *ordered_zones = g_zones;

    if (munmap_if_free(ordered_zones) < 0)
    {
        pthread_mutex_unlock(&g_free_mutex);
        return ;
    }

    pthread_mutex_unlock(&g_free_mutex);
}


// Check if asked pointer correspond to an allocated address