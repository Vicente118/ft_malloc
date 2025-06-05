#include "malloc.h"

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

void    *realloc(void *ptr, size_t size)
{
    pthread_mutex_lock(&g_mutex);

    t_zone  *zone   = g_zones;
    t_block *block  = NULL;

    if (ptr == NULL)
    {
        pthread_mutex_unlock(&g_mutex);
        return malloc(size);
    }

    if (size == 0)
    {
        pthread_mutex_unlock(&g_mutex);
        free(ptr);
        return NULL;
    }

    t_zone *tmp_zone = zone;
    while (tmp_zone != NULL && block == NULL)
    {
        block = find_block_by_ptr(tmp_zone, ptr);
        tmp_zone = tmp_zone->next;
    }


    if (block == NULL)
    {
        pthread_mutex_unlock(&g_mutex);
        return NULL;
    }

    if (block->size == size)
    {
        pthread_mutex_unlock(&g_mutex);
        return ptr;
    }

    size_t block_size = block->size;

    pthread_mutex_unlock(&g_mutex);

    void    *new_ptr = malloc(size);

    if (new_ptr == NULL)
        return NULL;

    size_t copy_size;

    if (block_size < size)
        copy_size = block_size;
    else
        copy_size = size;

    ft_memcpy(new_ptr, ptr, copy_size);

    free(ptr);

    return new_ptr;
}