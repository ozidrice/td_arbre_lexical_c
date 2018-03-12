#include <stdio.h>
#include <stdlib.h>

typedef struct noeud{
    unsigned char lettre;
    struct noeud *fg, *frd;
} Noeud, *Arbre;

Arbre allocNoeud(char val, Arbre fg, Arbre frd){
    Arbre tmp;
    tmp = malloc(sizeof(Noeud));
	if (tmp != NULL){
		tmp->lettre = val;
		tmp->fg = fg;
		tmp->frd = frd;
	}
	return tmp;
}

void ajoutMot(Arbre *a, char *mot){
    if (*a == NULL){
        *a = allocNoeud(mot[0], NULL, NULL);
        if(mot[0] != '\0'){
            ajoutMot(&((*a)->fg), &mot[1]);
        }
    }
    else if((*a)->lettre == mot[0]){
        ajoutMot(&((*a)->fg), &mot[1]);
    }
    else if ((*a)->lettre > mot[0]){
        ajoutMot(&((*a)->frd), mot);
    }
    else if ((*a)->lettre < mot[0]){
        //mot[0] = (*a)->frd->lettre;
        ajoutMot(&((*a)->frd), mot);
    }
}

int main(){
    Arbre a = NULL;
    char *mot1 = "ce";
    char *mot2 = "ces";
    char *mot3 = "des";
    char *mot4 = "le";
    char *mot5 = "les";
    char *mot6 = "lettre";
    char *mot7 = "mes";
    char *mot8 = "mettre";
    
    ajoutMot(&a, mot1);
    ajoutMot(&a, mot2);
    ajoutMot(&a, mot3);
    ajoutMot(&a, mot4);
    ajoutMot(&a, mot5);
    ajoutMot(&a, mot6);
    ajoutMot(&a, mot7);
    ajoutMot(&a, mot8);
}