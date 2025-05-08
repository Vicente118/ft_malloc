# Cours universitaire: Gestion de la mémoire et implémentation de malloc

---

## Table des matières

1. Introduction
2. Architecture de la mémoire d'un processus
3. Mémoire virtuelle et pagination
4. Allocation dynamique et le tas
5. Implémentation de malloc
6. Structures de données et algorithmes
7. Fragmentation et optimisation
8. Thread-safety et concurrence
9. Cas d'utilisation et bonnes pratiques
10. Conclusion et perspectives
11. Références

---

## 1. Introduction

L'allocation de mémoire est un aspect fondamental de tout système informatique. En programmation C, la gestion manuelle de la mémoire via les fonctions malloc et `free` est essentielle pour développer des programmes efficaces et éviter les fuites mémoire. Ce cours explore en profondeur l'implémentation d'un allocateur de mémoire similaire à celui de la libc, mais conçu de manière pédagogique.

### 1.1 Objectifs du cours

- Comprendre l'organisation de la mémoire d'un processus
- Maîtriser les concepts de mémoire virtuelle et de pagination
- Analyser une implémentation de malloc, `free` et les fonctions associées
- Identifier les défis et solutions liés à l'allocation mémoire

### 1.2 Contexte historique

Les premières implémentations d'allocateurs dynamiques de mémoire remontent aux années 1960 avec des langages comme ALGOL. L'introduction de la fonction malloc en C dans les années 1970 a révolutionné la programmation en permettant une gestion flexible et dynamique de la mémoire. Depuis, de nombreuses optimisations ont été apportées aux allocateurs, comme ptmalloc (GNU/Linux), jemalloc (FreeBSD), tcmalloc (Google) et d'autres.

---

## 2. Architecture de la mémoire d'un processus

Lorsqu'un programme est chargé en mémoire, le système d'exploitation lui attribue un espace d'adressage virtuel complet. Cet espace est structuré en plusieurs segments aux fonctions distinctes.

### 2.1 Segments de mémoire

Comme indiqué dans le code source analysé:

```
+------------------+  Adresses basses
| Segment de texte |  (Code exécutable)
+------------------+
| Segment des      |  (Variables globales/statiques initialisées)
| données          |
+------------------+
| Segment BSS      |  (Variables globales/statiques non initialisées)
+------------------+
| Tas (Heap)       |  ← Zone gérée par malloc/free
|     ↓            |  (Croît vers les adresses hautes)
|                  |
+------------------+
|                  |
| Espace libre     |
|                  |
+------------------+
|     ↑            |
| Pile (Stack)     |  (Variables locales, paramètres de fonction)
+------------------+  (Croît vers les adresses basses)
| Arguments et     |
| variables        |
| d'environnement  |
+------------------+  Adresses hautes
```

#### 2.1.1 Segment de texte

Ce segment contient le code exécutable du programme, compilé à partir du code source. Il est généralement en lecture seule pour empêcher les modifications accidentelles ou malveillantes du code en exécution.

#### 2.1.2 Segment de données

Ce segment stocke les variables globales et statiques initialisées avec des valeurs non nulles. Par exemple:
```c
int global_var = 42; // Stocké dans le segment de données
```

#### 2.1.3 Segment BSS (Block Started by Symbol)

Le BSS contient les variables globales et statiques non initialisées ou initialisées à zéro. Le système d'exploitation optimise l'utilisation de la mémoire en ne réservant que l'espace nécessaire pour ces variables, sans stocker les valeurs zéro.
```c
int uninitialized_var; // Stocké dans le segment BSS
static int static_var = 0; // Également stocké dans le BSS
```

#### 2.1.4 Tas (Heap)

Le tas est une zone de mémoire dynamique qui croît vers les adresses hautes. C'est ici que malloc alloue de la mémoire pour les besoins dynamiques du programme. La gestion de cette zone est l'objet principal de notre étude.

#### 2.1.5 Pile (Stack)

La pile stocke les variables locales, les paramètres de fonction et les adresses de retour. Elle croît vers les adresses basses, à l'opposé du tas. Chaque appel de fonction pousse un nouveau cadre de pile (stack frame).

#### 2.1.6 Arguments et variables d'environnement

Cette région contient les arguments passés au programme (argv) et les variables d'environnement.

### 2.2 Durée de vie des variables

- **Variables statiques et globales**: durée de vie égale à celle du programme
- **Variables de la pile**: durée de vie limitée au bloc où elles sont déclarées
- **Variables du tas**: durée de vie contrôlée par le programmeur via malloc et `free`

