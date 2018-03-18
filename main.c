/*
*   Auteurs : TROLARD Damien
*             BRAVO Noélie
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STRING_LENGTH 51 //With \0

typedef struct noeud{
	char lettre;
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
*	 les propriétés de l'arbre
*/
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
        Arbre tmp = allocNoeud(mot[0], NULL, *a);
        tmp->frd = *a;
        *a = tmp;	
        ajoutMot(&(tmp->fg), &mot[1]);
	}
	else if ((*a)->lettre < mot[0]){
		ajoutMot(&((*a)->frd), mot);
	}
}

/*
*   Recherche un mot dans l'arbre:  return 1 présent.
*                                   return 0 absent.
*/
int estPresent (Arbre *a, char *mot){
    if (*a == NULL){
        return 0;
    }
    else if((*a)->lettre == mot[0]){
        if(mot[0] == '\0'){
            return 1;
        }
        else estPresent(&((*a)->fg), &mot[1]);
    }
    else if((*a)->lettre < mot[0]){
        estPresent(&((*a)->frd), &mot[0]);
    }
    else if((*a)->lettre > mot[0]){
        return 0;
    }
    return 0;
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

/*
*	Créé le contenu d'un fichier dot
*/
void createDotGraphContent(Arbre a, FILE *f){
	if(a != NULL){	
		if(a->lettre == '\0')
			fprintf(f, "struct%p[label=\"<f0> |<f1> \\\\0|<f2> \"];\n", a);
		else
			fprintf(f, "struct%p[label=\"<f0> |<f1> %c|<f2> \"];\n", a, a->lettre);
		if(!(a->frd == NULL && a->fg == NULL)){
			if(a->fg != NULL){
				fprintf(f, "struct%p:f0 -> struct%p:f1;\n", a, a->fg);
				createDotGraphContent(a->fg,f);
			}
			if(a->frd != NULL){
				fprintf(f, "struct%p:f2 -> struct%p:f1;\n", a, a->frd);
				createDotGraphContent(a->frd,f);
			}
		}
	}
}

/*
*	Génère un fichier tree.dot correspondant à l'arbre
*	Créé via la commande dot le pdf lui correspondant
*	Affiche l'arbre via evince 
*/
void viewTree(Arbre a){
	if(a != NULL){
		FILE *f = fopen("tree.dot","w");
		
		//Create dot file
		fprintf(f, "digraph D {\nnode [shape=record];\n");
		createDotGraphContent(a,f);
		fprintf(f, "}");
	}else{
		printf("[ERROR] : Impossible d'afficher l'arbre : ARBRE NULL\n");
	}
}

int main(int argc, char const *argv[]) {
    
    Arbre a = NULL;
    char *mot1 = "ce";
    char *mot2 = "ces";
    char *mot3 = "des";
    char *mot4 = "le";
    char *mot5 = "les";
    char *mot6 = "lettre";
    char *mot7 = "mes";
    char *mot8 = "mettre";
     
    ajoutMot(&a, mot8);
    ajoutMot(&a, mot2);
    ajoutMot(&a, mot3);
    ajoutMot(&a, mot4);
    ajoutMot(&a, mot5);
    ajoutMot(&a, mot6);
    ajoutMot(&a, mot7);
    ajoutMot(&a, mot1);
    
    viewTree(a);
    
	/*if(argc == 1){
		fprintf(stderr, "ERROR : Fichier non spécifié\n");
		return 1;
	}
	//Chargement du fichier mis en parametre
	char filename[128];
	strcpy(filename,argv[argc-1]);
	FILE *texte = fopen(filename, "r");	
	if(texte == NULL){
		fprintf(stderr, "ERROR : Fichier impossible à lire\n");	
		return 1;
	}
	//Creation de l'arbre
	Arbre a = NULL;
	read_file_load_tree(texte,&a);
	
	//Sauvegarde dans l'ordre alphabetique dans .L
	FILE *file_alphabetical_order = fopen(strcat(filename,".L"), "w");
	save_alphabetical_order(file_alphabetical_order,a,NULL);	
	//Free
	fclose(texte);
	free(texte);
	fclose(file_alphabetical_order);
	free(file_alphabetical_order);
	return 0;*/
}