#ifndef LECTURE_H
#define LECTURE_H

#include <stdio.h>
#include "structure.h"
#include "avl.h"

// Lit le fichier CSV et remplit les structures
// Retourne 0 si OK, 1 si erreur
int traiter_fichier(const char *chemin_fichier, NoeudAVL_Histo **racine_histo, Troncon **racine_fuites, NoeudAVL_Dico **dico_fuites);

// Fonction utilitaire pour parser une ligne (découpage point-virgule)
// Plus robuste que le code précédent
void recuperer_colonne(char *ligne, int num_colonne, char *buffer);
#endif
