#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- STRUCTURES POUR L'HISTOGRAMME (Partie 1) ---

// Représente une usine et ses volumes calculés
typedef struct Station {
    char id[50];        // Identifiant (ex: "Plant #...")
    long capacite;      // Capacité max (donnée dans le CSV)
    long vol_source;    // Volume total capté/envoyé (Calculé)
    long vol_traite;    // Volume réel utile après pertes (Calculé)
} Station;

// Nœud de l'AVL pour stocker et trier les usines
typedef struct NoeudAVL_Histo {
    Station *data;
    struct NoeudAVL_Histo *fg; // Fils gauche
    struct NoeudAVL_Histo *fd; // Fils droit
    int equilibre;             // Hauteur du nœud
} NoeudAVL_Histo;


// --- STRUCTURES POUR LES FUITES (Partie 2) ---

// Représente un élément du réseau (Arbre N-aire)
typedef struct Troncon {
    char id[50];                 // Identifiant du tronçon
    float pourcentage_fuite;     // Pourcentage de perte à cet endroit
    struct Troncon *frere;       // Liste chaînée des frères (niveau horizontal)
    struct Troncon *fils;        // Premier fils (niveau inférieur)
} Troncon;

// Nœud de l'AVL "Dictionnaire" pour indexer les tronçons
// Permet de retrouver un parent en O(log n) au lieu de O(n)
typedef struct NoeudAVL_Dico {
    char id[50];                 // Clé de recherche
    Troncon *adresse_troncon;    // Pointeur direct vers le nœud du réseau
    struct NoeudAVL_Dico *fg;
    struct NoeudAVL_Dico *fd;
    int equilibre;
} NoeudAVL_Dico;

// --- PROTOTYPES DES FONCTIONS ---

// Génération du fichier pour Gnuplot
void generer_fichier_histo(NoeudAVL_Histo *racine, char *nom_fichier);

// Nettoyage de la mémoire
void liberer_troncons(Troncon *t);

// Logique métier : Parcours récursif pour calculer les volumes
long accumuler_flux(Troncon *t, NoeudAVL_Histo *dico_usines);

#endif
