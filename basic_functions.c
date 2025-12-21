// basic_functions.c (version finale corrigée)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "basic_functions.h"
#include "avl.h"


arbres_fuites *createNode(char *ligne, int type) {
    infra *ancien = remplir_infra(ligne, type);
    if (ancien == NULL) {
        fprintf(stderr, "createNode: remplir_infra a échoué\n");
        exit(1);
    }

    arbres_fuites *nouveau = (arbres_fuites *)malloc(sizeof(arbres_fuites));
    if (nouveau == NULL) {
        fprintf(stderr, "createNode: erreur allocation memoire\n");
        exit(1);
    }

    nouveau->structure = ancien;
    nouveau->suivant = NULL;
    nouveau->actuelf = NULL;
    return nouveau;
}

arbres_fuites *addChildfuites(arbres_fuites *parent, arbres_fuites *child) {
    if (parent == NULL || child == NULL) {
        fprintf(stderr, "addChildfuites: parent ou child NULL\n");
        return parent;
    }

    for (arbres_fuites *cur = parent->actuelf; cur; cur = cur->suivant) {
        if (cur == child) {
            // déjà lié => éviter cycle
            return parent;
        }
    }

    child->suivant = parent->actuelf;
    parent->actuelf = child;
    return parent;
}


infra *remplir_infra(char *line, int type) {
    if (!line) return NULL;

    infra *newi = (infra *)malloc(sizeof(infra));
    if (newi == NULL) {
        fprintf(stderr, "remplir_infra: erreur allocation memoire\n");
        return NULL;
    }

    newi->type = type;
    newi->fuite = 0.0f;
    newi->flux  = 0.0f;
    newi->code_usine[0] = '\0';
    newi->code_precedent[0] = '\0';
    newi->code_actuel[0] = '\0';

    char *col[5] = {0};
    int i = 0;

    char *tmp = strdup(line);
    if (!tmp) { free(newi); return NULL; }

    char *piece = strtok(tmp, ";");
    while (piece && i < 5) {
        col[i++] = piece;
        piece = strtok(NULL, ";");
    }

    // garde minimale
    if (i < 3) { free(tmp); free(newi); return NULL; }

    if (type == 3) { // storage
        if (!col[1] || !col[2]) { free(tmp); free(newi); return NULL; }

        char *p1 = strchr(col[1], '#');
        if (!p1) { free(tmp); free(newi); return NULL; }
        p1++;

        strncpy(newi->code_usine, p1, CODE_SIZE - 1);
        newi->code_usine[CODE_SIZE - 1] = '\0';

        strncpy(newi->code_precedent, p1, CODE_SIZE - 1);
        newi->code_precedent[CODE_SIZE - 1] = '\0';

        char *p2 = strchr(col[2], '#');
        if (!p2) { free(tmp); free(newi); return NULL; }
        p2++;

        strncpy(newi->code_actuel, p2, CODE_SIZE - 1);
        newi->code_actuel[CODE_SIZE - 1] = '\0';
    }

    if (type == 4 || type == 5 || type == 6) { // jonction / service / menage
        if (!col[0] || !col[1] || !col[2]) { free(tmp); free(newi); return NULL; }

        char *p0 = strchr(col[0], '#');
        if (!p0) { free(tmp); free(newi); return NULL; }
        p0++;

        strncpy(newi->code_usine, p0, CODE_SIZE - 1);
        newi->code_usine[CODE_SIZE - 1] = '\0';

        char *p1 = strchr(col[1], '#');
        if (!p1) { free(tmp); free(newi); return NULL; }
        p1++;

        strncpy(newi->code_precedent, p1, CODE_SIZE - 1);
        newi->code_precedent[CODE_SIZE - 1] = '\0';

        char *p2 = strchr(col[2], '#');
        if (!p2) { free(tmp); free(newi); return NULL; }
        p2++;

        strncpy(newi->code_actuel, p2, CODE_SIZE - 1);
        newi->code_actuel[CODE_SIZE - 1] = '\0';
    }

    if (col[4] && strcmp(col[4], "-") != 0) {
        newi->fuite = strtof(col[4], NULL);
    }

    free(tmp);
    return newi;
}


racine *chercher_avl(const char *code_usine, arbre *root) {
    if (code_usine == NULL || root == NULL) return NULL;
    if (root->usine == NULL) return NULL;

    int cmp = strcmp(code_usine, root->usine->code_usine);
    if (cmp == 0) return root->usine;
    if (cmp < 0) return chercher_avl(code_usine, root->fg);
    return chercher_avl(code_usine, root->fd);
}

