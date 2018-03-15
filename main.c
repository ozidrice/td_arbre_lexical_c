#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STRING_LENGTH 51 //With \0

typedef struct noeud{
	unsigned char lettre;
	struct noeud *fg, *frd;
} Noeud, *Arbre;


/*
*	Alloue la mémoire necessaire à un arbre
*	et lui défini les valeurs entrées
*/
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

/*
*	Ajoute un mot à l'arbre en concervant
*	 les propriété de l'arbre
*/
void ajoutMot(Arbre *a, char *mot){
	if (*a == NULL){
		*a = allocNoeud(mot[0], NULL, NULL);
		if(mot[0] != '\0'){
			ajoutMot(&((*a)->fg), &mot[1]);
		}
	}
	else {
		if((*a)->lettre == mot[0]){
			ajoutMot(&((*a)->fg), &mot[1]);
		}
		else {
			if ((*a)->lettre > mot[0]){
				ajoutMot(&((*a)->frd), mot);
			}
			else{
				if ((*a)->lettre < mot[0]){
					//TODO
					ajoutMot(&((*a)->frd), mot);//MARCHE PAS
				}
			}
		}
	}
}

/*
*	Lit le fichier f et charge tout les mots dans l'arbre
*/
void read_file_load_tree(FILE *f, Arbre *a){
	char tmp[MAX_STRING_LENGTH];
	while(fscanf(f,"%51s",tmp) != EOF){
		ajoutMot(a,tmp);
	}
}

/*
*	Sauvegarde dans le fichier tous les mots de l'arbre dans l'ordre alphabétique 
*	str = NULL pour lancer la fonction 
*/
void save_alphabetical_order(FILE *f, Arbre a, char *str){
	if(a != NULL){
		if(str == NULL){
			str = (char*)malloc(MAX_STRING_LENGTH);
			str[0] = '\0';
		}	
		char tmp[MAX_STRING_LENGTH];
		strcpy(tmp,str);
		strcat(tmp, &a->lettre);

		if(a->lettre == '\0')
			fprintf(f,"%s\n",tmp);
		save_alphabetical_order(f,a->fg,tmp);
		save_alphabetical_order(f,a->frd,str);
	}
}

int main(int argc, char const *argv[])
{
	if(argc == 1){
		fprintf(stderr, "ERROR : Fichier non spécifié\n");
		return 1;
	}

	char * file = argv[argc-1];

	FILE *texte = fopen(file, "r");	
	if(texte == NULL){
		fprintf(stderr, "ERROR : Fichier impossible à lire\n");	
		return 1;
	}

	Arbre a = NULL;
	read_file_load_tree(texte,&a);
	save_alphabetical_order(fopen(strcat(file,".L"), "w"),a,NULL);	

	fclose(texte);
	free(texte);
	return 0;
}