---

## 3. Mémoire virtuelle et pagination

### 3.1 Concept de mémoire virtuelle

La mémoire virtuelle est une abstraction qui permet à chaque processus de "croire" qu'il dispose d'une mémoire continue et isolée, alors qu'en réalité:
- La mémoire physique est partagée entre tous les processus
- La mémoire virtuelle peut excéder la taille de la RAM physique
- Les données peuvent résider temporairement sur le disque (swap)

### 3.2 Avantages de la mémoire virtuelle

1. **Isolation**: Les processus ne peuvent pas accéder aux données des autres
2. **Protection**: Permissions d'accès (lecture/écriture/exécution) par page
3. **Efficacité**: Utilisation optimale de la mémoire physique disponible
4. **Simplicité**: Modèle de programmation plus simple avec un espace d'adressage continu

### 3.3 Pages et cadres de page

L'unité de base de la gestion mémoire est la page.

```c
# define PAGE_SIZE sysconf(_SC_PAGESIZE) // In Linux for x86-64 processors (4096)
```

- **Page**: Bloc de mémoire virtuelle de taille fixe (généralement 4 Ko)
- **Cadre de page**: Bloc de mémoire physique de même taille où la page peut être chargée
- **Table des pages**: Structure qui maintient la correspondance entre pages virtuelles et cadres physiques

### 3.4 Pagination à la demande

Les pages ne sont chargées en mémoire physique que lorsqu'elles sont accédées:

1. Le processus tente d'accéder à une adresse
2. Si la page n'est pas en mémoire, un défaut de page (page fault) se produit
3. Le système d'exploitation charge la page depuis le disque
4. L'exécution reprend

### 3.5 Appels système pour la gestion mémoire

```c
void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
int munmap(void *addr, size_t length);
```

`mmap` est utilisé pour:
1. Créer des mappages de fichiers en mémoire
2. Allouer de la mémoire anonyme (non liée à un fichier)

Dans notre implémentation, `mmap` est utilisé pour obtenir de grandes zones de mémoire directement du système d'exploitation:

```c
void *ptr = mmap(NULL, zone_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
```

Cette approche est différente de l'implémentation classique qui utilise `brk`/`sbrk` pour les petites allocations et `mmap` uniquement pour les grandes, mais elle simplifie l'implémentation tout en restant efficace.

---

## 4. Allocation dynamique et le tas

### 4.1 Rôle du tas dans la mémoire d'un processus

Le tas est la région de la mémoire où les allocations dynamiques sont effectuées. Contrairement à la pile qui suit un modèle LIFO strict, le tas permet des allocations et libérations dans un ordre arbitraire.

### 4.2 Défis de l'allocation dynamique

1. **Fragmentation externe**: Espaces libres dispersés entre des blocs alloués
2. **Fragmentation interne**: Espace gaspillé à l'intérieur des blocs alloués
3. **Efficacité**: Besoin de trouver rapidement des blocs appropriés
4. **Overhead**: Les métadonnées nécessaires à la gestion du tas occupent elles-mêmes de l'espace
5. **Concurrence**: Gestion des accès simultanés en environnement multi-thread

### 4.3 Fonctions d'allocation standard en C

- **malloc**: Alloue un bloc de mémoire de taille spécifiée
- **calloc**: Alloue et initialise à zéro un tableau d'éléments
- **realloc**: Redimensionne un bloc précédemment alloué
- **free**: Libère un bloc de mémoire précédemment alloué

### 4.4 Métadonnées de gestion

Chaque allocation nécessite des métadonnées pour gérer efficacement la mémoire. Dans notre implémentation:

```c
// 32 bytes
struct s_block
{
    size_t          size;       // Taille du bloc alloué
    bool            allocated;  // Indique si le bloc est alloué ou libre
    t_block         *next;      // Pointeur vers le bloc suivant
    t_block         *prev;      // Pointeur vers le bloc précédent
};
```

Ces métadonnées permettent de naviguer entre les blocs, de connaître leur état et leur taille, et ainsi de réutiliser efficacement l'espace libéré.

---

## 5. Implémentation de malloc

### 5.1 Vue d'ensemble de l'implémentation

L'implémentation analysée organise la mémoire en:
- **Zones**: Grandes régions obtenues via `mmap`
- **Blocs**: Sous-divisions des zones qui peuvent être allouées individuellement

