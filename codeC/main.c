#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structure.h"
#include "avl.h"
#include "lecture.h"

int main(int argc, char *argv[]) {
    // Verification simple des arguments
    if (argc < 3) {
        printf("Erreur : Il manque des arguments.\n");
        printf("Usage : %s <fichier_donnees> <commande>\n", argv[0]);
        return 1;
    }

    // 1. Initialisation des pointeurs (Arbres vides au depart)
    NoeudAVL_Histo *racine_histo = NULL;
    Troncon *racine_fuites = NULL;      // Liste des points de depart (Sources)
    NoeudAVL_Dico *dico_fuites = NULL;  // Pour retrouver les parents rapidement

    // 2. Chargement des donnees depuis le fichier CSV
    printf("Chargement du fichier %s ...\n", argv[1]);
    int retour = traiter_fichier(argv[1], &racine_histo, &racine_fuites, &dico_fuites);
    
    if (retour != 0) {
        return 1; // On arrete tout si le fichier n'est pas bon
    }

    // 3. Etape de calcul 
    // On parcourt chaque reseau depuis sa source pour calculer les volumes
    Troncon *courant = racine_fuites;
    while (courant != NULL) {
        // Cette fonction recursive va remplir les volumes dans l'AVL Histo
        accumuler_flux(courant, racine_histo);
        courant = courant->frere; // On passe a la source suivante
    }

    // 4. Gestion des commandes (histo ou leaks)
    if (strcmp(argv[2], "histo") == 0) {
        // Commande : HISTO
        // On genere le fichier .dat complet, le script shell fera le tri
        generer_fichier_histo(racine_histo, "histo_all.dat");
        printf("Succes : Données exportees dans histo_all.dat\n");
    }
    else if (strcmp(argv[2], "leaks") == 0) {
        // Commande : LEAKS
        // On ouvre le fichier log en mode "ajout" (append) comme demande
        FILE *f_log = fopen("leaks_per_plant.log", "a");
        if (f_log != NULL) {
            // Ici on ecrit juste une confirmation pour l'instant
            // (La logique detaillee des fuites pourra etre ajoutee ici)
            fprintf(f_log, "Traitement leaks effectue sur %s\n", argv[1]);
            fclose(f_log);
            printf("Succes : Rapport ajoute a leaks_per_plant.log\n");
        }
    }
    else {
        printf("Commande inconnue : %s\n", argv[2]);
    }

    // 5. Nettoyage final 
    liberer_avl_histo(racine_histo);
    liberer_avl_dico(dico_fuites);
    liberer_troncons(racine_fuites);

    return 0;
}
