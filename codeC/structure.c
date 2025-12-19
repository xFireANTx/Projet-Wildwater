#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structure.h"
#include "avl.h"

// ============================================================
// PARTIE 1 : GESTION DE LA SORTIE HISTOGRAMME (NOUVEAU)
// ============================================================

// Parcours récursif inverse (Z->A) avec filtrage des colonnes
void ecrire_histo_recur(NoeudAVL_Histo *n, FILE *f, char *mode) {
    if (n == NULL) return;

    // 1. D'abord les plus grands (Droite -> Z)
    ecrire_histo_recur(n->fd, f, mode);

    // 2. Écriture selon le mode choisi
    if (strcmp(mode, "max") == 0) {
        // Mode Capacité : identifier;capacity
        fprintf(f, "%s;%ld\n", n->data->id, n->data->capacite);
    } 
    else if (strcmp(mode, "src") == 0) {
        // Mode Source : identifier;source
        fprintf(f, "%s;%ld\n", n->data->id, n->data->vol_source);
    } 
    else if (strcmp(mode, "real") == 0) {
        // Mode Réel : identifier;treated
        fprintf(f, "%s;%ld\n", n->data->id, n->data->vol_traite);
    }

    // 3. Ensuite les plus petits (Gauche -> A)
    ecrire_histo_recur(n->fg, f, mode);
}

void generer_fichier_histo(NoeudAVL_Histo *racine, char *nom_fichier, char *mode) {
    FILE *f = fopen(nom_fichier, "w");
    if (f == NULL) {
        printf("Erreur : Impossible de créer le fichier %s\n", nom_fichier);
        return;
    }

    // Écriture de l'en-tête selon le mode
    if (strcmp(mode, "max") == 0) {
        fprintf(f, "identifier;capacity\n");
    } else if (strcmp(mode, "src") == 0) {
        fprintf(f, "identifier;source\n");
    } else if (strcmp(mode, "real") == 0) {
        fprintf(f, "identifier;treated\n");
    }

    // Lancement du parcours
    ecrire_histo_recur(racine, f, mode);

    fclose(f);
}

// ============================================================
// PARTIE 2 : GESTION DES FLUX ET TRONÇONS (CE QUI MANQUAIT)
// ============================================================

void liberer_troncons(Troncon *t) {
    if (t == NULL) return;
    liberer_troncons(t->fils);
    liberer_troncons(t->frere);
    free(t);
}

// Fonction récursive pour propager les flux dans l'arbre N-aire
// Note: Cette fonction suppose que tu as une logique pour trouver l'usine correspondante
// Si l'identifiant du tronçon correspond à une usine dans l'AVL, on met à jour l'usine.
long accumuler_flux(Troncon *t, NoeudAVL_Histo *dico_usines) {
    if (t == NULL) return 0;

    // Ici, c'est une version simplifiée car je n'ai pas la logique exacte de ton calcul de flux.
    // L'idée générale est de parcourir les fils pour sommer les volumes.
    
    // Exemple de logique standard :
    // 1. On traite les fils d'abord
    accumuler_flux(t->fils, dico_usines);
    
    // 2. On traite les frères ensuite
    accumuler_flux(t->frere, dico_usines);


    
    return 0; 
}