```
+------------------+
| t_zone           |  En-tête de la zone
+------------------+
| t_block (bloc1)  |  En-tête du premier bloc
+------------------+
| Données bloc1    |  Espace utilisateur du bloc 1
+------------------+
| t_block (bloc2)  |  En-tête du deuxième bloc
+------------------+
| Données bloc2    |  Espace utilisateur du bloc 2
+------------------+
| ...              |
+------------------+
```

### 5.2 Structures de données principales

```c
// 40 bytes
struct s_zone
{
    int             type;       // Type de zone: TINY (0), SMALL (1), LARGE (2)
    size_t          total_size; // Taille totale de la zone
    t_block         *blocks;    // Liste des blocs dans cette zone
    t_zone          *next;      // Pointeur vers la zone suivante
    t_zone          *prev;      // Pointeur vers la zone précédente
};
```

L'implémentation utilise trois types de zones:
- **TINY**: Pour les allocations jusqu'à 128 octets
- **SMALL**: Pour les allocations entre 129 et 512 octets
- **LARGE**: Pour les allocations supérieures à 512 octets

### 5.3 La fonction malloc

```c
void *malloc(size_t size)
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
    else                        type = LARGE;

    t_zone  *zone        = g_zones;
    t_block *found_block = NULL;

    // Look in existing blocks for a free block
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

    // If no blocks are found or no zone has been created, create a new zone
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
```

Le flux d'exécution de cette fonction est:

1. **Verrouiller** le mutex pour éviter les problèmes de concurrence
2. **Valider** la taille demandée
3. **Aligner** la taille pour respecter les contraintes d'alignement mémoire
4. **Déterminer** le type de zone approprié
5. **Rechercher** un bloc libre de taille suffisante dans les zones existantes
6. Si nécessaire, **créer** une nouvelle zone
7. Si possible, **fragmenter** le bloc pour éviter le gaspillage
8. **Marquer** le bloc comme alloué
9. **Déverrouiller** le mutex
10. **Retourner** l'adresse après l'en-tête du bloc

### 5.4 Alignement mémoire

```c
static size_t align_size(size_t size) 
{
    return (size + ALIGNEMENT - 1) & ~(ALIGNEMENT - 1);
}
```

Cette fonction aligne la taille demandée sur une frontière de 16 octets, ce qui:
- Optimise les performances sur les architectures x86-64
- Respecte les contraintes d'alignement des types de données
- Évite les problèmes de performance liés aux accès non alignés

Exemple: Pour `size = 10`, le calcul donne `(10 + 16 - 1) & ~(16 - 1) = 25 & ~15 = 25 & 0xFFFFFFF0 = 16`.

---

## 6. Structures de données et algorithmes

### 6.1 Création de zones

```c
static t_zone *create_new_zone(int type, size_t size)
{
    size_t zone_size = get_optimal_zone_size(type, size);

    void *ptr = mmap(NULL, zone_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

    if (ptr == (void *)-1)
    {
        ft_putstr_fd("Error from mmap syscall\n", 2);
        return NULL;
    }

    t_zone *zone = (t_zone *)ptr;

    zone->total_size = zone_size;
    zone->type       = type;
    zone->next       = g_zones;
    zone->prev       = NULL;

    if (g_zones != NULL)
    {
        g_zones->prev = zone;
    }

    g_zones = zone;

    t_block *block = (t_block *)((char *)ptr + sizeof(t_zone));

    block->size      = zone_size - sizeof(t_zone) - sizeof(t_block);
    block->allocated = false;
    block->next      = NULL;
    block->prev      = NULL;

    zone->blocks = block;
    return zone;
}
```

Cette fonction:
1. Calcule une taille optimale pour la zone
2. Alloue la mémoire via `mmap`
3. Initialise la structure `t_zone`
4. Place la zone en tête de la liste chaînée de zones
5. Initialise un premier bloc qui occupe tout l'espace disponible
6. Associe ce bloc à la zone

### 6.2 Taille optimale des zones

```c
static size_t get_optimal_zone_size(int type, size_t size)
{
    size_t page_size = PAGE_SIZE;
    size_t zone_size;

    switch (type)
    {
        case TINY:                              
            zone_size = (sizeof(t_block) + TINY_MAX) * MIN_ALLOC_PER_ZONE + sizeof(t_zone);
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
```

Cette fonction dimensionne les zones selon leur type:
- Pour **TINY** et **SMALL**: espace pour au moins `MIN_ALLOC_PER_ZONE` (128) allocations
- Pour **LARGE**: juste l'espace nécessaire pour une allocation

