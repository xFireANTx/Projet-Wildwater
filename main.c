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
//Argv: 0: ./exe  1:Projet-wildwater.dat 2:fichier entree 3:fichier sortie(recup par le bash) 4: histo/leaks 5:max/src/reel ou "Nom_usine #identifiant"
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
		FILE *histo_reel = fopen("temporaire_reel","w");//On met les valeurs reelles qui parviennent à l'usine de traitement 
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
		char *code_usine_recherché = NULL;
		code_usine_recherché = malloc(strlen(argv[5])+1);
		if(code_usine_recherché == NULL){
			printf("Erreur alloc memoire code_usine\n");
			exit(1);
		}
		strcpy(code_usine_recherché, argv[5]); 
		arbres_fuites *p1;              
		arbre *root = NULL; // premiere node avl usine
		infra *p2;
		while(fgets(ligne, sizeof(ligne), flux)){
		    ligne[strcspn(ligne, "\r\n")] = '\0';
		    strncpy(tmp, ligne, sizeof(tmp)-1);
		    tmp[sizeof(tmp)-1] = '\0';
		    root = ajouter_avl_flux(root, tmp);
		}
		
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
		            p1 = createNode(ligne, 3);
		            ajouter_arbre_usine(chercher_avl(p1->structure->code_usine, root), p1);
		        break;

		        case 4://jonction
		            p1 = createNode(ligne, 4);
		            ajouter_arbre_usine(chercher_avl(p1->structure->code_usine, root), p1);
		        break;

		        case 5://service
		            p1 = createNode(ligne, 5);
		            ajouter_arbre_usine(chercher_avl(p1->structure->code_usine, root), p1);
		        break;

		        case 6://menage
		            p1 = createNode(ligne, 6);
		            ajouter_arbre_usine(chercher_avl(p1->structure->code_usine, root), p1);
		        break;
		    }
		}  
		if (flux) fclose(flux);
		//Suppresion du fichier temporaire 
		if(remove("temporaire_reel") != 0){
   		perror("Erreur suppression temporaire_reel");
		} 
		if (fichier) fclose(fichier);
		float total_fuite;
		traverse_avl(root); // calcule les fuites une fois que toutes les infrastructures sont ajoutées
		if (!code_usine_recherché) { total_fuite = -1; }
		else{
			code_usine_recherché = strchr(code_usine_recherché, '#'); 
			if (code_usine_recherché) { code_usine_recherché++; }

			total_fuite = recuperer_fuites(chercher_avl(code_usine_recherché, root));
			
			
		}
		printf("Ajout\n");
		fprintf(sortie, "%s;%f\n",argv[5],total_fuite);
	}
	else {
		printf("Erreur: 'histo' ou 'leaks' attendu en 4e argument\n");
		return 1;
	}
	fclose(sortie);
	
}
