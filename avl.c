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

racine *remplir_racine(char *ligne){
    printf("le");
    racine *nouveau = malloc(sizeof(racine));
    if (!nouveau) return NULL;
        char *col[2] = {0};
    int i = 0;
    char *tmp = ligne;
    char *piece = strtok(tmp, ";");
    printf("boubou");
    while(piece && i < 2){
        col[i++] = piece;
        piece = strtok(NULL, ";");
    }
    col[0] = strchr(col[0], '#');
    if (!col[0]) return 0;
    col[0]++;
    strncpy(nouveau->code_usine, col[0], sizeof(nouveau->code_usine)-1);
    nouveau->code_usine[sizeof(nouveau->code_usine) - 1] = '\0';            //code usine apres le #
    printf("dou");
    nouveau->flux = strtof(col[1], NULL);
    printf("la?\n");
    nouveau->premierf = NULL;
    nouveau->suivantf = NULL;
    return nouveau;
}

arbre *creer_noeud_arbre(char *ligne){
    arbre *nouveau = malloc(sizeof(arbre));
    if (!nouveau) return NULL;

    nouveau->usine = remplir_racine(ligne);
    if (!nouveau->usine) {
        free(nouveau);
        return NULL;
    }

    nouveau->fg = NULL;
    nouveau->fd = NULL;

    return nouveau;
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

arbre *ajouter_avl_flux(arbre *node, char *ligne){
    char *temp = ligne;
    arbre *nouveau = creer_noeud_arbre(temp);
    if(nouveau == NULL){
        printf("pas nouveau");exit (1);}
    if (node == NULL) {
        return nouveau;
    }

    if(strcmp(nouveau->usine->code_usine, node->usine->code_usine) < 0){
        node->fg = ajouter_avl_flux(node->fg, ligne);
    }
    else if(strcmp(nouveau->usine->code_usine, node->usine->code_usine) > 0){
        node->fd = ajouter_avl_flux(node->fd, ligne);
    }
    else{
        return node; // pas de doublons
    }
    return equilibrer(node);
}