racine *ajouter_arbre_usine(racine *node, arbres_fuites *newn) {
    if (node == NULL || newn == NULL || newn->structure == NULL) return node;

    for (racine *r = node; r != NULL; r = r->suivant) {
        switch (newn->structure->type) {

            case 3: { // storage: parent = racine (usine)
                if (strcmp(r->code_usine, newn->structure->code_precedent) == 0) {
                    for (arbres_fuites *cur = r->actuelf; cur; cur = cur->suivant) {
                        if (cur == newn) return node; // déjà ajouté
                    }
                    newn->suivant = r->actuelf;
                    r->actuelf = newn;
                    return node; 
                }
            } break;

            case 4: { // jonction: parent = storage (dans r->actuelf)
                for (arbres_fuites *s = r->actuelf; s; s = s->suivant) {
                    if (s->structure &&
                        strcmp(s->structure->code_actuel, newn->structure->code_precedent) == 0) {
                        addChildfuites(s, newn);
                        return node; 
                    }
                }
            } break;

            case 5: { // service: parent = jonction
                for (arbres_fuites *s = r->actuelf; s; s = s->suivant) {
                    for (arbres_fuites *j = s->actuelf; j; j = j->suivant) {
                        if (j->structure &&
                            strcmp(j->structure->code_actuel, newn->structure->code_precedent) == 0) {
                            addChildfuites(j, newn);
                            return node; 
                        }
                    }
                }
            } break;

            case 6: { // menage: parent = service
                for (arbres_fuites *s = r->actuelf; s; s = s->suivant) {
                    for (arbres_fuites *j = s->actuelf; j; j = j->suivant) {
                        for (arbres_fuites *svc = j->actuelf; svc; svc = svc->suivant) {
                            if (svc->structure &&
                                strcmp(svc->structure->code_actuel, newn->structure->code_precedent) == 0) {
                                addChildfuites(svc, newn);
                                return node; 
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


int empty(const char *s) {
    return s == NULL || strcmp(s, "-") == 0;
}

int code_len(const char *s) {
    const char *p = strchr(s, '#');
    if (!p) return 0;
    return (int)strlen(p + 1);
}

int detect_type(char *line) {
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

float recuperer_fuites(racine *usine) {
    if (usine == NULL) return -1.0f;

    float total_fuite = 0.0f;

    // garde anti-cycle
    const size_t GUARD_LIMIT = 5000000;
    size_t steps = 0;

    for (arbres_fuites *s = usine->actuelf; s; s = s->suivant) {
        if (++steps > GUARD_LIMIT) { fprintf(stderr, "recuperer_fuites: garde anti-cycle\n"); break; }
        for (arbres_fuites *j = s->actuelf; j; j = j->suivant) {
            if (++steps > GUARD_LIMIT) { fprintf(stderr, "recuperer_fuites: garde anti-cycle\n"); break; }
            for (arbres_fuites *svc = j->actuelf; svc; svc = svc->suivant) {
                if (++steps > GUARD_LIMIT) { fprintf(stderr, "recuperer_fuites: garde anti-cycle\n"); break; }
                for (arbres_fuites *m = svc->actuelf; m; m = m->suivant) {
                    if (++steps > GUARD_LIMIT) { fprintf(stderr, "recuperer_fuites: garde anti-cycle\n"); break; }
                    if (m->structure) total_fuite += m->structure->fuite;
                }
            }
        }
    }

    return total_fuite;
}

void calcule_fuites(racine *usine) {
    if (!usine) return;

    const size_t GUARD_LIMIT = 8000000;
    size_t steps = 0;

    for (racine *r = usine; r; r = r->suivant) {
        if (++steps > GUARD_LIMIT) { fprintf(stderr, "calcule_fuites: garde anti-cycle racine\n"); break; }

        if (!r->actuelf) continue;

        float flux_r = r->flux;

        for (arbres_fuites *s = r->actuelf; s; s = s->suivant) {
            if (++steps > GUARD_LIMIT) { fprintf(stderr, "calcule_fuites: garde anti-cycle storage\n"); break; }
            if (s->structure) s->structure->flux = flux_r * (1.0f - s->structure->fuite / 100.0f);
            float flux_s = s->structure ? s->structure->flux : flux_r;

            for (arbres_fuites *j = s->actuelf; j; j = j->suivant) {
                if (++steps > GUARD_LIMIT) { fprintf(stderr, "calcule_fuites: garde anti-cycle jonction\n"); break; }
                if (j->structure) j->structure->flux = flux_s * (1.0f - j->structure->fuite / 100.0f);
                float flux_j = j->structure ? j->structure->flux : flux_s;

                for (arbres_fuites *svc = j->actuelf; svc; svc = svc->suivant) {
                    if (++steps > GUARD_LIMIT) { fprintf(stderr, "calcule_fuites: garde anti-cycle service\n"); break; }
                    if (svc->structure) svc->structure->flux = flux_j * (1.0f - svc->structure->fuite / 100.0f);
                    float flux_svc = svc->structure ? svc->structure->flux : flux_j;

                    for (arbres_fuites *m = svc->actuelf; m; m = m->suivant) {
                        if (++steps > GUARD_LIMIT) { fprintf(stderr, "calcule_fuites: garde anti-cycle menage\n"); break; }
                        if (m->structure) m->structure->flux = flux_svc * (1.0f - m->structure->fuite / 100.0f);
                    }
                }
            }
        }
    }
}

void traverse_avl(arbre *root) {
    if (root == NULL) return;
    calcule_fuites(root->usine);
}
