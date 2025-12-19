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
    nouveau->suivant = NULL;
    nouveau->actuelf = NULL;
    return nouveau;
}

arbres_fuites *creer_parent(arbres_fuites *child){
    arbres_fuites *parent = malloc(sizeof(arbres_fuites));
    if (!parent) return NULL;
    parent->actuelf = child;
    parent->suivant = NULL;
    return parent;
}

arbres_fuites *addChildfuites(arbres_fuites *parent, arbres_fuites *child){
    if (parent == NULL || child == NULL) {
        fprintf(stderr, "addChildfuites: parent or child is NULL\n");
        return NULL;
    }
    // attach `child` under parent's actuelf list
    if (parent->actuelf == NULL) {
        parent->actuelf = child;
        child->suivant = NULL;
        return parent;
    }
    arbres_fuites *cur = parent->actuelf;
    while (cur->suivant != NULL) cur = cur->suivant;
    cur->suivant = child;
    child->suivant = NULL;
    return parent;
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

racine *ajouter_arbre_usine(racine *node, arbres_fuites *new){ // on suppose que on a deja compare et trouvé la bonne racine
    if (node == NULL) return NULL;
    if (new == NULL) return node;

    for (racine *r = node; r != NULL; r = r->suivant) {
        switch (new->structure->type) {
            case 3: { // storage: attach to r->actuelf where racine code matches precedent
                if (r->code_usine && strcmp(r->code_usine, new->structure->code_precedent) == 0) {
                    if (r->actuelf == NULL) {
                        r->actuelf = new;
                        new->suivant = NULL;
                    } else {
                        arbres_fuites *s = r->actuelf;
                        while (s->suivant) s = s->suivant;
                        s->suivant = new;
                        new->suivant = NULL;
                    }
                }
            } break;

            case 4: { // jonction: find matching storage nodes under this racine
                for (arbres_fuites *s = r->actuelf; s != NULL; s = s->suivant) {
                    if (s->structure && s->structure->code_actuel && strcmp(s->structure->code_actuel, new->structure->code_precedent) == 0) {
                        addChildfuites(s, new);
                    }
                }
            } break;

            case 5: { // service: storage -> jonction -> compare
                for (arbres_fuites *s = r->actuelf; s != NULL; s = s->suivant) {
                    for (arbres_fuites *j = s->actuelf; j != NULL; j = j->suivant) {
                        if (j->structure && j->structure->code_actuel && strcmp(j->structure->code_actuel, new->structure->code_precedent) == 0) {
                            addChildfuites(j, new);
                        }
                    }
                }
            } break;

            case 6: { // menage: storage -> jonction -> service -> compare
                for (arbres_fuites *s = r->actuelf; s != NULL; s = s->suivant) {
                    for (arbres_fuites *j = s->actuelf; j != NULL; j = j->suivant) {
                        for (arbres_fuites *svc = j->actuelf; svc != NULL; svc = svc->suivant) {
                            if (svc->structure && svc->structure->code_actuel && strcmp(svc->structure->code_actuel, new->structure->code_precedent) == 0) {
                                addChildfuites(svc, new);
                            }
                        }
                    }
                }
            } break;

            default:
                break;
        }
    }
    return node;
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



