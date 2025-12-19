#ifndef AVL_H_  
#define AVL_H_
#include <stdio.h>
#include <stdlib.h>
#include "basic_functions.h"
#include <string.h>

typedef struct Infra infra;
typedef struct Arbres_fuites arbres_fuites;
typedef struct Racine racine;
typedef struct Arbre arbre;

arbre *rotation_droite(arbre* a);
arbre *rotation_gauche(arbre* a);
arbre *rotation_droite_gauche(arbre *a);
arbre *rotation_gauche_droite(arbre *a);
int hauteur(arbre *n);
int facteur_equilibre(arbre *n);
arbre *equilibrer(arbre *n);
racine *remplir_racine(char *ligne);
arbre *creer_noeud_arbre(char *ligne);
arbre *ajouter_avl_flux(arbre *node, char *ligne);


#endif 