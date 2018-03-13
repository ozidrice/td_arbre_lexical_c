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

FILE *openFile(char *file, char *mode){
	FILE *f = fopen(file,mode);
	if(f == NULL)
		return NULL;
	return f;
}

/*
void affiche(Tarbre a, char* str){
	if(a != NULL){
		if(str == NULL){
			char *c = (char*)malloc(128);
			c[0] = '\0';
			str = c;
		}
		char tmp[128];
		strcpy(tmp,str);

		affiche(a->frg,str);
		if(a->lettre == '\0')
			printf("%s\n",strcat(tmp,&a->lettre));
		affiche(a->fils,strcat(tmp,&a->lettre));
		affiche(a->frd,str);
	}
}
*/

void save_alphabetical_order(FILE *f, Arbre a, char *str){
	fprintf(f, "%s", a->lettre);

	if(str == NULL){
		str = (char*)malloc(51);
		str[0] = '\0';
	}
	char tmp[51];
	strcpy(tmp,str);

	if(a->lettre == '\0')
		printf("%s\n",strcat(tmp,&a->lettre));

	save_alphabetical_order(f,a->fg,tmp);
	save_alphabetical_order(f,a->frd,tmp);

}

int main(int argc, char const *argv[])
{
	if(argc == 1){
		fprintf(stderr, "ERROR : Fichier non spécifié\n");
		return 1;
	}


	FILE *texte = openFile(argv[argc-1], "r");	
	
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


	fclose(f);
	free(f);
	return 0;
}
