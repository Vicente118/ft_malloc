#include "malloc.h"

t_zone	       *g_zones         = NULL;
pthread_mutex_t g_alloc_mutex   = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t g_display_mutex = PTHREAD_MUTEX_INITIALIZER;


static size_t	align_size(size_t size) 
{
	return (size + ALIGNEMENT - 1) & ~(ALIGNEMENT - 1);
}

static t_block	*find_block(t_zone *zone, size_t size)
{
	t_block		*block = zone->blocks;
    t_block     *best_fit = NULL;

	while (block != NULL)
	{
	 	if (block->allocated == false && block->size >= size)
		{
            if (block->size == size)   // If exact same size block found, function returns it
            {
			    return block;
            }

            if (best_fit == NULL || block->size < best_fit->size)  // Try to find the block with the best fit with the size
            {
                best_fit = block;
            }
		}
		block = block->next;
	}

	return best_fit;
}

static size_t	get_optimal_zone_size(int type, size_t size)
{
    size_t page_size = PAGE_SIZE;
    size_t zone_size;

    switch (type)
    {
        case TINY:                              
            zone_size = (sizeof(t_block) + TINY_MAX) * MIN_ALLOC_PER_ZONE + sizeof(t_zone);  // Size for 128 allocations for this zone
            zone_size = (zone_size + page_size - 1) & ~(page_size - 1);
            break;

        case SMALL:
            zone_size = (sizeof(t_block) + SMALL_MAX) * MIN_ALLOC_PER_ZONE + sizeof(t_zone);
            zone_size = (zone_size + page_size - 1) & ~(page_size - 1);
            break;

        case LARGE:
            zone_size = size + sizeof(t_block) + sizeof(t_zone);
            zone_size = (zone_size + page_size - 1) & ~(page_size - 1);
            break;
    }
    
    return zone_size;
}

static t_zone	*create_new_zone(int type, size_t size)
{
    size_t zone_size = get_optimal_zone_size(type, size);

    void *ptr = mmap(NULL, zone_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

    if (ptr == (void *)-1)
    {
        ft_putstr_fd("Error from mmap syscall\n", 2);
        return NULL;
    }

    t_zone  *zone = (t_zone *)ptr;

    zone->total_size = zone_size;
    zone->type       = type;
    zone->next       = g_zones;
    zone->prev       = NULL;

    if (g_zones != NULL)
    {
        g_zones->prev = zone;
    }

    g_zones = zone;


    t_block *block = (t_block *)((char *)ptr + sizeof(t_zone));   // We are casting ptr into (char *) so we can increment byte by byte

    block->size      = zone_size - sizeof(t_zone) - sizeof(t_block);
    block->allocated = false;
    block->next      = NULL;
    block->prev      = NULL;

    zone->blocks = block;
    return zone;
}

static void    print_zone_type(t_zone *zone)
{
    switch(zone->type)
    {
        case 0:
            ft_putstr_fd(BGREEN, 1);
            ft_putstr_fd("TINY : ", 1);
            break;
        case 1:
            ft_putstr_fd(BGREEN, 1);
            ft_putstr_fd("SMALL : ", 1);
            break;
        case 2:
            ft_putstr_fd(BGREEN, 1);
            ft_putstr_fd("LARGE : ", 1);
    }
    print_address(zone);
    ft_putstr_fd("\n", 1);
    ft_putstr_fd(RESET, 1);
}

void    show_alloc_mem()
{
    pthread_mutex_lock(&g_display_mutex);

    size_t  allocated_bytes = 0;
    t_zone  *tmp_zone       = g_zones;

    if (g_zones == NULL)
    {
        ft_putstr_fd(BRED, 1);
        ft_putstr_fd("No allocation to display\n", 1);
        ft_putstr_fd(RESET, 1);
        pthread_mutex_unlock(&g_display_mutex);
        return;
    }

    while (tmp_zone->next)
    {
        tmp_zone = tmp_zone->next;
    }

    while (tmp_zone)
    {
        t_block *tmp_block = tmp_zone->blocks;     
    
        print_zone_type(tmp_zone);

        while (tmp_block)
        {
            void    *offset_address = (void *)((char *)tmp_block + tmp_block->size);

            if (tmp_block->allocated == true)
            {
                allocated_bytes += tmp_block->size;
                
                print_block(tmp_block, offset_address, tmp_block->size);
            }
            tmp_block = tmp_block->next;
        }
        tmp_zone = tmp_zone->prev;

        ft_putstr_fd("\n", 1);
    }

    print_total(allocated_bytes);

    pthread_mutex_unlock(&g_display_mutex);
}

void    fragment_block(t_block *found_block, size_t size)
{
    t_block *new_block   = (t_block *)((char *)found_block + sizeof(t_block) + size);
    
    new_block->size      = found_block->size - size - sizeof(t_block);
    new_block->allocated = false;
    new_block->next      = found_block->next;
    new_block->prev      = found_block;

    if (new_block->next)  // If not the last block in list make the next prev pointer point on the right block
    {
        new_block->next->prev = new_block;
    }

    found_block->next    = new_block;
    found_block->size    = size;
}

void    *malloc(size_t size)
{
    pthread_mutex_lock(&g_alloc_mutex);

    int type;
    
	if ((ssize_t)size <= 0)
	{
        pthread_mutex_unlock(&g_alloc_mutex);
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

/// If no blocks are found or no zone has been created, create a new zone ///

    if (found_block == NULL)
    { 
        zone = create_new_zone(type, size);
        if (zone == NULL)
        {
            pthread_mutex_unlock(&g_alloc_mutex);
            return NULL;
        }
        found_block = zone->blocks;
    }

    if (found_block->size > size + sizeof(t_block))
    {
        fragment_block(found_block, size);
    }

    found_block->allocated = true;

    pthread_mutex_unlock(&g_alloc_mutex);

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
//

/*
Simplistically malloc and free work like this:

malloc provides access to a process's heap. The heap is a construct in the C core library (commonly libc) that allows objects to obtain exclusive access to some space on the process's heap.

Each allocation on the heap is called a heap cell. This typically consists of a header that hold information on the size of the cell as well as a pointer to the next heap cell. This makes a heap effectively a linked list.

When one starts a process, the heap contains a single cell that contains all the heap space assigned on startup. This cell exists on the heap's free list.

When one calls malloc, memory is taken from the large heap cell, which is returned by malloc. The rest is formed into a new heap cell that consists of all the rest of the memory.

When one frees memory, the heap cell is added to the end of the heap's free list. Subsequent malloc's walk the free list looking for a cell of suitable size.

As can be expected the heap can get fragmented and the heap manager may from time to time, try to merge adjacent heap cells.

When there is no memory left on the free list for a desired allocation, malloc calls brk or sbrk which are the system calls requesting more memory pages from the operating system.

Now there are a few modification to optimize heap operations.

    For large memory allocations (typically > 512 bytes, the heap manager may go straight to the OS and allocate a full memory page.
    The heap may specify a minimum size of allocation to prevent large amounts of fragmentation.
    The heap may also divide itself into bins one for small allocations and one for larger allocations to make larger allocations quicker.
    There are also clever mechanisms for optimizing multi-threaded heap allocation.
*/