La taille est toujours arrondie à un multiple de `PAGE_SIZE` pour optimiser l'utilisation de la mémoire physique.

### 6.3 Recherche de bloc libre

```c
static t_block *find_block(t_zone *zone, size_t size)
{
    t_block    *block = zone->blocks;
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
```

Cet algorithme implémente la stratégie "best-fit":
1. Parcourt tous les blocs de la zone
2. Cherche un bloc libre de taille exactement égale (match parfait)
3. Sinon, retient le plus petit bloc libre suffisamment grand
4. Cette approche minimise la fragmentation

### 6.4 Fragmentation des blocs

```c
void fragment_block(t_block *found_block, size_t size)
{
    t_block *new_block = (t_block *)((char *)found_block + sizeof(t_block) + size);
    
    new_block->size = found_block->size - size - sizeof(t_block);
    new_block->allocated = false;
    new_block->next = found_block->next;
    new_block->prev = found_block;

    if (new_block->next)  // If not the last block in list make the next prev pointer point on the right block
    {
        new_block->next->prev = new_block;
    }

    found_block->next = new_block;
    found_block->size = size;
}
```

Lorsqu'un bloc libre est beaucoup plus grand que nécessaire, cette fonction le divise en deux:
1. Un bloc alloué de la taille demandée
2. Un bloc libre avec le reste de l'espace

Ce mécanisme est essentiel pour éviter le gaspillage de mémoire.

---

## 7. Fragmentation et optimisation

### 7.1 Types de fragmentation

#### 7.1.1 Fragmentation externe

La fragmentation externe se produit quand l'espace libre total est suffisant mais dispersé en petits blocs non contigus, rendant impossible l'allocation d'un grand bloc.

```
[Alloc][Free 10][Alloc][Free 15][Alloc][Free 20]
```

Bien que 45 octets soient libres au total, on ne peut pas allouer un bloc de 30 octets.

#### 7.1.2 Fragmentation interne

La fragmentation interne se produit quand un bloc alloué est plus grand que nécessaire.

```
[Allocated: 128 bytes (utilized: 65)]
```

Ici, 63 octets sont gaspillés à l'intérieur du bloc.

### 7.2 Stratégies d'allocation

L'implémentation utilise une combinaison de stratégies:

1. **Best-fit**: Choisit le plus petit bloc suffisamment grand
2. **Binning**: Classification des allocations en catégories de taille (TINY, SMALL, LARGE)
3. **Fragmentation**: Division des blocs trop grands pour réduire le gaspillage

### 7.3 Optimisations possibles non implémentées

1. **Coalescence**: Fusion des blocs adjacents lors de la libération
2. **Caches de tailles fréquentes**: Stockage séparé pour les tailles courantes
3. **Compactage**: Réorganisation de la mémoire pour regrouper les blocs libres
4. **Réutilisation préférentielle**: Utilisation prioritaire des blocs récemment libérés pour améliorer la localité

---

## 8. Thread-safety et concurrence

### 8.1 Défis de la concurrence en allocation mémoire

En environnement multi-thread, plusieurs problèmes peuvent survenir:
- Corruptions de la structure interne de l'allocateur
- Conditions de course sur les blocs libres
- Double libération ou allocation du même bloc
- Interblocages (deadlocks)

### 8.2 Utilisation des mutex

```c
pthread_mutex_t g_alloc_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t g_display_mutex = PTHREAD_MUTEX_INITIALIZER;
```

L'implémentation utilise deux mutex distincts:
- `g_alloc_mutex`: Protège les opérations d'allocation et de libération
- `g_display_mutex`: Protège l'affichage des statistiques de mémoire

Cette séparation permet un verrouillage plus fin et limite les contentions.

### 8.3 Sections critiques

Dans la fonction malloc:
```c
pthread_mutex_lock(&g_alloc_mutex);
// ... opérations d'allocation ...
pthread_mutex_unlock(&g_alloc_mutex);
```

Dans la fonction `show_alloc_mem`:
```c
pthread_mutex_lock(&g_display_mutex);
// ... affichage des statistiques ...
pthread_mutex_unlock(&g_display_mutex);
```

Cette approche garantit l'exclusion mutuelle et protège l'intégrité des structures de données.

### 8.4 Optimisations possibles de concurrence

1. **Verrous à grain plus fin**: Verrouillage par zone plutôt que global
2. **Allocateurs par thread**: Caches locaux par thread pour réduire la contention
3. **Opérations atomiques**: Remplacement de certains mutex par des opérations atomiques
4. **Algorithmes sans verrou**: Structures de données lock-free pour certaines opérations

