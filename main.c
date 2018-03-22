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
*	Recharge un arbre depuis un fichier .DICO 
*	(Voir fonction save_as_string)
*/
void read_DICO(FILE *f, Arbre *a){
	char c;
	Arbre *tmp = a;
	if(fscanf(f,"%c",&c) != EOF){
		if(c == '\n'){
			tmp = NULL;
		}else{
			*tmp = allocNoeud(0,NULL,NULL); 
			if(c != ' '){
				(*tmp)->lettre = c;
				read_DICO(f,&((*tmp)->fg));
			}
			read_DICO(f,&((*tmp)->frd));
		}
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
void viewTree(char *filename, Arbre a){
	if(a != NULL && filename != NULL){		
		//Create dot file
		char dot_file_name[128];
		strcpy(dot_file_name, filename);  
		strcat(dot_file_name,".dot");
		FILE *f = fopen(dot_file_name,"w");

		//Creation du fichier dot
		fprintf(f, "digraph D {\nnode [shape=record];\n");
		createDotGraphContent(a,f);
		fprintf(f, "}");
		
		//Creation du pdf
		if(system("rm -f tree.pdf") == 0){
			char dot_command[128] = "dot -Tpdf "; //tree.dot -o tree.pdf
			strcat(dot_command, dot_file_name);
			strcat(dot_command, " -o tree.pdf");
			printf("%s\n",dot_command );
			if(system(dot_command) == 0){
				if(system("evince -w tree.pdf") == 0){
					printf("Lancement du PDF...\n");
				}
			}
		}
		fclose(f);
	}else{
		printf("[ERROR] : Impossible d'afficher l'arbre\n");
	}
}

/*
*   Affiche le man
*/
void afficheHelp() {
	if(system("cat man") == 0)
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
	printf("\tV\t\t Affiche l'arbre sous forme de PDF [Necessite dot & evince]\n");
	printf("\tq\t\t Quitter\n");
}


/*
*   lance la fonction correspondante
*	@params
*	Arbre a = Arbre chargé
*	Filename = Nom du fichier à traiter
*	choice = Choix du menu
*	optional_param : Parametre supplémentaire si necessaire
*/
void traiteMenu(Arbre a, char *filename, char *choice, char *optional_param) {
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
				printf("Données enregistrées dans le fichier %s\n",tmp_file_name);
				fclose(file_alphabetical_order);
				free(file_alphabetical_order);
				break;
			case 'r':
				//Recherche d'un mot
				if(optional_param == NULL)
					printf("Merci de préciser le mot recherché\n");
				else
					if(estPresent(&a, optional_param) == 1)
						printf("Le mot '%s' est est présent dans le fichier %s\n",optional_param,filename);
					else
						printf("Le mot '%s' est n'est pas présent dans le fichier %s\n",optional_param,filename);
				break;
			case 'S':
				//Sauvegarde dans .DICO
				strcpy(tmp_file_name, filename);
				strcat(tmp_file_name,".DICO");
				FILE *file_save_string = fopen(tmp_file_name, "w");
				save_as_string(file_save_string,a);	
				printf("Données enregistrées dans le fichier %s\n",tmp_file_name);
				fclose(file_save_string);
				free(file_save_string);
				break;
			case 'V':
				//View Tree avec dot
				viewTree(filename, a);
				break;
			default:
				printf("Option inconnue\n");
				break;
		}
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
*	Charge l'arbre a depuis un fichier DICO
*	@param : 	filename : Nom du fichier .DICO 
*				a : Arbre NULL
* 	@return :  	1 si probleme au chargement
*				0 si OK
*/
int load_from_dico(char *filename, Arbre *a){
	FILE *texte_DICO = fopen(filename, "r");
	if(texte_DICO == NULL)
		return 1;
	read_DICO(texte_DICO,a);
	fclose(texte_DICO);
	return 0;
}

/*
*	Charge l'arbre a depuis un fichier contenant du texte
*	@param : 	filename : Nom du fichier .DICO 
*				a : Arbre NULL
* 	@return :  	1 si probleme au chargement
*				0 si OK
*/
int load_from_file(char *filename, Arbre *a){
	FILE *texte = fopen(filename, "r");		
	if(texte == NULL)
		return 1;
	read_file_load_tree(texte,a);
	fclose(texte);
	return 0;
}


/*
* Libère la mémoire allouée a l'arbre
*/
void free_arbre(Arbre a){
    if(a != NULL){
        free_arbre(a->fg);
        free_arbre(a->frd);
        free(a);
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

	//Création de l'arbre
	Arbre a = NULL;
	char filename[128];
	
	if(argc >= 3 && strcmp(argv[1],"-L")  == 0){
		strcpy(filename,argv[2]);
		//Chargement depuis fichier DICO
		char DICO_filename[128];
		strcpy(DICO_filename, filename);
		strcat(DICO_filename, ".DICO");
		if(load_from_dico(DICO_filename,&a) != 0){
			fprintf(stderr, "ERROR : Fichier %s illisible\n",DICO_filename);
			return 1;	
		}
		argv = argv+1; 	
		argc--;	
	}else{
		//Lit dans le fichier
		strcpy(filename,argv[1]);
		if(load_from_file(filename, &a) != 0){
			fprintf(stderr, "ERROR : fichier %s illisible\n",filename);
			return 1;	
		}
	}

	
	//Menu
	if(argc == 2){
		//Si aucune option mentionnée à l'execution
		char choice[128]; 				//Choix sélectionné
		char optional_param[128] = ""; //Parametre optionnel si necessaire
		do{
			printf("---------------------------------------\n");
			afficheMenu();
			printf("Votre choix : ");
			scanf("%s", choice);
			if(strlen(choice) == 1 && choice[0] == 'r') //Necessaire pour récuperer le mot pour la recherche
				scanf("%s", optional_param);
			printf("---------------------------------------\n");
			
			if(choice[0] != 'q')
				traiteMenu(a,filename,choice,optional_param);
		}while(strlen(choice) != 1 || choice[0] != 'q'); //Tant que pas 'q' on boucle
	}else{
		//Si une option mentionnée à l'execution
		traiteMenu(a,filename,&argv[2][1],argv[3]);
	}
	
	//Free Arbre
	free_arbre(a);
	return 0;
}