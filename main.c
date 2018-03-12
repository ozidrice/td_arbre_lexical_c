#include <stdio.h>
#include <stdlib.h>

typedef struct noeud{
    unsigned char lettre;
    struct noeud *fg, *frd;
} Noeud, *Arbre;

