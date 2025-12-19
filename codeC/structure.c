#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structure.h"
#include "avl.h"

// ============================================================
// PARTIE 1 : GESTION DE LA SORTIE HISTOGRAMME
// ============================================================

void ecrire_histo_recur(NoeudAVL_Histo *n, FILE *f, char *mode) {
    if (n == NULL) return;

    // Parcours Infixe (Z->A) pour un tri propre dans le .dat
    ecrire_histo_recur(n->fd, f, mode);

    if (strcmp(mode, "max") == 0) {
        fprintf(f, "%s;%ld\n", n->data->id, n->data->capacite);
    } 
    else if (strcmp(mode, "src") == 0) {
        fprintf(f, "%s;%ld\n", n->data->id, n->data->vol_source);
    } 
    else if (strcmp(mode, "real") == 0) {
        fprintf(f, "%s;%ld\n", n->data->id, n->data->vol_traite);
    }

    ecrire_histo_recur(n->fg, f, mode);
}

void generer_fichier_histo(NoeudAVL_Histo *racine, char *nom_fichier, char *mode) {
    FILE *f = fopen(nom_fichier, "w");
    if (f == NULL) {
        printf("Erreur : Impossible de créer le fichier %s\n", nom_fichier);
        return;
    }

    // En-tête standard pour Gnuplot
    fprintf(f, "identifier;value\n");

    ecrire_histo_recur(racine, f, mode);
    fclose(f);
}

// ============================================================
// PARTIE 2 : GESTION DES FLUX ET TRONÇONS
// ============================================================

void liberer_troncons(Troncon *t) {
    if (t == NULL) return;
    liberer_troncons(t->fils);
    liberer_troncons(t->frere);
    free(t);
}

// Correction majeure : On connecte enfin l'arbre N-aire à l'AVL des stations
long accumuler_flux(Troncon *t, NoeudAVL_Histo *dico_usines) {
    if (t == NULL) return 0;

    // 1. On cherche si ce tronçon (t->id) est une station de notre histogramme
    Station *s = rechercher_station(dico_usines, t->id);
    if (s != NULL) {
        // On met à jour les volumes de la station avec les données du tronçon
        s->vol_source += t->volume; 
        
        // Calcul du volume réel (Treated) en soustrayant la fuite
        float perte = (float)t->volume * (t->pourcentage_fuite / 100.0);
        s->vol_traite += (long)((float)t->volume - perte);
    }

    // 2. Récursion pour parcourir tout le réseau
    accumuler_flux(t->fils, dico_usines);
    accumuler_flux(t->frere, dico_usines);
    
    return 0; 
}