#include <stdio.h>
#include <stdlib.h>

typedef struct noeud{
    unsigned char lettre;
    struct noeud *fg, *frd;
} Noeud, *Arbre;

Arbre allocNoeud(char val){
    Arbre tmp;
    tmp = malloc(sizeof(Noeud));
	if (tmp != NULL){
		tmp->lettre = val;
		tmp->fg = NULL;
		tmp->frd = NULL;
	}
	return tmp;
}

int main(){
    
}