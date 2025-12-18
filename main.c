#include <stdio.h>
#include <stdlib.h>
#include "basic_functions.h"
#include <string.h>
#include <stdio.h>

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
    printf("  capa_max       : %d\n", i->capa_max);
    printf("  fuite          : %.3f\n", i->fuite);
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
    int boucle_principale = 0;   int type = 0;
    char line[256];  char tmp[256];
    //long pos = ftell(fichier);
    arbres_fuites *p1;
    infra *p2;
    while(fgets(line, sizeof(line), fichier)){
        strcpy(tmp, line); // strtok modifie la ligne
        int type = detect_type(tmp);
        //fseek(fichier, pos, SEEK_CUR);          //retourne au debut de la ligne
        switch(type){
            case 0:
                printf("end\n");
                return;
            case 1: // source pas utile pour les fuites
                break;
            case 2:// usine
                printf("2\n");
                p2 = remplir_infra(line, 2);
                afficher_infra(p2);
                //p1 = createNode(remplir_infra(line, 2));
                //afficher_noeud_fuites(p1);
            break;
            case 3:// stockage
                printf("3\n");
                p2 = remplir_infra(line, 3);
                afficher_infra(p2);
            break;
            case 4://jonction
                printf("4\n");
                p1 = createNode(remplir_infra(line, 4));
                afficher_noeud_fuites(p1);
            break;
            case 5://service
                printf("5\n");
                p2 = remplir_infra(line, 5);
                afficher_infra(p2);
            break;
            case 6://menage
                printf("6\n");
                p2 = remplir_infra(line, 6);
                afficher_infra(p2);
            break;
        }
    }    
    fclose(fichier);
}


