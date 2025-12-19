#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structure.h"
#include "avl.h"

// Compteur pour le debug (pour vérifier que tout est écrit)
int compteur_stations = 0;

void ecrire_histo_recur(NoeudAVL_Histo *n, FILE *f, char *mode) {
    if (n == NULL) return;

    // Parcours Infixe standard pour traiter TOUT l'arbre
    ecrire_histo_recur(n->fg, f, mode);

    // Écriture de la station actuelle
    if (strcmp(mode, "max") == 0) {
        fprintf(f, "%s;%ld\n", n->data->id, n->data->capacite);
    } 
    else if (strcmp(mode, "src") == 0) {
        fprintf(f, "%s;%ld\n", n->data->id, n->data->vol_source);
    } 
    else if (strcmp(mode, "real") == 0) {
        fprintf(f, "%s;%ld\n", n->data->id, n->data->vol_traite);
    }
    compteur_stations++;

    ecrire_histo_recur(n->fd, f, mode);
}

void generer_fichier_histo(NoeudAVL_Histo *racine, char *nom_fichier, char *mode) {
    FILE *f = fopen(nom_fichier, "w");
    if (f == NULL) {
        printf("Erreur : Impossible de créer le fichier %s\n", nom_fichier);
        return;
    }

    compteur_stations = 0; // Reset
    fprintf(f, "identifier;value\n");

    ecrire_histo_recur(racine, f, mode);
    
    fclose(f);
    printf("DEBUG : %d stations ont été écrites dans %s\n", compteur_stations, nom_fichier);
}

void liberer_troncons(Troncon *t) {
    if (t == NULL) return;
    liberer_troncons(t->fils);
    liberer_troncons(t->frere);
    free(t);
}

long accumuler_flux(Troncon *t, NoeudAVL_Histo *dico_usines) {
    if (t == NULL) return 0;

    Station *s = rechercher_station(dico_usines, t->id);
    if (s != NULL) {
        // src : somme des volumes prélevés
        s->vol_source += t->volume; 
        
        // real : volume - pourcentage de fuite
        float perte = (float)t->volume * (t->pourcentage_fuite / 100.0);
        s->vol_traite += (long)((float)t->volume - perte);
    }

    accumuler_flux(t->fils, dico_usines);
    accumuler_flux(t->frere, dico_usines);
    
    return 0; 
}