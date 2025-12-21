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
    if (!line) return NULL;
    infra *new = malloc(sizeof(infra));
    if(new == NULL){ 
        fprintf(stderr, "erreur d'allocation memoire\n");
        return NULL;
    }
    new->type = type;   
    char *col[5] = {0};
    int i = 0;
    char *tmp = strdup(line);
    if (!tmp) { free(new); return NULL; }
    char *piece = strtok(tmp, ";");
    while(piece && i < 5){
        col[i++] = piece;
        piece = strtok(NULL, ";");
    }
        if(type == 3){ //storage
        col[1] = strchr(col[1], '#');
        if (!col[1]) { free(new); free(tmp); return NULL; }
        col[1]++;
        strncpy(new->code_usine, col[1], sizeof(new->code_usine)-1); //la premiere ccase d un storage est vide
        new->code_usine[sizeof(new->code_usine)-1] = '\0'; 

        strncpy(new->code_precedent, col[1], sizeof(new->code_precedent)-1);
        new->code_precedent[sizeof(new->code_precedent)-1] = '\0'; 

        col[2] = strchr(col[2], '#');
        if (!col[2]) { free(new); free(tmp); return NULL; }
        col[2]++;
        strncpy(new->code_actuel, col[2], sizeof(new->code_actuel)-1);
        new->code_actuel[sizeof(new->code_actuel)-1] = '\0'; 
    }                                                                   
    if(type == 4 || type == 5 || type == 6){ // jonction || service || cust
        col[0] = strchr(col[0], '#');
        if (!col[0]) { free(new); free(tmp); return NULL; }
        col[0]++;
        strncpy(new->code_usine, col[0], sizeof(new->code_usine)-1);
        new->code_usine[sizeof(new->code_usine)-1] = '\0'; 

        col[1] = strchr(col[1], '#');
        if (!col[1]) { free(new); free(tmp); return NULL; }
        col[1]++;
        strncpy(new->code_precedent, col[1], sizeof(new->code_precedent)-1);
        new->code_precedent[sizeof(new->code_precedent)-1] = '\0'; 

        col[2] = strchr(col[2], '#');
        if (!col[2]) { free(new); free(tmp); return NULL; }
        col[2]++;
        strncpy(new->code_actuel, col[2], sizeof(new->code_actuel)-1);
        new->code_actuel[sizeof(new->code_actuel)-1] = '\0'; 
    }
    new->fuite = col[4] ? strtof(col[4], NULL) : 0.0f;

    new->flux = 0;
    free(tmp);
    return new;
}

racine *chercher_avl(const char *code_usine, arbre *root){
    if (code_usine == NULL || root == NULL) return NULL;
    if (root->usine == NULL) return NULL;
    int cmp = strcmp(code_usine, root->usine->code_usine);
    if (cmp == 0) return root->usine;
    if (cmp < 0) return chercher_avl(code_usine, root->fg);
    return chercher_avl(code_usine, root->fd);
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
    if (!line) return 0;
    char *col[5] = {0};
    int i = 0;
    char *tmp = strdup(line);
    if (!tmp) return 0;
    char *piece = strtok(tmp, ";");
    while (piece && i < 5) {
        col[i++] = piece;
        piece = strtok(NULL, ";");
    }

    int l1 = empty(col[0]) ? 0 : code_len(col[0]);
    int l2 = empty(col[1]) ? 0 : code_len(col[1]);
    int l3 = empty(col[2]) ? 0 : code_len(col[2]);
    int c4 = !empty(col[3]);
    int result = 0;
    if(l1 == 0 && l2 == 9 && l3 == 9 && c4) //source
        result = 1;
    else if(l1 == 0 && l2 == 9 && l3 == 0 && c4) //usine
        result = 2;
    else if(l1 == 0 && l2 == 9 && l3 == 5) //storage
        result = 3;
    else if(l1 == 9 && l2 == 5 && l3 == 8) //jonction
        result = 4;
    else if(l1 == 9 && l2 == 8 && l3 == 9) //service
        result = 5;
    else if(l1 == 9 && l2 == 9 && l3 == 10) //menage
        result = 6;
    free(tmp);
    return result;
}

//recup fuites pour toutes les usines
//recup fuites pour une usine donnee

float recuperer_fuites(racine *usine){
    if(usine == NULL) return -1;
    float total_fuite = 0;
    for (arbres_fuites *s = usine->actuelf; s != NULL; s = s->suivant) {
        for (arbres_fuites *j = s->actuelf; j != NULL; j = j->suivant) {
            for (arbres_fuites *svc = j->actuelf; svc != NULL; svc = svc->suivant) {
                for (arbres_fuites *m = svc->actuelf; m != NULL; m = m->suivant) {
                    total_fuite += m->structure->fuite;
                }
            }
        }
    }
    return total_fuite;
}

