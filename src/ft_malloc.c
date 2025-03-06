#include "ft_malloc.h"

static t_zone	       *g_zones = NULL;
static pthread_mutex_t  g_mutex = PTHREAD_MUTEX_INITIALIZER;


static size_t	align_size(size_t size)
{
	return (size + ALIGNEMENT - 1) & ~(ALIGNEMENT - 1);
}


static t_block	*find_block(t_zone *zone, size_t size)
{
	t_block		*block = zone->blocks;

	while (block != NULL)
	{
	 	if (block->allocated == false && block->size >= size)
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

    block->size      = zone_size - sizeof(t_zone) - sizeof(t_block);
    block->allocated = false;
    block->next  = NULL;
    block->prev  = NULL;

    zone->blocks = block;

    return zone;
}

static void    print_zone_type(t_zone *zone)
{
    switch(zone->type)
    {
        case 0: 
            printf("%s","TINY : ");
            break;
        case 1:
            printf("%s","SMALL : ");
            break;
        case 2:
            printf("%s","LARGE : ");
    }

    printf("%p\n\n", zone);
}

static t_zone  *reverse_list(t_zone *head) 
{
    t_zone  *prev = NULL, *current = head, *next = NULL;

    while (current != NULL) 
    {
        next = current->next; 
        current->next = prev; 
        prev = current;       
        current = next;       
    }

    return prev;
}

void    show_alloc_mem()
{
    size_t  allocated_bytes = 0;

    t_zone  *tmp_zone = reverse_list(g_zones);

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

                printf("%p - %p : %lu bytes\n", tmp_block, offset_address, tmp_block->size);
            }

            tmp_block = tmp_block->next;
        }

        tmp_zone = tmp_zone->next;
    }

    printf("Total : %lu bytes\n", allocated_bytes);
}


void    *malloc(size_t size)
{
    pthread_mutex_lock(&g_mutex);

    int type;

	if ((ssize_t)size <= 0)
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
        
        new_block->size      = found_block->size - size - sizeof(t_block);
        new_block->allocated = false;
        new_block->next      = found_block->next;
        new_block->prev      = found_block;

        found_block->next = new_block;
        found_block->size = size;
    }

    found_block->allocated = true;

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