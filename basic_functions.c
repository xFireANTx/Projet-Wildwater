#include <stdio.h>
#include <stdlib.h>
#include "basic_functions.h"
#include "avl.h"
#include <string.h>

arbres_fuites *createNode(char *ligne, int type){
    infra *ancien = remplir_infra(ligne, type);
    if(ancien==NULL){exit(1);}
    arbres_fuites *nouveau = malloc(sizeof(arbres_fuites));
    if(nouveau == NULL){exit (1);}
    nouveau->structure = ancien;
    nouveau->premierf = NULL;
    nouveau->suivantf = NULL;
    return nouveau;
}

void addChild(arbres_fuites *parent, arbres_fuites *child){
    if (parent->premierf == NULL) {
        parent->premierf = child;
    } else {
        arbres_fuites *sibling = parent->premierf;
        while (sibling->suivantf != NULL) {
            sibling = sibling->suivantf;
        }
        sibling->suivantf = child;
    }
}



// Libération mémoire
void freeTree(arbres_fuites *node){
    if (node == NULL) return;
    freeTree(node->premierf);
    freeTree(node->suivantf);
    // `code_usine` is an array inside `infra`, don't free that separately.
    if (node->structure) free(node->structure);
    free(node);
}

infra *remplir_infra(char *line, int type){
    infra *new = malloc(sizeof(infra));
    if(new == NULL){ 
        printf("erreur d'allocation memoire");
        exit(1);
    }
    new->type = type;   
    char *col[5] = {0};
    int i = 0;
    char *piece = strtok(line, ";");
    while(piece && i < 5){
        col[i++] = piece;
        piece = strtok(NULL, ";");
    }
        if(type == 3){ //storage
        col[1] = strchr(col[1], '#');
        if (!col[1]) return 0;
        col[1]++;
        strncpy(new->code_usine, col[1], sizeof(new->code_usine)-1); //la premiere ccase d un storage est vide
        new->code_usine[sizeof(new->code_usine)-1] = '\0'; 

        strncpy(new->code_precedent, col[1], sizeof(new->code_precedent)-1);
        new->code_precedent[sizeof(new->code_precedent)-1] = '\0'; 

        col[2] = strchr(col[2], '#');
        if (!col[2]) return 0;
        col[2]++;
        strncpy(new->code_actuel, col[2], sizeof(new->code_actuel)-1);
        new->code_actuel[sizeof(new->code_actuel)-1] = '\0'; 
    }
    if(type == 4 || type == 5 || type == 6){ // jonction service cust
        col[0] = strchr(col[0], '#');
        if (!col[0]) return 0;
        col[0]++;
        strncpy(new->code_usine, col[0], sizeof(new->code_usine)-1);
        new->code_usine[sizeof(new->code_usine)-1] = '\0'; 

        col[1] = strchr(col[1], '#');
        if (!col[1]) return 0;
        col[1]++;
        strncpy(new->code_precedent, col[1], sizeof(new->code_precedent)-1);
        new->code_precedent[sizeof(new->code_precedent)-1] = '\0'; 

        col[2] = strchr(col[2], '#');
        if (!col[2]) return 0;
        col[2]++;
        strncpy(new->code_actuel, col[2], sizeof(new->code_actuel)-1);
        new->code_actuel[sizeof(new->code_actuel)-1] = '\0'; 
    }
    new->fuite = strtof(col[4], NULL);

    new->flux = 0;
    return new;
}

/*recup csv
junction = (nom #code usine (10);nom #code stockage(6);nom #code jonction(9);        ;fuite)
service =  (nom #code usine (10);nom #code jonction(9);nom #code service(9) ;        ;fuite)
menage =   (nom #code usine (10);nom #code service(9) ;nom #code menage(10) ;        ;fuite)
source =   (                    ;nom #code source(10) ;nom #code usine(10)  ;capa_max;fuite)
storage =  (                    ;nom #code usine (10) ;nom #code stockage(6);        ;fuite)
usine =    (                    ;nom #code usine (10) ;                     ;capa_max;     )*/

int empty(const char *s){
    return s == NULL || strcmp(s, "-") == 0;
}

int code_len(const char *s){
    const char *p = strchr(s, '#');
    if (!p) return 0;
    return strlen(p + 1);
}

int detect_type(char *line){
    char *col[5] = {0};
    int i = 0;

    char *piece = strtok(line, ";");
    while (piece && i < 5) {
        col[i++] = piece;
        piece = strtok(NULL, ";");
    }

    int l1 = empty(col[0]) ? 0 : code_len(col[0]);
    int l2 = empty(col[1]) ? 0 : code_len(col[1]);
    int l3 = empty(col[2]) ? 0 : code_len(col[2]);
    int c4 = !empty(col[3]);
    if(l1 == 0 && l2 == 9 && l3 == 9 && c4) //source
        return 1;

    if(l1 == 0 && l2 == 9 && l3 == 0 && c4) //usine
        return 2;

    if(l1 == 0 && l2 == 9 && l3 == 5) //storage
        return 3;

    if(l1 == 9 && l2 == 5 && l3 == 8) //jonction
        return 4;

    if(l1 == 9 && l2 == 8 && l3 == 9) //service
        return 5;

    if(l1 == 9 && l2 == 9 && l3 == 10) //menage
        return 6;
    return 0;
}