---

## 9. Cas d'utilisation et bonnes pratiques

### 9.1 Visualisation de l'état de la mémoire

La fonction `show_alloc_mem()` permet de visualiser l'état actuel des allocations:

```c
void show_alloc_mem()
{
    pthread_mutex_lock(&g_display_mutex);

    size_t allocated_bytes = 0;
    t_zone *tmp_zone = g_zones;

    // Parcours et affichage des zones et blocs...
    
    print_total(allocated_bytes);

    pthread_mutex_unlock(&g_display_mutex);
}
```

Cette fonction est cruciale pour le débogage et le suivi de la consommation mémoire.

### 9.2 Bonnes pratiques d'utilisation

1. **Libération systématique**: Toujours libérer la mémoire allouée
2. **Vérification des retours**: Tester si malloc a renvoyé NULL
3. **Allocation en une fois**: Préférer les grandes allocations aux multiples petites
4. **Réutilisation**: Redimensionner avec `realloc` plutôt que de libérer et réallouer
5. **Alignement**: Respecter les contraintes d'alignement pour les structures

### 9.3 Détection des erreurs courantes

1. **Fuites mémoire**: Mémoire allouée mais jamais libérée
2. **Dépassements de tampon**: Écriture au-delà de la zone allouée
3. **Double free**: Tentative de libérer deux fois le même bloc
4. **Use after free**: Utilisation d'un pointeur après sa libération

Des outils comme Valgrind sont utiles pour détecter ces erreurs.

---

## 10. Conclusion et perspectives

### 10.1 Forces de l'implémentation analysée

1. **Architecture claire**: Séparation en zones et blocs bien définis
2. **Gestion efficace de la mémoire**: Stratégie best-fit et fragmentation
3. **Thread-safety**: Protection contre les problèmes de concurrence
4. **Flexibilité**: Adaptation à différentes tailles d'allocation
5. **Debugging**: Fonctions de visualisation de l'état de la mémoire

### 10.2 Limites et améliorations possibles

1. **Absence de coalescence**: L'implémentation ne fusionne pas les blocs adjacents libres
2. **Gestion simple des grandes allocations**: Pas d'optimisations spécifiques pour les allocations très grandes
3. **Granularité des verrous**: Un seul mutex global peut limiter les performances en environnement très concurrent
4. **Pas de cache par thread**: Pas d'optimisation pour réduire la contention entre threads

### 10.3 Évolutions des allocateurs modernes

Les allocateurs modernes comme jemalloc, tcmalloc ou mimalloc intègrent:
- Des caches hiérarchiques (thread-local, processeur-local, global)
- Des techniques avancées de réduction de fragmentation
- Des algorithmes sans verrou pour améliorer la scalabilité
- Des heuristiques d'apprentissage pour s'adapter aux patterns d'allocation

### 10.4 L'avenir de la gestion mémoire

Les tendances futures incluent:
- L'intégration de l'intelligence artificielle pour prédire les besoins mémoire
- La gestion mémoire consciente de l'architecture NUMA pour les systèmes multicœurs
- L'optimisation pour les nouvelles technologies mémoire (HBM, PMEM)
- L'intégration plus étroite avec les ramasse-miettes des langages de haut niveau

---

## 11. Références

### 11.1 Publications académiques

- Wilson, P. R., Johnstone, M. S., Neely, M., & Boles, D. (1995). "Dynamic Storage Allocation: A Survey and Critical Review". International Workshop on Memory Management.
- Berger, E. D., McKinley, K. S., Blumofe, R. D., & Wilson, P. R. (2000). "Hoard: A Scalable Memory Allocator for Multithreaded Applications". ASPLOS-IX.

### 11.2 Documentation technique

- "The GNU C Library (glibc) Manual", section "Memory Allocation"
- "The Linux Programming Interface" par Michael Kerrisk, chapitres sur la gestion mémoire
- Documentation du projet jemalloc: http://jemalloc.net/

### 11.3 Ressources en ligne

- "A Malloc Tutorial" par Marwan Burelle: https://danluu.com/malloc-tutorial/
- "Inside memory management" sur IBM Developer Works
- "How malloc() and free() work" par CProgramming.com

---

Ce cours a présenté une analyse détaillée de la gestion de la mémoire dans un processus et une implémentation éducative de malloc. La compréhension de ces mécanismes est fondamentale pour tout développeur système ou toute personne travaillant sur des applications à haute performance où la gestion efficace de la mémoire est cruciale.