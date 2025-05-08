3.1 Concepts de mémoire d'un processus
L'une des ressources les plus fondamentales dont dispose un processus est la mémoire. Il existe de nombreuses façons d'organiser la mémoire dans les systèmes, mais typiquement, chaque processus dispose d'un espace d'adressage virtuel linéaire, avec des adresses allant de zéro jusqu'à une valeur maximale très élevée. Cet espace n'a pas besoin d'être contigu ; c'est-à-dire que toutes ces adresses ne peuvent pas nécessairement être utilisées pour stocker des données.

La mémoire virtuelle est divisée en pages (4 kilooctets est une taille typique). Derrière chaque page de mémoire virtuelle se trouve une page de mémoire réelle (appelée cadre) ou un espace de stockage secondaire, généralement de l'espace disque. L'espace disque peut être de l'espace d'échange (swap) ou simplement un fichier disque ordinaire. En fait, une page composée uniquement de zéros n'a parfois rien du tout derrière elle – il y a juste un indicateur signalant qu'elle ne contient que des zéros.

Le même cadre de mémoire réelle ou de stockage secondaire peut soutenir plusieurs pages virtuelles appartenant à plusieurs processus. C'est normalement le cas, par exemple, avec la mémoire virtuelle occupée par le code de la bibliothèque GNU C. Le même cadre de mémoire réelle contenant la fonction printf est associé à une page de mémoire virtuelle dans chacun des processus existants qui a un appel printf dans son programme.

Pour qu'un programme puisse accéder à une partie quelconque d'une page virtuelle, cette page doit, à ce moment précis, être associée à ("connectée à") un cadre réel. Mais comme il y a généralement beaucoup plus de mémoire virtuelle que de mémoire réelle, les pages doivent régulièrement aller et venir entre la mémoire réelle et le stockage secondaire, arrivant en mémoire réelle lorsqu'un processus a besoin d'y accéder, puis retournant vers le stockage secondaire lorsqu'elles ne sont plus nécessaires. Ce mouvement est appelé pagination.

Lorsqu'un programme tente d'accéder à une page qui n'est pas à ce moment-là soutenue par de la mémoire réelle, cela est connu comme un défaut de page. Quand un défaut de page se produit, le noyau suspend le processus, place la page dans un cadre de page réel (ce qu'on appelle "charger la page" ou "faire défaut à la page"), puis reprend le processus de sorte que, du point de vue du processus, la page était en mémoire réelle depuis le début. En fait, pour le processus, toutes les pages semblent toujours être en mémoire réelle. À une exception près : le temps d'exécution d'une instruction qui serait normalement de quelques nanosecondes devient soudainement beaucoup, beaucoup plus long (car le noyau doit généralement effectuer des opérations d'E/S pour terminer le chargement de la page). Pour les programmes sensibles à cela, les fonctions décrites dans Verrouillage de pages peuvent le contrôler.

Au sein de chaque espace d'adressage virtuel, un processus doit garder une trace de ce qui se trouve à quelles adresses, et ce processus s'appelle l'allocation de mémoire. L'allocation évoque généralement la distribution de ressources rares, mais dans le cas de la mémoire virtuelle, ce n'est pas un objectif majeur, car il y en a généralement beaucoup plus que quiconque n'en a besoin. L'allocation de mémoire au sein d'un processus consiste principalement à s'assurer que le même octet de mémoire n'est pas utilisé pour stocker deux choses différentes.

Les processus allouent de la mémoire de deux façons principales : par exec et par programmation. En fait, la création de processus est une troisième façon, mais elle n'est pas très intéressante. Voir Création d'un processus.

Exec est l'opération de création d'un espace d'adressage virtuel pour un processus, de chargement de son programme de base et d'exécution du programme. Elle est effectuée par la famille de fonctions "exec" (par exemple execl). L'opération prend un fichier de programme (un exécutable), alloue de l'espace pour charger toutes les données de l'exécutable, les charge et transfère le contrôle au programme. Ces données sont notamment les instructions du programme (le texte), mais aussi les littéraux et les constantes du programme et même certaines variables : les variables C avec la classe de stockage statique (voir Allocation de mémoire dans les programmes C).

Une fois que ce programme commence à s'exécuter, il utilise l'allocation programmatique pour obtenir de la mémoire supplémentaire. Dans un programme C avec la bibliothèque GNU C, il existe deux types d'allocation programmatique : automatique et dynamique. Voir Allocation de mémoire dans les programmes C.

Les E/S mappées en mémoire constituent une autre forme d'allocation de mémoire virtuelle dynamique. Mapper la mémoire à un fichier signifie déclarer que le contenu d'une certaine plage d'adresses d'un processus doit être identique au contenu d'un fichier régulier spécifié. Le système fait en sorte que la mémoire virtuelle contienne initialement le contenu du fichier, et si vous modifiez la mémoire, le système écrit la même modification dans le fichier. Notez que grâce à la magie de la mémoire virtuelle et des défauts de page, le système n'a aucune raison d'effectuer des E/S pour lire le fichier, ou d'allouer de la mémoire réelle pour son contenu, jusqu'à ce que le programme accède à la mémoire virtuelle. Voir E/S mappées en mémoire.

De même qu'il alloue de la mémoire par programmation, le programme peut par programmation la libérer (free). Vous ne pouvez pas libérer la mémoire qui a été allouée par exec. Lorsque le programme se termine ou exécute un exec, on pourrait dire que toute sa mémoire est libérée, mais puisque dans les deux cas l'espace d'adressage cesse d'exister, la question est vraiment sans importance. Voir Terminaison de programme.

L'espace d'adressage virtuel d'un processus est divisé en segments. Un segment est une plage contiguë d'adresses virtuelles. Trois segments importants sont :

Le segment de texte contient les instructions, les littéraux et les constantes statiques d'un programme. Il est alloué par exec et reste de la même taille pendant toute la durée de vie de l'espace d'adressage virtuel.
Le segment de données est l'espace de travail du programme. Il peut être préalloué et préchargé par exec et le processus peut l'étendre ou le réduire en appelant des fonctions comme décrit dans Voir Redimensionnement du segment de données. Son extrémité inférieure est fixe.
Le segment de pile contient une pile de programme. Il grandit au fur et à mesure que la pile grandit, mais ne rétrécit pas lorsque la pile rétrécit.
3.2 Allocation de stockage pour les données de programme
Cette section traite de la façon dont les programmes ordinaires gèrent le stockage de leurs données, y compris la célèbre fonction malloc et certaines fonctionnalités plus sophistiquées spécifiques à la bibliothèque GNU C et au compilateur GNU.

3.2.1 Allocation de mémoire dans les programmes C
Le langage C prend en charge deux types d'allocation de mémoire via les variables dans les programmes C :

L'allocation statique est ce qui se passe lorsque vous déclarez une variable statique ou globale. Chaque variable statique ou globale définit un bloc d'espace, de taille fixe. L'espace est alloué une fois, lorsque votre programme est lancé (dans le cadre de l'opération exec), et n'est jamais libéré.
L'allocation automatique se produit lorsque vous déclarez une variable automatique, comme un argument de fonction ou une variable locale. L'espace pour une variable automatique est alloué lorsqu'on entre dans l'instruction composée contenant la déclaration, et est libéré lorsqu'on sort de cette instruction composée.
En GNU C, la taille du stockage automatique peut être une expression qui varie. Dans d'autres implémentations de C, ce doit être une constante.

