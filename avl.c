#include <stdio.h>
#include <stdlib.h>
#include "basic_functions.h"
#include "avl.h"
#include <string.h>

//arbres
arbre *rotation_droite(arbre *a){
    arbre *b = a->fg;
    arbre *temp = b->fd;
    b->fd = a;
    a->fg = temp;
    return b;
}


arbre *rotation_gauche(arbre* a){
    arbre *b = a->fd;
    arbre *temp = b->fg;
    b->fg = a;
    a->fd = temp;
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

int hauteur(arbre *n){
    if (n == NULL)
        return 0;

    int hg = hauteur(n->fg);
    int hd = hauteur(n->fd);

    return 1 + (hg > hd ? hg : hd);
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

arbre *ajouter_avl(arbre *node, char *code_usine[11]){

    if (node == NULL) {
        arbre *nouveau = malloc(sizeof(arbre));
        if (!nouveau) return NULL; 
        strncpy(nouveau->usine->code_usine, code_usine, sizeof(nouveau->usine->code_usine) - 1);
        nouveau->usine->code_usine[sizeof(nouveau->usine->code_usine) - 1] = '\0';  
        nouveau->fg = nouveau->fd = NULL;
        return nouveau;
    }

    if(strcmp(code_usine, node->usine->code_usine) < 0){
        node->fg = ajouter_avl(node->fg, code_usine);
    }
    else if(strcmp(code_usine, node->usine->code_usine) > 0){
        node->fd = ajouter_avl(node->fd, code_usine);
    }
    else{
        return node; // pas de doublons
    }
    return equilibrer(node);
}


