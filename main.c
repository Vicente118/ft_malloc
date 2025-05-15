#include "src/malloc.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define SUCCESS "\033[1;32m✓\033[0m "
#define ERROR "\033[1;31m✗\033[0m "

void test_malloc(void)
{
    printf("\n=== Test malloc ===\n");
    
    // Cas basique
    int *arr = malloc(20 * sizeof(int));
    if (arr) {
        for (int i = 0; i < 20; i++) arr[i] = i;
        printf(SUCCESS "Allocation basique et écriture\n");
        free(arr);
    }
    
    // Cas extrêmes
    void *p1 = malloc(0);
    printf(p1 == NULL ? SUCCESS : ERROR);
    printf("malloc(0) -> %p\n", p1);
    free(p1);
    void *p2 = malloc(1024 * 1024);
    printf(p2 ? SUCCESS : ERROR);
    printf("malloc(1MB) -> %p\n", p2);
    free(p2);
    
    // Allocations multiples
    void *ptrs[5];
    for (int i = 0; i < 5; i++) {
        ptrs[i] = malloc(100 * (i + 1));
        printf(ptrs[i] ? SUCCESS : ERROR);
    }
    printf("Allocations multiples\n");
    show_alloc_mem_ex();
    // Libérations dans un ordre aléatoire
    free(ptrs[2]);
    free(ptrs[0]);
    free(ptrs[4]);
    free(ptrs[1]);
    free(ptrs[3]);
    printf(SUCCESS "Libérations dans un ordre aléatoire\n");
}

void test_realloc(void)
{
    printf("\n=== Test realloc ===\n");
    
    // Cas basiques
    char *str = malloc(10);
    strcpy(str, "test");
    
    // Agrandissement
    str = realloc(str, 20);
    if (str && strcmp(str, "test") == 0)
        printf(SUCCESS "realloc agrandissement préserve les données\n");
    else
        printf(ERROR "realloc agrandissement a corrompu les données\n");
    
    // Réduction
    str = realloc(str, 3);
    if (str && strncmp(str, "te", 2) == 0)
        printf(SUCCESS "realloc réduction préserve les données\n");
    else
        printf(ERROR "realloc réduction a corrompu les données\n");
    
    // Cas spéciaux
    free(str);
    void *p1 = realloc(NULL, 10);
    printf(p1 ? SUCCESS : ERROR);
    printf("realloc(NULL, 10) -> %p\n", p1);
    
    void *p2 = realloc(p1, 0);
    printf(p2 == NULL ? SUCCESS : ERROR);
    printf("realloc(ptr, 0) -> %p\n", p2);
    
    // Pointeur invalide
    void *result = realloc((void*)0x12345678, 10);
    printf(result == NULL ? SUCCESS : ERROR);
    printf("realloc(invalid, 10) -> %p\n", result);
}

void test_free(void)
{
    printf("\n=== Test free ===\n");
    
    // free(NULL)
    free(NULL);
    printf(SUCCESS "free(NULL) ne crash pas\n");
    
    // Double free (commenté pour sécurité)
    int *ptr = malloc(sizeof(int));
    free(ptr);
    printf(SUCCESS "Premier free OK\n");
    // free(ptr);  // Décommenter pour tester le double free
}

void test_fragmentation(void)
{
    printf("\n=== Test fragmentation ===\n");
    
    // Créer de la fragmentation
    void *blocks[10];
    for (int i = 0; i < 10; i++)
        blocks[i] = malloc(50);
    
    // Libérer les blocs pairs
    for (int i = 0; i < 10; i += 2)
        free(blocks[i]);
    
    // Allouer de nouveau
    int success = 1;
    for (int i = 0; i < 10; i += 2) {
        blocks[i] = malloc(40);
        if (!blocks[i]) success = 0;
    }
    
    printf(success ? SUCCESS : ERROR);
    printf("Réallocation après fragmentation\n");
    
    // Nettoyage
    for (int i = 0; i < 10; i++)
        free(blocks[i]);
}

int main(void)
{
    printf("Tests de l'implémentation de malloc/free/realloc\n");
    
    test_malloc();
    test_realloc();
    test_free();
    test_fragmentation();
    
    return 0;
}