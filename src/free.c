#include "malloc.h"

static int    munmap_if_free(t_zone  *zone, t_zone* prev_zone, bool can_free_zone)
{
/*
    If all block of a zone are free and is NOT the last zone, then munmap this zone and adjust pointers.
*/

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
/*
    Find block corresponding to the pointer to free.
*/
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
/*
    Check if next block exist AND is free. Add next size block to initial block to merge them into one block.
    Then adjust spointer.
*/

    if (block->next != NULL && block->next->allocated == false)
    {
        t_block *next_block = block->next;

        block->size += next_block->size + sizeof(t_block);
        block->next = next_block->next;

        if (next_block->next != NULL)
            next_block->next->prev = block;
    }

/*
    Same here but with prev block.    
*/

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
/*
    Check if every block of a zone are free to munmap zone just after.
*/
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
    pthread_mutex_lock(&g_mutex);

    if (ptr == NULL)
    {
        pthread_mutex_unlock(&g_mutex);
        return ;
    }
    
    t_zone  *zone       = g_zones;
    t_zone  *prev_zone  = NULL;         //Keep a trace of prev zone in case of full zone liberation.
    t_block *block      = NULL;
    
/* 
    Loop to find block to free. Check if pointer to free is in the zone.
    If yes, enter into find_block_by_ptr() to find the right block to free.
*/

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


    if (error_management(zone, block) == -1)
    {
        pthread_mutex_unlock(&g_mutex);
        return;
    }

    block->allocated = false;  // Put block to free
    memset((void*)((char*)block + sizeof(t_block)), 0, block->size); // Erase data in memory for security reason

    defragment_memory(block);

    bool can_free_zone = is_zone_free(zone);

    if (munmap_if_free(zone, prev_zone, can_free_zone) < 0)
    {
        pthread_mutex_unlock(&g_mutex);
        return ;
    }

    pthread_mutex_unlock(&g_mutex);
}