Un troisième type important d'allocation de mémoire, l'allocation dynamique, n'est pas pris en charge par les variables C mais est disponible via les fonctions de la bibliothèque GNU C.

3.2.1.1 Allocation dynamique de mémoire
L'allocation dynamique de mémoire est une technique par laquelle les programmes déterminent, pendant leur exécution, où stocker certaines informations. Vous avez besoin d'allocation dynamique lorsque la quantité de mémoire dont vous avez besoin, ou la durée pendant laquelle vous en avez besoin, dépend de facteurs qui ne sont pas connus avant l'exécution du programme.

Par exemple, vous pouvez avoir besoin d'un bloc pour stocker une ligne lue à partir d'un fichier d'entrée ; puisqu'il n'y a pas de limite à la longueur d'une ligne, vous devez allouer la mémoire dynamiquement et la rendre dynamiquement plus grande au fur et à mesure que vous lisez plus de la ligne.

Ou, vous pouvez avoir besoin d'un bloc pour chaque enregistrement ou chaque définition dans les données d'entrée ; puisque vous ne pouvez pas savoir à l'avance combien il y en aura, vous devez allouer un nouveau bloc pour chaque enregistrement ou définition au fur et à mesure que vous les lisez.

Quand vous utilisez l'allocation dynamique, l'allocation d'un bloc de mémoire est une action que le programme demande explicitement. Vous appelez une fonction ou une macro quand vous voulez allouer de l'espace, et spécifiez la taille avec un argument. Si vous voulez libérer l'espace, vous le faites en appelant une autre fonction ou macro. Vous pouvez faire ces choses quand vous voulez, aussi souvent que vous voulez.

L'allocation dynamique n'est pas prise en charge par les variables C ; il n'existe pas de classe de stockage "dynamique", et il ne peut jamais y avoir de variable C dont la valeur est stockée dans un espace alloué dynamiquement. La seule façon d'obtenir de la mémoire allouée dynamiquement est via un appel système (qui passe généralement par un appel de fonction de la bibliothèque GNU C), et la seule façon de faire référence à un espace alloué dynamiquement est par l'intermédiaire d'un pointeur. Parce que c'est moins pratique, et parce que le processus réel d'allocation dynamique nécessite plus de temps de calcul, les programmeurs utilisent généralement l'allocation dynamique seulement lorsque ni l'allocation statique ni l'allocation automatique ne conviennent.

Par exemple, si vous voulez allouer dynamiquement un espace pour contenir une struct foobar, vous ne pouvez pas déclarer une variable de type struct foobar dont le contenu est l'espace alloué dynamiquement. Mais vous pouvez déclarer une variable de type pointeur struct foobar * et lui attribuer l'adresse de l'espace. Ensuite, vous pouvez utiliser les opérateurs '*' et '->' sur cette variable pointeur pour faire référence au contenu de l'espace :

