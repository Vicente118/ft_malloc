#include "src/malloc.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define SUCCESS "\033[1;32m✓\033[0m "
#define ERROR "\033[1;31m✗\033[0m "

int main()
{
    int i;
    char *addr;
    i = 0;
    while (i < 1024)
    {
        addr = (char*)malloc(1024);
        addr[0] = 42;
        i++;
    }
    return (0);
}