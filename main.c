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
	



	fclose(f);
	free(f);
	return 0;
}
