#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lecture.h"
#include "avl.h"
#include "structure.h" // <--- Sans 's' ici aussi !

int main(int argc, char *argv[]) {
    // Arguments attendus :
    // 1: Fichier CSV input
    // 2: Commande ("histo" ou "leaks")
    // 3: Paramètre (mode "max/src/real" OU identifiant usine)
    // 4: Nom du fichier de sortie

    if (argc < 4) {
        printf("Usage: %s <fichier.csv> <histo|leaks> <param> [fichier_sortie]\n", argv[0]);
        return 1;
    }

    const char *fichier_input = argv[1];
    const char *commande = argv[2];
    char *parametre = argv[3];
    
    // Initialisation
    NoeudAVL_Histo *racine_histo = NULL;
    Troncon *racine_fuites = NULL;      
    NoeudAVL_Dico *dico_fuites = NULL;  

    // Lecture
    if (traiter_fichier(fichier_input, &racine_histo, &racine_fuites, &dico_fuites) != 0) {
        return 2; 
    }

    // Calculs flux
    Troncon *courant = racine_fuites;
    while (courant != NULL) {
        accumuler_flux(courant, racine_histo);
        courant = courant->frere;
    }

    // Traitement
    if (strcmp(commande, "histo") == 0) {
        if (argc < 5) {
            printf("Erreur : Nom de fichier de sortie manquant pour histo\n");
            return 3;
        }
        char *fichier_sortie = argv[4];

        // Appel avec le mode et le nom de fichier
        generer_fichier_histo(racine_histo, fichier_sortie, parametre);
    }
    else if (strcmp(commande, "leaks") == 0) {
        // (Partie Leaks à venir)
        printf("Mode Leaks pour %s (En construction)\n", parametre);
    }
    else {
        printf("Commande inconnue\n");
        return 1;
    }

    liberer_troncons(racine_fuites);
    return 0;
}