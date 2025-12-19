#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lecture.h"
#include "avl.h"
#include "structure.h" 

int main(int argc, char *argv[]) {
    // Vérification des arguments [cite: 2025-12-19]
    if (argc < 4) {
        printf("Usage: %s <fichier.csv> <histo|leaks> <param> [fichier_sortie]\n", argv[0]);
        return 1;
    }

    const char *fichier_input = argv[1];
    const char *commande = argv[2];
    char *parametre = argv[3];
    
    // Initialisation des structures [cite: 2025-12-19]
    NoeudAVL_Histo *racine_histo = NULL;
    Troncon *racine_fuites = NULL;      
    NoeudAVL_Dico *dico_fuites = NULL;  

    // 1. Lecture et remplissage des arbres
    if (traiter_fichier(fichier_input, &racine_histo, &racine_fuites, &dico_fuites) != 0) {
        return 2; 
    }

    // 2. Calculs des flux (Accumulation Src/Real)
    // On parcourt chaque racine de réseau trouvée
    Troncon *courant = racine_fuites;
    while (courant != NULL) {
        accumuler_flux(courant, racine_histo);
        courant = courant->frere;
    }

    // 3. Traitement et Génération de la sortie
    if (strcmp(commande, "histo") == 0) {
        if (argc < 5) {
            printf("Erreur : Nom de fichier de sortie manquant pour histo\n");
            return 3;
        }
        char *fichier_sortie = argv[4];

        // Génère le fichier .dat trié avec tous les identifiants uniques
        generer_fichier_histo(racine_histo, fichier_sortie, parametre);
    }
    else if (strcmp(commande, "leaks") == 0) {
        // Logique Leaks (à implémenter ultérieurement) [cite: 2025-12-19]
        printf("Mode Leaks pour %s (En construction)\n", parametre);
    }
    else {
        printf("Commande inconnue\n");
        return 1;
    }

    // 4. Libération de la mémoire
    liberer_troncons(racine_fuites);
    liberer_avl_histo(racine_histo);
    liberer_avl_dico(dico_fuites);

    return 0;
}