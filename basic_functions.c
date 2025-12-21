#include <stdio.h>
#include <stdlib.h>
#include "basic_functions.h"
#include "avl.h"
#include <string.h>

// simple block pool to reduce malloc/free overhead
#define POOL_BLOCK 256
typedef struct InfraBlock {
    struct InfraBlock *next;
    infra items[POOL_BLOCK];
    size_t next_idx;
} InfraBlock;

typedef struct ArbresBlock {
    struct ArbresBlock *next;
    arbres_fuites items[POOL_BLOCK];
    size_t next_idx;
} ArbresBlock;

static InfraBlock *infra_blocks = NULL;
static ArbresBlock *arbres_blocks = NULL;

static infra *alloc_infra(void){
    if (!infra_blocks || infra_blocks->next_idx >= POOL_BLOCK){
        InfraBlock *b = malloc(sizeof(InfraBlock));
        if (!b) return NULL;
        b->next = infra_blocks;
        b->next_idx = 0;
        infra_blocks = b;
    }
    return &infra_blocks->items[infra_blocks->next_idx++];
}

static arbres_fuites *alloc_arbres_fuites(void){
    if (!arbres_blocks || arbres_blocks->next_idx >= POOL_BLOCK){
        ArbresBlock *b = malloc(sizeof(ArbresBlock));
        if (!b) return NULL;
        b->next = arbres_blocks;
        b->next_idx = 0;
        arbres_blocks = b;
    }
    return &arbres_blocks->items[arbres_blocks->next_idx++];
}

arbres_fuites *createNode(char *ligne, int type){
    infra *ancien = remplir_infra(ligne, type);
    if (ancien == NULL) { return NULL; }
    arbres_fuites *nouveau = alloc_arbres_fuites();
    if (nouveau == NULL) { return NULL; }
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
    child->suivant = parent->actuelf;
    parent->actuelf = child;
    return parent;
}

infra *remplir_infra(char *line, int type){
    if (!line) return NULL;
    infra *new = alloc_infra();
    if (new == NULL){ 
        fprintf(stderr, "erreur d'allocation memoire infra\n");
        return NULL;
    }
    new->type = type;
    char *col[5] = {0};
    int i = 0;
    char *saveptr = NULL;
    char *piece = strtok_r(line, ";", &saveptr);
    while(piece && i < 5){
        col[i++] = piece;
        piece = strtok_r(NULL, ";", &saveptr);
    }
    if(type == 3){ //storage
        if (!col[1]) return NULL;
        char *q = strchr(col[1], '#');
        if (!q) return NULL;
        q++;
        memcpy(new->code_usine, q, CODE_SIZE-1);
        new->code_usine[CODE_SIZE-1] = '\0';
        memcpy(new->code_precedent, q, CODE_SIZE-1);
        new->code_precedent[CODE_SIZE-1] = '\0';

        if (!col[2]) return NULL;
        q = strchr(col[2], '#');
        if (!q) return NULL;
        q++;
        memcpy(new->code_actuel, q, CODE_SIZE-1);
        new->code_actuel[CODE_SIZE-1] = '\0';
    }
    if(type == 4 || type == 5 || type == 6){ // jonction || service || cust
        if (!col[0] || !col[1] || !col[2]) return NULL;
        char *q = strchr(col[0], '#');
        if (!q) return NULL; q++;
        memcpy(new->code_usine, q, CODE_SIZE-1); new->code_usine[CODE_SIZE-1] = '\0';
        q = strchr(col[1], '#'); if (!q) return NULL; q++;
        memcpy(new->code_precedent, q, CODE_SIZE-1); new->code_precedent[CODE_SIZE-1] = '\0';
        q = strchr(col[2], '#'); if (!q) return NULL; q++;
        memcpy(new->code_actuel, q, CODE_SIZE-1); new->code_actuel[CODE_SIZE-1] = '\0';
    }
    new->fuite = col[4] ? strtof(col[4], NULL) : 0.0f;
    new->flux = 0;
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
    char *saveptr = NULL;
    char *piece = strtok_r(line, ";", &saveptr);
    while (piece && i < 5) {
        col[i++] = piece;
        piece = strtok_r(NULL, ";", &saveptr);
    }

    int l1 = empty(col[0]) ? 0 : code_len(col[0]);
    int l2 = empty(col[1]) ? 0 : code_len(col[1]);
    int l3 = empty(col[2]) ? 0 : code_len(col[2]);
    int c4 = !empty(col[3]);
    if (l1 == 0 && l2 == 9 && l3 == 9 && c4) return 1;
    if (l1 == 0 && l2 == 9 && l3 == 0 && c4) return 2;
    if (l1 == 0 && l2 == 9 && l3 == 5) return 3;
    if (l1 == 9 && l2 == 5 && l3 == 8) return 4;
    if (l1 == 9 && l2 == 8 && l3 == 9) return 5;
    if (l1 == 9 && l2 == 9 && l3 == 10) return 6;
    return 0;
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
    for (racine *r = usine; r; r = r->suivant) {
        if (!r->actuelf) continue;
        float flux_r = r->flux;
        for (arbres_fuites *s = r->actuelf; s; s = s->suivant) {
            if (s->structure) s->structure->flux = flux_r * (1.0f - s->structure->fuite / 100.0f);
            float flux_s = s->structure ? s->structure->flux : flux_r;
            for (arbres_fuites *j = s->actuelf; j; j = j->suivant) {
                if (j->structure) j->structure->flux = flux_s * (1.0f - j->structure->fuite / 100.0f);
                float flux_j = j->structure ? j->structure->flux : flux_s;
                for (arbres_fuites *svc = j->actuelf; svc; svc = svc->suivant) {
                    if (svc->structure) svc->structure->flux = flux_j * (1.0f - svc->structure->fuite / 100.0f);
                    float flux_svc = svc->structure ? svc->structure->flux : flux_j;
                    for (arbres_fuites *m = svc->actuelf; m; m = m->suivant) {
                        if (m->structure) m->structure->flux = flux_svc * (1.0f - m->structure->fuite / 100.0f);
                    }
                }
            }
        }
    }
}

// In-order traversal of the AVL. Calls `visit` for each `racine` node.
void traverse_avl(arbre *root){
    if (root == NULL) return;
    calcule_fuites(root->usine);
    traverse_avl(root->fg);
    traverse_avl(root->fd);
}


