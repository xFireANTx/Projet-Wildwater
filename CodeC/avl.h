#ifndef AVL_H
#define AVL_H

#include "structures.h"

// --- Fonctions de base  ---
int max(int a, int b);
int hauteur_histo(NoeudAVL_Histo *n);
int hauteur_dico(NoeudAVL_Dico *n);

// --- Gestion AVL Histogramme ---
NoeudAVL_Histo* creer_noeud_histo(char *id, long capacite);
NoeudAVL_Histo* rotation_gauche_histo(NoeudAVL_Histo *a);
NoeudAVL_Histo* rotation_droite_histo(NoeudAVL_Histo *a);
NoeudAVL_Histo* equilibrer_histo(NoeudAVL_Histo *a);
NoeudAVL_Histo* inserer_station(NoeudAVL_Histo *a, char *id, long capacite, int *h_change);
Station* rechercher_station(NoeudAVL_Histo *a, char *id);

// --- Gestion AVL Fuite ---
NoeudAVL_Dico* creer_noeud_dico(char *id, Troncon *ptr);
NoeudAVL_Dico* rotation_gauche_dico(NoeudAVL_Dico *a);
NoeudAVL_Dico* rotation_droite_dico(NoeudAVL_Dico *a);
NoeudAVL_Dico* equilibrer_dico(NoeudAVL_Dico *a);
NoeudAVL_Dico* inserer_dico(NoeudAVL_Dico *a, char *id, Troncon *ptr, int *h_change);
Troncon* rechercher_troncon(NoeudAVL_Dico *a, char *id);

// --- Fonctions de libération ---
void liberer_avl_histo(NoeudAVL_Histo *a);
void liberer_avl_dico(NoeudAVL_Dico *a);

#endif
