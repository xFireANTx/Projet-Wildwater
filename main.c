#include <stdio.h>
#include <stdlib.h>
#include "basic_functions.h"
#include "avl.h"
#include <string.h>

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


void main(){
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
}


