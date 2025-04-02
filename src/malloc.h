#ifndef FT_MALLOC
# define FT_MALLOC

# include <unistd.h>
# include <stdio.h>
# include <sys/mman.h>
# include <sys/resource.h>
# include <pthread.h>

# define ALIGNEMENT    16			 	  // malloc from glibc is aligned to 16 bytes (Most optimal on 64 bits systems)
# define TINY_MAX      128
# define SMALL_MAX     512
# define MAP_ANONYMOUS 0x20
# define MIN_ALLOC_PER_ZONE 128
# define PAGE_SIZE     sysconf(_SC_PAGESIZE) // In Linux for x86-64 processors (4096), can be obtained with sysconf(_SC_PAGESIZE) in C or getconf PAGE_SIZE in Bash

# define BBLACK        "\033[90m"
# define BRED          "\033[91m"
# define BGREEN        "\033[92m"
# define BYELLOW       "\033[93m"
# define BBLUE         "\033[94m"
# define BMAGENTA      "\033[95m"
# define BCYAN         "\033[96m"
# define BWHITE        "\033[97m"
# define RESET         "\033[0m"

typedef enum bool
{
	false = 0,
	true  = 1
}	bool;

enum
{
	TINY,
	SMALL,
	LARGE
};

typedef struct s_block t_block;
typedef struct s_zone t_zone;

// 32 bytes
struct s_block
{
 	size_t          size;		// Size of allocated block
	bool            allocated;	// Tells if the block is free to use or not (used = 1, free = 0)
	t_block         *next; 		// Pointer to the next block
	t_block	        *prev;		// Pointer to the prev block
};

// 32 bytes
struct s_zone
{
    int             type;       // Type of zone : TINY (0), SMALL (1), LARGE (, SMALL (1), LARGE (2)2)
  	size_t		    total_size;	// Total size of the zone
  	t_block		    *blocks;	// List of block in this zone
	t_zone	        *next;		// Pointer to the next zone
};

extern t_zone	*g_zones;
extern pthread_mutex_t g_mutex; 

void	*mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
int	    munmap(void *addr, size_t length);
int	    getrlimit(int resource, struct rlimit *rlim);
long	sysconf(int name);

void    *malloc(size_t size);
void    *realloc(void *ptr, size_t size);
void    free(void *ptr);

void    show_alloc_mem();
t_zone  *reverse_list(t_zone *head) ;


#endif


/*
 
 -Malloc takes an input refering to the number of bytes we want to dynamically allocate.
It returns a pointer to a block of memory of that size. This pointer contains the address of that block.

- Realloc adjusts the size of the memory block while keeping data intact

- Free cleanup the memory block

Static and Global variables: Lives in memory from the starts until the end of the program.

Data Structure:
	- Zone/Heap  : Memory zone allocated by mmap
	- Blocks: Each heap is filled with blocks of memory

getconf PAGE_SIZE = 4096 bytes

*/
