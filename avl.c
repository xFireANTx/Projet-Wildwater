#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "basic_functions.h"
#include "avl.h"

int max_int(int a, int b) {
    return (a > b) ? a : b;
}

static int parse_code_flux(const char *ligne, char out_code[CODE_SIZE], float *out_flux) {
    if (!ligne || !out_code || !out_flux) return 0;

    out_code[0] = '\0';
    *out_flux = 0.0f;

    char *tmp = strdup(ligne);
    if (!tmp) return 0;

    char *saveptr = NULL;
    char *col0 = strtok_r(tmp, ";", &saveptr);
    char *col1 = strtok_r(NULL, ";", &saveptr);

    if (!col0) { free(tmp); return 0; }

    char *p = strchr(col0, '#');
    if (!p || !*(p + 1)) { free(tmp); return 0; }
    p++; 

    strncpy(out_code, p, CODE_SIZE - 1);
    out_code[CODE_SIZE - 1] = '\0';

    if (col1 && strcmp(col1, "-") != 0) {
        *out_flux = strtof(col1, NULL);
    }

    free(tmp);
    return 1;
}

int hauteur(arbre *n) {
    return (n == NULL) ? 0 : n->hauteur;
}

int facteur_equilibre(arbre *n) {
    if (n == NULL) return 0;
    return hauteur(n->fg) - hauteur(n->fd);
}

arbre *rotation_droite(arbre *a) {
    if (a == NULL || a->fg == NULL) return a;

    arbre *b = a->fg;
    arbre *temp = b->fd;

    b->fd = a;
    a->fg = temp;

    a->hauteur = 1 + max_int(hauteur(a->fg), hauteur(a->fd));
    b->hauteur = 1 + max_int(hauteur(b->fg), hauteur(b->fd));

    return b;
}

arbre *rotation_gauche(arbre *a) {
    if (a == NULL || a->fd == NULL) return a;

    arbre *b = a->fd;
    arbre *temp = b->fg;

    b->fg = a;
    a->fd = temp;

    a->hauteur = 1 + max_int(hauteur(a->fg), hauteur(a->fd));
    b->hauteur = 1 + max_int(hauteur(b->fg), hauteur(b->fd));

    return b;
}

arbre *rotation_droite_gauche(arbre *a) {
    if (!a) return a;
    a->fd = rotation_droite(a->fd);
    return rotation_gauche(a);
}

arbre *rotation_gauche_droite(arbre *a) {
    if (!a) return a;
    a->fg = rotation_gauche(a->fg);
    return rotation_droite(a);
}

arbre *equilibrer(arbre *n) {
    if (n == NULL) return NULL;

    int balance = facteur_equilibre(n);

    if (balance > 1 && facteur_equilibre(n->fg) >= 0) {
        return rotation_droite(n);
    }

    if (balance > 1 && facteur_equilibre(n->fg) < 0) {
        return rotation_gauche_droite(n);
    }

    if (balance < -1 && facteur_equilibre(n->fd) <= 0) {
        return rotation_gauche(n);
    }

    if (balance < -1 && facteur_equilibre(n->fd) > 0) {
        return rotation_droite_gauche(n);
    }

    return n; 
}


racine *remplir_racine(char *ligne) {
    if (!ligne) return NULL;

    racine *nouveau = (racine *)malloc(sizeof(racine));
    if (nouveau == NULL) { exit(123); }

    nouveau->code_usine[0] = '\0';
    nouveau->flux = 0.0f;
    nouveau->actuelf = NULL;
    nouveau->suivant = NULL;

    char code[CODE_SIZE];
    float flux = 0.0f;

    if (!parse_code_flux(ligne, code, &flux)) {
        free(nouveau);
        return NULL;
    }

    strncpy(nouveau->code_usine, code, CODE_SIZE - 1);
    nouveau->code_usine[CODE_SIZE - 1] = '\0';
    nouveau->flux = flux;

    return nouveau;
}

arbre *creer_noeud_arbre(char *ligne) {
    if (!ligne) return NULL;

    arbre *nouveau = (arbre *)malloc(sizeof(arbre));
    if (nouveau == NULL) { exit(123); }

    nouveau->usine = remplir_racine(ligne);
    if (!nouveau->usine) {
        free(nouveau);
        return NULL;
    }

    nouveau->fg = NULL;
    nouveau->fd = NULL;
    nouveau->hauteur = 1;

    return nouveau;
}

arbre *ajouter_avl_flux(arbre *node, char *ligne) {
    if (!ligne) return node;

    char code[CODE_SIZE];
    float flux = 0.0f;

    if (!parse_code_flux(ligne, code, &flux)) {
        return node;
    }


    if (node == NULL) {
        arbre *n = creer_noeud_arbre(ligne);
        return n; 
    }

    int cmp = strcmp(code, node->usine->code_usine);

    if (cmp == 0) {
        node->usine->flux = flux;
        return node;
    } else if (cmp < 0) {
        node->fg = ajouter_avl_flux(node->fg, ligne);
    } else {
        node->fd = ajouter_avl_flux(node->fd, ligne);
    }

    node->hauteur = 1 + max_int(hauteur(node->fg), hauteur(node->fd));
    return equilibrer(node);
}
