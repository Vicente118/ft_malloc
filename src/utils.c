#include "malloc.h"

void    print_char_as_hex(unsigned char c)
{
    char    *base = "0123456789abcdef";
    char    buffer[3];
    int     int_c = c;

    buffer[0] = base[int_c / 16]; 
    buffer[1] = base[int_c % 16];
    buffer[2] = '\0';
    
    ft_putstr_fd(buffer, 1);
}

void    print_memory_hex(void *addr, size_t size)
{
    unsigned char   *ptr =  (unsigned char *)addr;

    for (size_t i = 0; i < size; i = i + 16)
    {
        print_address(ptr + i);
        ft_putstr_fd(": ", 1);

        for (size_t j = 0; j < 16; j++)
        {
            print_char_as_hex(ptr[i + j]);
            ft_putstr_fd(" ", 1);
        }

        ft_putstr_fd(" ", 1);

        char buffer[17] = {0};

        for (size_t n = 0; n < 16; n++)
        {
            if (ft_isprint(ptr[n + i]))
                buffer[n] = ptr[n + i];
            else
                buffer[n] = '.';
        }
        buffer[16] = '\0';

        write(1, buffer, 16);
        ft_putstr_fd("\n", 1);
    }
}

void    print_address(void *ptr)
{
    char            *base = "0123456789abcdef";
    unsigned long   addr = (unsigned long)ptr;
    int i = 17;

    char buffer[18];
    
    buffer[--i] = '\0';
    
    if (addr == 0)
    {
        buffer[--i] = '0';
    }

    else
    {
        while (addr > 0 && i > 2)
        {
            buffer[--i] = base[addr & 15]; // Get last hex digit
            addr >>= 4;
        }
    }
    
    buffer[--i] = 'x';
    buffer[--i] = '0';
    
    write(1, &buffer[i], 18 - i - 1);
}

void	unsigned_putnbr(unsigned int nb)
{
	if (nb > 9)
	{
		ft_putnbr_fd(nb / 10, 1);
		ft_putnbr_fd(nb % 10, 1);
	}

	else
    {
		ft_putchar_fd(nb + 48, 1);
    }
}

void    print_block(t_block *block, void *offset_address, size_t block_size)
{
    print_address(block);
    ft_putstr_fd(" - ", 1);
    print_address(offset_address);
    ft_putstr_fd(" : ", 1);
    unsigned_putnbr(block_size);
    ft_putstr_fd(" bytes\n", 1);
}

void    print_total(size_t allocated_bytes)
{
    ft_putstr_fd(BMAGENTA, 1);
    ft_putstr_fd("Total : ", 1);
    unsigned_putnbr(allocated_bytes);
    ft_putstr_fd(" bytes\n", 1);
    ft_putstr_fd(RESET, 1);
}