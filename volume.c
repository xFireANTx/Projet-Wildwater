#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	if(argc != 3){
		printf("Erreur: format attendue: %s fichier_entree.csv fichier_sortie.csv\n",argv[0]);
		return 1;
	}
	FILE* in = NULL;
	FILE* out = NULL;
	in=fopen(argv[1],"r");
	out=fopen(argv[2],"w");
	
	if(in == NULL || out == NULL){
		printf("Erreur ouverture de fichier\n");
		return 1;
	}
	char ligne_courante[100];
	double valeur;
	
	while(fscanf(in, "%99[^;];%lf\n",ligne_courante,&valeur)== 2){
		fprintf(out,"%s;%lf\n",ligne_courante,valeur/1000);
	}
	
	fclose(in);
	fclose(out);
}
