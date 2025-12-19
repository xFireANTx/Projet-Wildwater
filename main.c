#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "basic_functions.h"
#include "avl.h"
#include "avl_histo_reel.h"
#include "avl_histo_traitement.h"


void afficher_infra(const infra *i) {
    if (i == NULL) {
        printf("Infra: NULL\n");
        return;
    }

    printf("Infra {\n");
    printf("  type           : %d\n", i->type);
    printf("  code_usine     : %s\n", i->code_usine);
    printf("  code_precedent : %s\n", i->code_precedent);
    printf("  code_actuel    : %s\n", i->code_actuel);
    printf("  flux           : %f\n", i->flux);
    printf("  fuite          : %f\n", i->fuite);
    printf("}\n");
}


void afficher_noeud_fuites(const arbres_fuites *n) {
    if (n == NULL) {
        printf("Noeud: NULL\n");
        return;
    }

    printf("Noeud Arbre_fuites {\n");
    afficher_infra(n->structure);
    printf("  premierf : %p\n", (void *)n->premierf);
    printf("  suivantf : %p\n", (void *)n->suivantf);
    printf("}\n");
}


void main(int argc, char* argv[]){
    /*if(argc != 5){
		printf("main: Erreur pas le bon nombre de fichiers apres l'executable\nFormat attendue: ./exe {fichier_entree.dat} {fichier_sortie.dat} {histo/leaks} {max/src/reel}\n");
		return 1;
	}
	FILE* entree = NULL;
	FILE* sortie = NULL;
	
	entree = fopen(argv[1],"r");
	sortie = fopen(argv[2],"w+");
	if(entree == NULL || sortie == NULL){
		printf("main: Erreur d'ouverture du fichier\n");
		return 1;
	}

    if (strcmp(argv[3], "histo") == 0) {
		char ligne[50];
		double vol_courant;
		if (strcmp(argv[4], "max") == 0) {
			Volume_traitement* arbre = NULL;
			while(fscanf(entree, "%49[^;];%lf\n", ligne,&vol_courant)==2){
				arbre=ajouter_vol_traitement(arbre,ligne,vol_courant);
			}
			infixe_traitement_inverse(arbre,sortie);
		}           
		else if (strcmp(argv[4], "src") == 0) {
			Volume_traitement* arbre = NULL;
			while(fscanf(entree, "%49[^;];%lf\n", ligne,&vol_courant)==2){
				arbre=ajouter_vol_source(arbre,ligne,vol_courant);
			}
			infixe_traitement_inverse(arbre,sortie);
		}
		else if (strcmp(argv[4], "reel") == 0) {
			Volume_reel* arbre = NULL;
			double perte;
			while(fscanf(entree, "%49[^;];%lf;%lf\n", ligne,&vol_courant,&perte)==3){
				arbre=ajouter_vol_reel(arbre,ligne,vol_courant,perte);
			}
			infixe_reel_inverse(arbre,sortie);
		} 
		else {
			printf("Erreur: 'max', 'src' ou 'reel' attendu en 4e argument\n");
			return 1;
		}
	}
	else if (strcmp(argv[3], "leaks") == 0) {
		printf("En cours\n");
	}
	else {
		printf("Erreur: 'histo' ou 'leaks' attendu en 3e argument\n");
		return 1;
	}*/
    FILE *fichier = fopen("test.csv", "r");
    //FILE *flux = fopen("histo_reel.dat", "r");
    int boucle_principale = 0;   int type = 0;
    char ligne[256];  char tmp[256];
    arbres_fuites *p1;
    infra *p2;
   /* while(fgets(ligne, sizeof(ligne), flux)){
        strcpy(tmp, ligne);          //  nom #code usine 11

    }*/
    while(fgets(ligne, sizeof(ligne), fichier)){
        strcpy(tmp, ligne);
        int type = detect_type(tmp);
        switch(type){
            case 0:
                printf("end\n");
                return;
                printf("1\n");
            case 1: // source pas utile pour les fuites
                printf("1\n");
            break;
            case 2: // usine
                printf("2\n");
                // mettre directement dans avl usine
            break;
            case 3:// stockage
                printf("3\n");
                p1 = createNode(remplir_infra(ligne, 3));
                afficher_noeud_fuites(p1);
            break;
            case 4://jonction
                printf("4\n");
                p1 = createNode(remplir_infra(ligne, 4));
                afficher_noeud_fuites(p1);
            break;
            case 5://service
                printf("5\n");
                p1 = createNode(remplir_infra(ligne, 5));
                afficher_noeud_fuites(p1);
            break;
            case 6://menage
                printf("6\n");
                p1 = createNode(remplir_infra(ligne, 6));
                afficher_noeud_fuites(p1);
            break;
        }
    }    
    fclose(fichier);
    fclose(entree);
    fclose(sortie);
}