```C
{
  struct foobar *ptr = malloc (sizeof *ptr);
  ptr->name = x;
  ptr->next = current_foobar;
  current_foobar = ptr;
}
```

3.2.2 L'allocateur GNU
L'implémentation de malloc dans la bibliothèque GNU C est dérivée de ptmalloc (pthreads malloc), qui est elle-même dérivée de dlmalloc (Doug Lea malloc). Ce malloc peut allouer de la mémoire de deux façons différentes selon leur taille et certains paramètres qui peuvent être contrôlés par les utilisateurs. La façon la plus courante consiste à allouer des portions de mémoire (appelées chunks) à partir d'une grande zone contiguë de mémoire et à gérer ces zones pour optimiser leur utilisation et réduire le gaspillage sous forme de chunks inutilisables. Traditionnellement, le tas système était configuré pour être la grande zone de mémoire, mais l'implémentation de malloc de la bibliothèque GNU C maintient plusieurs de ces zones pour optimiser leur utilisation dans les applications multi-threads. Chacune de ces zones est appelée en interne une arène.

Contrairement à d'autres versions, le malloc de la bibliothèque GNU C n'arrondit pas les tailles des chunks à des puissances de deux, ni pour les grandes ni pour les petites tailles. Les chunks voisins peuvent être fusionnés lors d'une libération quelle que soit leur taille. Cela rend l'implémentation adaptée à tous les types de modèles d'allocation sans généralement encourir un gaspillage élevé de mémoire par fragmentation. La présence de plusieurs arènes permet à plusieurs threads d'allouer de la mémoire simultanément dans des arènes séparées, améliorant ainsi les performances.

L'autre façon d'allouer de la mémoire concerne les très grands blocs, c'est-à-dire beaucoup plus grands qu'une page. Ces demandes sont allouées avec mmap (anonyme ou via /dev/zero ; voir E/S mappées en mémoire). Cela présente le grand avantage que ces chunks sont immédiatement rendus au système lorsqu'ils sont libérés. Par conséquent, il ne peut pas arriver qu'un grand chunk se retrouve "bloqué" entre des plus petits et gaspille de la mémoire même après avoir appelé free. Le seuil de taille pour l'utilisation de mmap est dynamique et s'ajuste en fonction des modèles d'allocation du programme. mallopt peut être utilisé pour ajuster statiquement le seuil avec M_MMAP_THRESHOLD et l'utilisation de mmap peut être complètement désactivée avec M_MMAP_MAX ; voir Paramètres réglables de Malloc.

Une description technique plus détaillée de l'allocateur GNU est maintenue dans le wiki de la bibliothèque GNU C. Voir https://sourceware.org/glibc/wiki/MallocInternals.

Il est possible d'utiliser votre propre malloc personnalisé à la place de l'allocateur intégré fourni par la bibliothèque GNU C. Voir Remplacer malloc.

3.2.3 Allocation sans contrainte
La facilité d'allocation dynamique la plus générale est malloc. Elle vous permet d'allouer des blocs de mémoire de n'importe quelle taille à tout moment, de les agrandir ou de les réduire à tout moment, et de libérer les blocs individuellement à tout moment (ou jamais).

3.2.3.1 Allocation de mémoire de base
Pour allouer un bloc de mémoire, appelez malloc. Le prototype de cette fonction est dans stdlib.h.

Fonction: void * malloc (size_t size)

Préliminaire: | MT-Safe | AS-Unsafe lock | AC-Unsafe lock fd mem | Voir Concepts de sécurité POSIX.

Cette fonction renvoie un pointeur vers un bloc nouvellement alloué de size octets de long, ou un pointeur nul (en définissant errno) si le bloc n'a pas pu être alloué.

Le contenu du bloc est indéfini ; vous devez l'initialiser vous-même (ou utiliser calloc à la place ; voir Allocation d'espace effacé). Normalement, vous convertiriez la valeur en un pointeur vers le type d'objet que vous souhaitez stocker dans le bloc. Voici un exemple de cette conversion et d'initialisation de l'espace avec des zéros en utilisant la fonction de bibliothèque memset (voir Copie de chaînes et tableaux) :

```C
struct foo *ptr = malloc (sizeof *ptr);
if (ptr == 0) abort ();
memset (ptr, 0, sizeof (struct foo));
```

Vous pouvez stocker le résultat de malloc dans n'importe quelle variable pointeur sans transtypage, car ISO C convertit automatiquement le type void * en un autre type de pointeur si nécessaire. Cependant, un transtypage est nécessaire si le type est requis mais non spécifié par le contexte.

N'oubliez pas que lors de l'allocation d'espace pour une chaîne, l'argument de malloc doit être supérieur d'une unité à la longueur de la chaîne. C'est parce qu'une chaîne se termine par un caractère nul qui n'est pas comptabilisé dans la "longueur" de la chaîne mais qui a besoin d'espace. Par exemple :

```C
char *ptr = malloc (length + 1);
```