void calcule_fuites(racine *usine){
    if (!usine) return;
    int nombre_enfant_usine = 0;
    int nombre_enfant_storage = 0;
    int nombre_enfant_jonction = 0;
    int nombre_enfant_service = 0;
    int nombre_enfant_menage = 0;
    for (racine *r = usine; r; r = r->suivant) {
        nombre_enfant_usine++;
        if (!r->actuelf) continue;
        for (arbres_fuites *s = r->actuelf; s; s = s->suivant) {
            nombre_enfant_storage++;
            for (arbres_fuites *j = s->actuelf; j; j = j->suivant) {
                nombre_enfant_jonction++;
                for (arbres_fuites *svc = j->actuelf; svc; svc = svc->suivant) {
                    nombre_enfant_service++;
                    for (arbres_fuites *m = svc->actuelf; m; m = m->suivant) {
                        nombre_enfant_menage++;
                    }
                    for(arbres_fuites *m = svc->actuelf; m; m = m->suivant){
                        m->actuelf->structure->flux = (m->structure->flux/nombre_enfant_menage) * (1-(m->actuelf->structure->fuite / 100.0f));
                        m = m->suivant;
                    }
                }
                for(arbres_fuites *svc = j->actuelf; svc; svc = svc->suivant){
                    svc->actuelf->structure->flux = (svc->structure->flux/nombre_enfant_service) * (1-(svc->actuelf->structure->fuite / 100.0f));
                    svc = svc->suivant;
                }
            }
            for(arbres_fuites *j = s->actuelf; j; j = j->suivant){
                j->actuelf->structure->flux = (j->structure->flux/nombre_enfant_jonction) * (1-(j->actuelf->structure->fuite / 100.0f));
                j = j->suivant;
            }
        }
        for(arbres_fuites *s = r->actuelf; s; s = s->suivant){
            s->actuelf->structure->flux = (s->structure->flux/nombre_enfant_storage) * (1-(s->actuelf->structure->fuite / 100.0f));
            s = s->suivant;    
        }
    }
    for(racine *r = usine; r; r = r->suivant){
        r->actuelf->structure->flux = (r->flux/nombre_enfant_usine) * (1-(r->actuelf->structure->fuite / 100.0f));
        r = r->suivant;

    }
}

// In-order traversal of the AVL. Calls `visit` for each `racine` node.
void traverse_avl(arbre *root){
    if (root == NULL) return;
    calcule_fuites(root->usine);
    traverse_avl(root->fg);
    traverse_avl(root->fd);
}

void free_arbre_usine(racine *root){
        if (!root) return;
    int nombre_enfant_usine = 0;
    int nombre_enfant_storage = 0;
    int nombre_enfant_jonction = 0;
    int nombre_enfant_service = 0;
    int nombre_enfant_menage = 0;
    for (racine *r = root; r; r = r->suivant) {
        nombre_enfant_usine++;
        if (!r->actuelf) continue;
        for (arbres_fuites *s = r->actuelf; s; s = s->suivant) {
            nombre_enfant_storage++;
            for (arbres_fuites *j = s->actuelf; j; j = j->suivant) {
                nombre_enfant_jonction++;
                for (arbres_fuites *svc = j->actuelf; svc; svc = svc->suivant) {
                    nombre_enfant_service++;
                    for (arbres_fuites *m = svc->actuelf; m; m = m->suivant) {
                        nombre_enfant_menage++;
                    }
                }
            }
        }
    }
    for(racine *r = root; r; r = r->suivant){
        racine *temp_racine = r;
        r = r->suivant;
        free(temp_racine);
        for(arbres_fuites *s = r->actuelf; s; s = s->suivant){
            arbres_fuites *temp_storage = s;
            s = s->suivant;
            free(temp_storage);    
            for(arbres_fuites *j = s->actuelf; j; j = j->suivant){
                arbres_fuites *temp_jonction = j;
                j = j->suivant;
                free(temp_jonction);
                for(arbres_fuites *svc = j->actuelf; svc; svc = svc->suivant){
                    arbres_fuites *temp_service = svc;
                    svc = svc->suivant;
                    free(temp_service);
                    for(arbres_fuites *m = svc->actuelf; m; m = m->suivant){
                        arbres_fuites *temp_menage = m;
                        m = m->suivant;
                        free(temp_menage);
                    }
                }
            }
        }
    }
}
