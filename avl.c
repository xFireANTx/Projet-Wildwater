#include <stdio.h>
#include <stdlib.h>
#include "basic_functions.h"
#include "avl.h"
#include <string.h>

//arbres
int max(int a, int b) {
    return (a > b) ? a : b;
}

arbre *rotation_droite(arbre *a) {
    arbre *b = a->fg;
    arbre *temp = b->fd;

    b->fd = a;
    a->fg = temp;

    a->hauteur = 1 + max(hauteur(a->fg), hauteur(a->fd));
    b->hauteur = 1 + max(hauteur(b->fg), hauteur(b->fd));

    return b;
}


arbre *rotation_gauche(arbre *a) {
    arbre *b = a->fd;
    arbre *temp = b->fg;

    b->fg = a;
    a->fd = temp;

    a->hauteur = 1 + max(hauteur(a->fg), hauteur(a->fd));
    b->hauteur = 1 + max(hauteur(b->fg), hauteur(b->fd));

    return b;
}

arbre *rotation_droite_gauche(arbre *a){
    a->fd = rotation_droite(a->fd);
    return rotation_gauche(a);
}

arbre *rotation_gauche_droite(arbre *a){
    a->fg = rotation_gauche(a->fg);
    return rotation_droite(a);
}

racine *remplir_racine(char *ligne){
    racine *nouveau = malloc(sizeof(racine));
    if (nouveau == NULL){exit (123);}
    char *col[2] = {0};
    int i = 0;
    char *tmp = strdup(ligne);
    if (!tmp) { free(nouveau); return NULL; }
    char *piece = strtok(tmp, ";");
    while(piece && i < 2){
        col[i++] = piece;
        piece = strtok(NULL, ";");
    }
    if (!col[0]) { free(tmp); free(nouveau); return NULL; }
    char *p = strchr(col[0], '#');
    if (!p) { free(tmp); free(nouveau); return NULL; }
    p++;
    strncpy(nouveau->code_usine, p, sizeof(nouveau->code_usine)-1);
    nouveau->code_usine[sizeof(nouveau->code_usine) - 1] = '\0';            //code usine apres le #
    nouveau->flux = col[1] ? strtof(col[1], NULL) : 0.0f;
    nouveau->actuelf = NULL;
    nouveau->suivant = NULL;
    free(tmp);
    return nouveau;
}

arbre *creer_noeud_arbre(char *ligne){
    arbre *nouveau = malloc(sizeof(arbre));
    if (nouveau == NULL){ exit(123);};
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


int hauteur(arbre *n) {
    if(n == NULL) return 0;
    return n->hauteur;
}

int facteur_equilibre(arbre *n){
    if (n == NULL)
        return 0;

    return hauteur(n->fg) - hauteur(n->fd);
}

arbre *equilibrer(arbre *n){
    if (n == NULL)
        return NULL;
    int balance = facteur_equilibre(n);

    if (balance > 1 && facteur_equilibre(n->fg) >= 0){
        return rotation_droite(n);
    }

    if (balance > 1 && facteur_equilibre(n->fg) < 0){
    return rotation_gauche_droite(n);
    }

    if (balance < -1 && facteur_equilibre(n->fd) <= 0){
        return rotation_gauche(n);
    }
    if (balance < -1 && facteur_equilibre(n->fd) > 0){
        return rotation_droite_gauche(n);
    }

    return n; // déjà équilibré
}

arbre *ajouter_avl_flux(arbre *node, char *ligne){
    if (node == NULL) {
    	arbre *n = creer_noeud_arbre(ligne);
    	n->hauteur = 1;
        return n;  
    }
    int i = 0;
    char *col[2] = {NULL, NULL};
    char *saveptr = NULL;
    char *piece = strtok_r(ligne, ";", &saveptr);
    while (piece && i < 2) {
        col[i++] = piece;
        piece = strtok_r(NULL, ";", &saveptr);
    }
    if (!col[0]) return node;
    char *p = strchr(col[0], '#');
    if (!p) return node;
    p++;
    char code[CODE_SIZE];
    strncpy(code, p, sizeof(code) - 1);
    code[sizeof(code) - 1] = '\0';
    if (strcmp(code, node->usine->code_usine) == 0) {
        if (col[1]) node->usine->flux = strtof(col[1], NULL);
        return node;
    } else if (strcmp(code, node->usine->code_usine) < 0) {
        node->fg = ajouter_avl_flux(node->fg, ligne);
    } else {
        node->fd = ajouter_avl_flux(node->fd, ligne);
    }
    node->hauteur = 1 + max(hauteur(node->fg), hauteur(node->fd));
    return equilibrer(node);
}



