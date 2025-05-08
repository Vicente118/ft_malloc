#include "malloc.h"

pthread_mutex_t g_free_mutex = PTHREAD_MUTEX_INITIALIZER;

static int    munmap_if_free(t_zone  *zone, t_zone* prev_zone, bool can_free_zone)
{
    if (can_free_zone && (g_zones != zone || zone->next != NULL || prev_zone != NULL))
    {
        if (prev_zone != NULL)
            prev_zone->next = zone->next;
        else
            g_zones = zone->next;

        if (zone->next != NULL)
            zone->next->prev = prev_zone;

        if (munmap(zone, zone->total_size) < 0)
        {
            printf("Error from munmap syscall\n");
            return -1;
        }
    }

    return 0;
}



static t_block  *find_block_by_ptr(t_zone *zone, void *ptr)
{
    t_block *block = zone->blocks;

    while (block != NULL)
    {
        void *block_data = (void *)((char *)block + sizeof(t_block));

        if (block_data == ptr)
        {
            return block;
        }

        block = block->next;
    }

    return NULL;
}



static int  error_management(t_zone *zone, t_block *block)
{
    if (zone == NULL || block == NULL)
    {
        ft_putstr_fd("Error: invalid pointer\n", 2);
        return -1;
    }
    
    if (block->allocated == false)
    {
        ft_putstr_fd("Error: double free\n", 2);
        return -1;
    }

    return 0;
}



static void defragment_memory(t_block *block)
{
    if (block->next != NULL && block->next->allocated == false)
    {
        t_block *next_block = block->next;

        block->size += next_block->size + sizeof(t_block);
        block->next = next_block->next;

        if (next_block->next != NULL)
            next_block->next->prev = block;
    }

    if (block->prev != NULL && block->prev->allocated == false)
    {
        t_block *prev_block = block->prev;

        prev_block->size += block->size + sizeof(t_block);
        prev_block->next = block->next;

        if (block->next != NULL)
            block->next->prev = prev_block;

        block = prev_block;
    }
}


static bool is_zone_free(t_zone *zone)
{
    t_block *block = zone->blocks;

    while (block != NULL)
    {
        if (block->allocated == true)
        {
            return false;
        }
        block = block->next;
    }
    return true;
}



void    free(void *ptr)     // ptr is referencing to (void *)((char *)tmp_zone->blocks + sizeof(t_block))
{
    pthread_mutex_lock(&g_free_mutex);

    if (ptr == NULL)
    {
        pthread_mutex_unlock(&g_free_mutex);
        return ;
    }
    
    t_zone  *zone       = g_zones;
    t_zone  *prev_zone  = NULL;
    t_block *block      = NULL;
    
/// Loop to find block to free.

    while (zone != NULL)
    {
        void    *zone_start = (void *)zone;
        void    *zone_end   = (void *)((char *)zone + zone->total_size);

        if (ptr > zone_start && ptr < zone_end)
        {
            block = find_block_by_ptr(zone, ptr);

            if (block != NULL)
                break;
        }
        prev_zone = zone;
        zone = zone->next;
    }

/// Error managment

    if (error_management(zone, block) == -1)
    {
        pthread_mutex_unlock(&g_free_mutex);
        return;
    }

    block->allocated = false;  // Put block to free

/// Memory defragmentation

    defragment_memory(block);

    bool can_free_zone = is_zone_free(zone);

    if (munmap_if_free(zone, prev_zone, can_free_zone) < 0)
    {
        pthread_mutex_unlock(&g_free_mutex);
        return ;
    }

    pthread_mutex_unlock(&g_free_mutex);
}