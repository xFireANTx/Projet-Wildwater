#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- STRUCTURES POUR L'HISTOGRAMME ---

// Représente une usine et ses compteurs pour l'histogramme
typedef struct Station {
    char id[50];        // Identifiant (ex: "Plant #...")
    long capacite;      // Capacité max (Col 4 si Usine)
    long vol_source;    // Total capté par les sources reliées
    long vol_traite;    // Volume réel sortie usine (pour le bonus)
} Station;

// Nœud de l'AVL pour stocker et trier les usines
typedef struct NoeudAVL_Histo {
    Station *data;
    struct NoeudAVL_Histo *fg; // Fils gauche
    struct NoeudAVL_Histo *fd; // Fils droit
    int equilibre;             // Facteur d'équilibre
} NoeudAVL_Histo;


// --- STRUCTURES POUR LES FUITES ---

// Représente un lien dans le réseau (Arbre N-aire)
typedef struct Troncon {
    char id[50];                 // Identifiant du nœud (ex: "Junction #...")
    float pourcentage_fuite;     // Col 5
    struct Troncon *frere;       // Liste chaînée des frères (N-aire)
    struct Troncon *fils;        // Premier fils (N-aire)
} Troncon;

// Nœud de l'AVL "Dictionnaire" pour retrouver rapidement un Tronçon par son ID
// (Indispensable pour la consigne page 10 : recherche logarithmique du parent)
typedef struct NoeudAVL_Dico {
    char id[50];                 // Clé de recherche
    Troncon *adresse_troncon;    // Pointeur vers le nœud dans l'arbre N-aire
    struct NoeudAVL_Dico *fg;
    struct NoeudAVL_Dico *fd;
    int equilibre;
} NoeudAVL_Dico;

// --- Prototypes ajoutés pour structures.c ---
void generer_fichier_histo(NoeudAVL_Histo *racine, char *nom_fichier);
void liberer_troncons(Troncon *t);

#endif
