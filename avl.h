#ifndef AVL_H_  
#define AVL_H_
#include <stdio.h>
#include <stdlib.h>
#include "basic_functions.h"
#include <string.h>

arbre *rotation_droite(arbre* a);
arbre *rotation_gauche(arbre* a);
arbre *rotation_droite_gauche(arbre *a);
arbre *rotation_gauche_droite(arbre *a);
int hauteur(arbre *n);
int facteur_equilibre(arbre *n);
arbre *equilibrer(arbre *n);
arbre *ajouter_avl(arbre *node, char *code_usine[11]);


#endif 