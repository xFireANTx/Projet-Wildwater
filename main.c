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


// Fonction récursive pour afficher l'AVL
void afficherAVL(arbre *a, int niveau) {
    if (a == NULL) return;

    // Affiche le sous-arbre droit en premier (pour visualiser comme un arbre)
    afficherAVL(a->fd, niveau + 1);

    // Indentation selon le niveau
    for (int i = 0; i < niveau; i++) {
        printf("    ");
    }

    // Affiche uniquement les infos de la racine (sans arbres_fuites)
    if (a->usine != NULL) {
        printf("%s (flux: %f)\n", a->usine->code_usine, a->usine->flux);
    }

    // Affiche le sous-arbre gauche
    afficherAVL(a->fg, niveau + 1);
}


void afficher_noeud_fuites(const arbres_fuites *n) {
    if (n == NULL) {
        printf("Noeud: NULL\n");
        return;
    }
    int i = 0;
    printf("Noeud Arbre_fuites {\n");
    afficher_infra(n->structure);
    printf("  fils%d : %p\n",i+1, (void *)n->suivant);
    while(n->suivant != NULL){
        i++;
        n = n->suivant;
        printf("  fils%d : %p\n",i+1, (void *)n->suivant);
    }

    printf("}\n");
}

int main(int argc, char* argv[]){
    if(argc != 6){
		printf("main: Erreur pas le bon nombre de fichiers apres l'executable\nFormat attendue: ./exe {fichier_complet} {fichier_entree.dat} {fichier_sortie.dat} {histo/leaks} {max/src/reel}\n");
		return 1;
	}
	FILE* entree = NULL;
	FILE* sortie = NULL;
	

	entree = fopen(argv[2],"r");
	sortie = fopen(argv[3],"w+");
	if(entree == NULL||sortie == NULL){
		printf("main: Erreur d'ouverture du fichier\n");
		return 1;
	}
	char ligne_courante[50];
	double vol_courant;
	double perte=0;
    if (strcmp(argv[4], "histo") == 0) {
		if (strcmp(argv[5], "max") == 0) {
			Volume_traitement* arbre_max = NULL;
			while(fscanf(entree, "%49[^;];%lf\n", ligne_courante,&vol_courant)==2){
				arbre_max = ajouter_vol_traitement(arbre_max,ligne_courante,vol_courant);
			}
			infixe_traitement_inverse(arbre_max,sortie);
		}           
		else if (strcmp(argv[5], "src") == 0) {
			Volume_traitement* arbre_src = NULL;
			while(fscanf(entree, "%49[^;];%lf\n", ligne_courante,&vol_courant)==2){
				arbre_src = ajouter_vol_source(arbre_src,ligne_courante,vol_courant);
			}
			infixe_traitement_inverse(arbre_src,sortie);
		}
		else if (strcmp(argv[5], "reel") == 0) {
			Volume_reel* arbre_reel = NULL;
			while(fscanf(entree, "%49[^;];%lf;%lf\n", ligne_courante,&vol_courant,&perte)==3){
				arbre_reel=ajouter_vol_reel(arbre_reel,ligne_courante,vol_courant,perte);
			}
			infixe_reel_inverse(arbre_reel,sortie);
		} 
		else {
			printf("Erreur: 'max', 'src' ou 'reel' attendu en 4e argument\n");
			return 1;
		}
	}
	else if (strcmp(argv[4], "leaks") == 0) {
        FILE *fichier = fopen(argv[1], "r");
        if (!fichier) {
            perror("fopen argv 1");
        }
        /*On créé temporaire_reel car on en a besoin par la suite (le traitement de celui-ci ne sera pas assigné à la sortie contrairement à son homologue dans histo reel)*/
        FILE *histo_reel = fopen("temporaire_reel","w");
        Volume_reel* arbre_reel = NULL;

		while(fscanf(entree, "%49[^;];%lf;%lf\n", ligne_courante,&vol_courant,&perte)==3){
			arbre_reel=ajouter_vol_reel(arbre_reel,ligne_courante,vol_courant,perte);
		}
		infixe_reel_inverse(arbre_reel,histo_reel);
        fclose(histo_reel);
        fclose(entree);
        
        FILE *flux = fopen("temporaire_reel", "r"); 
        if (!flux) {
            perror("fopen histo_reel.dat");
            if (fichier) fclose(fichier);
            return 1;
        }
        int boucle_principale = 0;   int type = 0;
        char ligne[256];  char tmp[256];
        arbres_fuites *p1;
        infra *p2;
        arbre* root = NULL;
        while(fgets(ligne, sizeof(ligne), flux)){
            ligne[strcspn(ligne, "\r\n")] = '\0';
            strncpy(tmp, ligne, sizeof(tmp)-1);
            tmp[sizeof(tmp)-1] = '\0';
            root = ajouter_avl_flux(root, tmp);
        }
        afficherAVL(root,0);
        while(fgets(ligne, sizeof(ligne), fichier)){
            strcpy(tmp, ligne);
            type = detect_type(tmp);
            switch(type){
                case 0:
                    printf("end\n");
                    break;
                case 1:
                break;
                case 2:
                break;
                case 3:// stockage
                    printf("3\n");
                    p1 = createNode(ligne, 3);
                    afficher_noeud_fuites(p1);
                break;
                case 4://jonction
                    printf("4\n");
                    p1 = createNode(ligne, 4);
                    afficher_noeud_fuites(p1);
                break;
                case 5://service
                    printf("5\n");
                    p1 = createNode(ligne, 5);
                    afficher_noeud_fuites(p1);
                break;
                case 6://menage
                    printf("6\n");
                    p1 = createNode(ligne, 6);
                    afficher_noeud_fuites(p1);
                break;
            }
        }  
        if (flux) fclose(flux);
        if (fichier) fclose(fichier);
        return 0;

	}
	else {
		printf("Erreur: 'histo' ou 'leaks' attendu en 4e argument\n");
		return 1;
	}
}
