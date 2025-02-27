#include "ft_malloc.h"

static t_zone	       *g_zones = NULL;
static pthread_mutex_t  g_mutex = PTHREAD_MUTEX_INITIALIZER;


static size_t	align_size(size_t size)
{
	return (size + 15) & ~15;
}


static t_block	*find_block(t_zone *zone, size_t size)
{
	t_block		*block = zone->blocks;

	while (block != NULL)
	{
	 	if (block->free != 0 && block->size >= size)
		{
			return block;
		}
		block = block->next;
	}
	return NULL;
}


static t_zone	*create_new_zone(int type, size_t size)
{
    size_t page_size = PAGE_SIZE;
    size_t zone_size;

    if (type == LARGE)
    {
        zone_size = align_size(size + sizeof(t_zone) + sizeof(t_block));
    }

    else
    {
        zone_size = page_size * 100; // No need t align because it's already a PAGE_SIZE multiple
    }

    void *ptr = mmap(NULL, zone_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

    if (ptr == (void *)-1)
    {
        return NULL;
    }

    t_zone  *zone = (t_zone *)ptr;

    zone->total_size = zone_size;
    zone->type       = type;
    zone->next       = g_zones;

    g_zones = zone;


    t_block *block = (t_block *)((char *)ptr + sizeof(t_zone));   // We are casting ptr into (char *) so we can increment byte by byte

    block->size  = zone_size - sizeof(t_zone) - sizeof(t_block);
    block->free  = 1;
    block->next  = NULL;
    block->prev  = NULL;

    zone->blocks = block;

    return zone;
}


void    *ft_malloc(size_t size)
{
    pthread_mutex_lock(&g_mutex);
    
    int type;

	if (size <= 0)
	{
        pthread_mutex_unlock(&g_mutex);
		return NULL;
	}

    size = align_size(size);

	if      (size <= TINY_MAX)  type = TINY;
	else if (size <= SMALL_MAX) type = SMALL;
	else 			            type = LARGE;

	t_zone	*zone        = g_zones;
    t_block *found_block = NULL;

/// Look in the existing blocks if there is a free block ///

	while (zone != NULL)
	{
		if (zone->type == type)
		{
			t_block *block = find_block(zone, size);
			
            if (block != NULL)
            {
                found_block = block;
                break;
            }
		}
		zone = zone->next;
	}

/// If no blocks are found, create a new zone ///

    if (found_block == NULL)
    {
        zone = create_new_zone(type, size);
        if (zone == NULL)
        {
            pthread_mutex_unlock(&g_mutex);
            return NULL;
        }
        found_block = zone->blocks;
    }

    if (found_block->size > size + sizeof(t_block))
    {
        t_block *new_block = (t_block *)((char *)found_block + sizeof(t_block) + size);
        
        new_block->size   = found_block->size - size - sizeof(t_block);
        new_block->free   = 1;
        new_block->next   = found_block->next;
        new_block->prev   = found_block;

        found_block->next = new_block;
        found_block->size = size;
    }

    found_block->free = 0;

    pthread_mutex_unlock(&g_mutex);

    return (void *)(found_block + 1);
}


// Exemple d'une zone TINY après deux allocations de 64 et 32 octets :

// +---------------------+
// | t_zone              |
// | total_size = 4096   |
// | blocks → bloc1      |
// +---------------------+
// | t_block (bloc1)     |
// | size = 64, free = 0 |
// +---------------------+
// | Données utilisateur |
// | (64 octets)         |
// +---------------------+
// | t_block (bloc2)     |
// | size = 32, free = 0 |
// +---------------------+
// | Données utilisateur |
// | (32 octets)         |
// +---------------------+
// | ... (espace libre)  |
// +---------------------+