#ifndef LECTURE_H
#define LECTURE_H

#include <stdio.h>
#include "structures.h"
#include "avl.h"

// Lit le fichier CSV et remplit les structures
// Retourne 0 si OK, 1 si erreur
int traiter_fichier(const char *chemin_fichier, NoeudAVL_Histo **racine_histo, Troncon **racine_fuites, NoeudAVL_Dico **dico_fuites);

// Fonction utilitaire pour parser une ligne (découpage point-virgule)
// Plus robuste que le code précédent
void parser_ligne(char *ligne, char *col1, char *col2, char *col3, char *col4, char *col5);

#endif
