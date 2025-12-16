#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


int main(int argc, char *argv[]){
	if(argc != 3){
		printf("%s \nformat attendue: %s fichier_entree.csv fichier_sortie.csv\n",strerror(errno) ,argv[0]);
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
	
	char ligne_courante[200];
	char ligne_ref[200];
	
	float vol_courant=0;
	float vol_tot =0;
	//recuperation des infos de la première ligne
	if(fscanf(in, "%199[^;];%f\n", ligne_ref,&vol_courant)!=2){
		fclose(in);
		fclose(out);
		printf("%s est vide\n",argv[1]);
		return 0;
	}
	
	vol_tot = vol_courant;
	/*On récupère la ligne suivante, on regarde si c'est la même usine, on ajoute au total si oui,
	on l'ajoute au fichier de sortie sinon et on définit la nouvelle usine de référence*/
	while(fscanf(in, "%199[^;];%f\n", ligne_courante,&vol_courant)==2){
	
		if ( strcmp(ligne_courante,ligne_ref) == 0){
			vol_tot += vol_courant;
		}
		else{
			vol_tot = vol_tot/1000.0;
			fprintf(out,"%s;%f\n",ligne_ref,vol_tot);
			strcpy(ligne_ref,ligne_courante);
			vol_tot = vol_courant;
		}
	}
	vol_tot /= 1000.0;
	fprintf(out,"%s;%f\n",ligne_ref,vol_tot);
	
	fclose(in);
	fclose(out);
	return 0;
}
