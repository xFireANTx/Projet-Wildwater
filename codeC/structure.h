#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- STRUCTURES POUR L'HISTOGRAMME ---

typedef struct Station {
    char id[256];       // Taille augmentée pour les noms longs comme "Facility complex..."
    long capacite;      // Capacité (Max)
    long vol_source;    // Volume Source (Src)
    long vol_traite;    // Volume Traité (Real)
} Station;

typedef struct NoeudAVL_Histo {
    Station *data;
    struct NoeudAVL_Histo *fg;
    struct NoeudAVL_Histo *fd;
    int equilibre;
} NoeudAVL_Histo;


// --- STRUCTURES POUR LES FUITES (Arbre N-aire) ---

typedef struct Troncon {
    char id[256];
    float pourcentage_fuite;
    long volume;                // AJOUT INDISPENSABLE pour stocker le flux lu en col 4
    struct Troncon *frere;
    struct Troncon *fils;
} Troncon;

typedef struct NoeudAVL_Dico {
    char id[256];
    Troncon *adresse_troncon;
    struct NoeudAVL_Dico *fg;
    struct NoeudAVL_Dico *fd;
    int equilibre;
} NoeudAVL_Dico;

// --- PROTOTYPES ---

void generer_fichier_histo(NoeudAVL_Histo *racine, char *nom_fichier, char *mode);
void liberer_troncons(Troncon *t);
long accumuler_flux(Troncon *t, NoeudAVL_Histo *dico_usines);

#endif