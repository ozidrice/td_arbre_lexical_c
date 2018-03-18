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
*	 les propriété de l'arbre
*/
void ajoutMot(Arbre *a, char *mot){
	if (*a == NULL){
		*a = allocNoeud(mot[0], NULL, NULL);
		if(mot[0] != '\0'){
			ajoutMot(&((*a)->fg), &mot[1]);
		}
	}
	else if((*a)->lettre == mot[0]){
		if(mot[0] != '\0')
			ajoutMot(&((*a)->fg), &mot[1]);
	}
	else if ((*a)->lettre > mot[0]){
		Arbre tmp = NULL;	
		ajoutMot(&tmp, mot);
		tmp->frd = *a;
		*a = tmp;
	}
	else if ((*a)->lettre < mot[0]){
		ajoutMot(&((*a)->frd), mot);
	}
}


/*
*	Lit le fichier f et charge tout les mots dans l'arbre
*/
void read_file_load_tree(FILE *f, Arbre *a){
	char tmp[MAX_STRING_LENGTH];
	while(fscanf(f,"%51s ",tmp) != EOF){
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
*	Sauvegarde dans le fichier tous les mots de l'arbre
*	dans le format d'un .DICO 
*	(préfixe contenant \n si arbre null, ' ' si \0
*	et si la lettre vaut \0 on ne sauvegarde que son frere droit
*/
void save_as_string(FILE *f, Arbre a){
	if(a != NULL){
		if(a->lettre != 0){
			fprintf(f, "%c", a->lettre);
			save_as_string(f,a->fg);
		}
		else 
			fprintf(f," ");
		save_as_string(f,a->frd);
	}else{
		fprintf(f,"\n");
	}
}


/*
*   Recherche un mot dans l'arbre:  return 1 présent.
*                                   return 0 absent.
*/
int estPresent (Arbre *a, char const *mot){
    if (*a == NULL){
        return 0;
    }
    else if((*a)->lettre == mot[0]){
        if(mot[0] == '\0'){
            return 1;
        }
        else return estPresent(&((*a)->fg), &mot[1]);
    }
    else if((*a)->lettre < mot[0]){
        return estPresent(&((*a)->frd), &mot[0]);
    }
    else if((*a)->lettre > mot[0]){
        return 0;
    }
    return 0;
}

/*
*   Affiche le man
*/
void afficheHelp() {
	if(system("cat man") != NULL)
		printf("Impossible d'afficher le manuel.\n");
}

/*
*   Affiche le menu de fonctionnalités
*/
void afficheMenu() {
	printf("Menu :\n");
	printf("\tl\t\t Afficher mots tri alphabétique\n");
	printf("\ts\t\t Sauvegarde mot tri alphabétique dans fichier .L\n");
	printf("\tr [mot]\t\t Indique si le mot est présent dans le fichier\n");
	printf("\tS\t\t Sauvegarde l'arbre dans un fichier .DICO\n");
	printf("\tq\t\t Quitter\n");
}

/*
*   lance la fonction correspondante
*	@params : 
*/
void traiteMenu(Arbre a, char *filename, char *choice, char *optional_params) {
	if(strlen(choice) != 1)
		printf("Option incomprise\n");
	else{
		char tmp_file_name[128];
		switch (choice[0]){
			case 'l':
				save_alphabetical_order(stdout,a,NULL);
				break;
			case 's':
				//Sauvegarde dans l'ordre alphabetique dans .L
				strcpy(tmp_file_name, filename);
				strcat(tmp_file_name,".L");
				FILE *file_alphabetical_order = fopen(tmp_file_name, "w");
				save_alphabetical_order(file_alphabetical_order,a,NULL);
				fclose(file_alphabetical_order);
				free(file_alphabetical_order);
				printf("Données enregistrées dans le fichier %s\n",tmp_file_name);
				break;
			case 'r':
				//Recherche d'un mot
				if(optional_params == NULL)
					printf("Merci de préciser le mot recherché\n");
				else
					if(estPresent(&a, optional_params) == 1)
						printf("Le mot '%s' est est présent dans le fichier %s\n",optional_params,filename);
					else
						printf("Le mot '%s' est n'est pas présent dans le fichier %s\n",optional_params,filename);
				break;
			case 'S':
				//Sauvegarde dans .DICO
				strcpy(tmp_file_name, filename);
				strcat(tmp_file_name,".DICO");
				FILE *file_save_string = fopen(tmp_file_name, "w");
				save_as_string(file_save_string,a);	
				fclose(file_save_string);
				free(file_save_string);
				printf("Données enregistrées dans le fichier %s\n",tmp_file_name);
				break;
			default:
				printf("Option inconnue\n");
				break;
		}
	}
}

int main(int argc, char *argv[])
{
	if(argc == 1){
		fprintf(stderr, "ERROR : Aucun parametre spécifié\n");
		return 1;
	}
	if(strcmp(argv[1],"-h")  == 0){
		afficheHelp();
		return 1;
	}

	//Chargement du fichier mis en parametre
	char filename[128];
	strcpy(filename,argv[1]);
	FILE *texte = fopen(filename, "r");	
	if(texte == NULL){
		fprintf(stderr, "ERROR : Fichier impossible à lire\n");	
		return 1;
	}

	//Creation de l'arbre
	Arbre a = NULL;
	read_file_load_tree(texte,&a);
	
	//Menu
	if(argc == 2){
		//Si aucune option mentionnée à l'execution
		char choice[128]; 				//Choix sélectionné
		char optional_params[128] = ""; //Parametre optionnel si necessaire
		do{
			printf("---------------------------------------\n");
			afficheMenu();
			printf("Votre choix : ");
			scanf("%s", choice);
			if(strlen(choice) == 1 && choice[0] == 'r') //Necessaire pour récuperer le mot pour la recherche
				scanf("%s", optional_params);
			printf("---------------------------------------\n");
			
			if(choice[0] != 'q')
				traiteMenu(a,filename,choice,optional_params);
		}while(strlen(choice) != 1 || choice[0] != 'q'); //Tant que pas 'q' on boucle
	}else{
		//Si une option mentionnée à l'execution
		traiteMenu(a,filename,&argv[2][1],argv[3]);
	}

	//Free
	fclose(texte);
	free(texte);

	
	//Free Arbre
	//TODO
	return 0;
}