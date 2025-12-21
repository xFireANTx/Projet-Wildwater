#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "basic_functions.h"

static void print_tree_racine(racine *r){
    for (; r; r = r->suivant){
        printf("Racine %s flux=%.3f\n", r->code_usine, r->flux);
        for (arbres_fuites *s = r->actuelf; s; s = s->suivant){
            printf("  Storage fuite=%.2f flux=%.3f\n", s->structure ? s->structure->fuite : 0.0f, s->structure ? s->structure->flux : 0.0f);
            for (arbres_fuites *j = s->actuelf; j; j = j->suivant){
                printf("    Jonction fuite=%.2f flux=%.3f\n", j->structure ? j->structure->fuite : 0.0f, j->structure ? j->structure->flux : 0.0f);
                for (arbres_fuites *svc = j->actuelf; svc; svc = svc->suivant){
                    printf("      Service fuite=%.2f flux=%.3f\n", svc->structure ? svc->structure->fuite : 0.0f, svc->structure ? svc->structure->flux : 0.0f);
                    for (arbres_fuites *m = svc->actuelf; m; m = m->suivant){
                        printf("        Menage fuite=%.2f flux=%.3f\n", m->structure ? m->structure->fuite : 0.0f, m->structure ? m->structure->flux : 0.0f);
                    }
                }
            }
        }
    }
}

int main(void){
    // build a simple tree:
    // racine (flux=100) -> 2 storages
    // storage1 -> 1 jonction -> 1 service -> 1 menage
    // storage2 -> no children

    racine *r = malloc(sizeof(racine));
    if(!r) return 1;
    strcpy(r->code_usine, "RHTEST");
    r->flux = 100.0f;
    r->suivant = NULL;
    r->actuelf = NULL;

    // storage 1
    arbres_fuites *s1 = malloc(sizeof(arbres_fuites));
    s1->suivant = NULL;
    s1->actuelf = NULL;
    s1->structure = malloc(sizeof(infra));
    s1->structure->type = 3;
    strcpy(s1->structure->code_usine, "RHTEST");
    s1->structure->fuite = 10.0f; // 10%
    s1->structure->flux = 0.0f;

    // storage 2
    arbres_fuites *s2 = malloc(sizeof(arbres_fuites));
    s2->suivant = NULL;
    s2->actuelf = NULL;
    s2->structure = malloc(sizeof(infra));
    s2->structure->type = 3;
    strcpy(s2->structure->code_usine, "RHTEST");
    s2->structure->fuite = 0.0f; // 0%
    s2->structure->flux = 0.0f;

    // link storages
    r->actuelf = s1;
    s1->suivant = s2;

    // jonction under s1
    arbres_fuites *j1 = malloc(sizeof(arbres_fuites));
    j1->suivant = NULL;
    j1->actuelf = NULL;
    j1->structure = malloc(sizeof(infra));
    j1->structure->type = 4;
    j1->structure->fuite = 20.0f; // 20%
    j1->structure->flux = 0.0f;
    s1->actuelf = j1;

    // service under j1
    arbres_fuites *svc1 = malloc(sizeof(arbres_fuites));
    svc1->suivant = NULL;
    svc1->actuelf = NULL;
    svc1->structure = malloc(sizeof(infra));
    svc1->structure->type = 5;
    svc1->structure->fuite = 5.0f; // 5%
    svc1->structure->flux = 0.0f;
    j1->actuelf = svc1;

    // menage under svc1
    arbres_fuites *m1 = malloc(sizeof(arbres_fuites));
    m1->suivant = NULL;
    m1->actuelf = NULL;
    m1->structure = malloc(sizeof(infra));
    m1->structure->type = 6;
    m1->structure->fuite = 2.0f; // 2%
    m1->structure->flux = 0.0f;
    svc1->actuelf = m1;

    // before
    printf("Before calcule_fuites:\n");
    print_tree_racine(r);

    // run calculation
    calcule_fuites(r);

    // after
    printf("\nAfter calcule_fuites:\n");
    print_tree_racine(r);

    free_arbre_usine(r);
    return 0;
}
