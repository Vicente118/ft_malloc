#include "src/ft_malloc.h"  // Ajout de l'en-tête
#include "libft/libft.h"

int main()
{
    int A = 'A';
    char *str;

    str = ft_malloc(sizeof(char) * 20);
    if (!str)  // Toujours vérifier le retour de malloc
        return (1);
        
    ft_memset(str, A, 19);  // Utilisez votre implémentation de ft_memset
    str[19] = '\0';         // Terminaison de chaîne

    printf("%s\n", str);
    
    return 0;
}