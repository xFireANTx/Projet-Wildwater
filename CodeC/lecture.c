#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lecture.h"
#include "structures.h"
#include "avl.h"

// Fonction utilitaire pour récupérer une colonne précise (1 à 5)

void recuperer_colonne(char *ligne, int num_colonne, char *buffer) {
    int col_actuelle = 1;
    int i = 0, j = 0;
    
    // On parcourt toute la ligne caractère par caractère
    while (ligne[i] != '\0' && ligne[i] != '\n') {
        if (ligne[i] == ';') {
            // Si on était en train de lire la colonne demandée, on a fini
            if (col_actuelle == num_colonne) {
                buffer[j] = '\0';
                return;
            }
            // Sinon on passe à la colonne suivante
            col_actuelle++;
            i++;
            continue;
        }

        // Si on est dans la bonne colonne, on copie le caractère
        if (col_actuelle == num_colonne) {
            buffer[j] = ligne[i];
            j++;
        }
        i++;
    }
    // Fin de ligne : ne pas oublier le \0
    buffer[j] = '\0';
}

int traiter_fichier(const char *chemin_fichier, NoeudAVL_Histo **racine_histo, Troncon **racine_fuites, NoeudAVL_Dico **dico_fuites) {
    FILE *f = fopen(chemin_fichier, "r");
    if (f == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier %s\n", chemin_fichier);
        return 1;
    }

    char ligne[1024]; // Tampon assez grand pour une ligne
    char col1[256], col2[256], col3[256], col4[256], col5[256];

    // On ignore la première ligne (en-tête) si elle contient "identifier"
    fgets(ligne, sizeof(ligne), f);
    if (strstr(ligne, "identifier") == NULL) {
        // Si ce n'était pas l'en-tête, on rembobine (cas rare mais prudent)
        rewind(f);
    }

    while (fgets(ligne, sizeof(ligne), f)) {
        // Nettoyage de la fin de ligne
        ligne[strcspn(ligne, "\r\n")] = 0;

        if (strlen(ligne) == 0) continue; // Sauter les lignes vides

        // Extraction des 5 colonnes 
        recuperer_colonne(ligne, 1, col1);
        recuperer_colonne(ligne, 2, col2);
        recuperer_colonne(ligne, 3, col3);
        recuperer_colonne(ligne, 4, col4);
        recuperer_colonne(ligne, 5, col5);

        // --- CAS 1 : C'est une USINE (Pour l'Histogramme) ---
        // Format typique : Col2 contient "Plant" et Col3 est vide "-"
        // On récupère la capacité dans Col4
        if (strstr(col2, "Plant") != NULL && strcmp(col3, "-") == 0) {
            long capacite = atol(col4); // Conversion string -> long
            int h = 0;
            // On l'insère dans l'AVL Histo (trié par nom)
            *racine_histo = inserer_station(*racine_histo, col2, capacite, &h);
        }

        // --- CAS 2 : C'est une CONNEXION (Pour les Fuites) ---
        // Format : Un parent (Col2) et un enfant (Col3) existent
        // On doit construire l'arbre N-aire
        if (strcmp(col2, "-") != 0 && strcmp(col3, "-") != 0) {
            float fuite = atof(col5); // Conversion string -> float
            int h = 0;

            // 1. Créer le nœud pour l'enfant (le tronçon actuel)
            Troncon *nouveau = malloc(sizeof(Troncon));
            strcpy(nouveau->id, col3);
            nouveau->pourcentage_fuite = fuite;
            nouveau->fils = NULL;
            nouveau->frere = NULL;

            // 2. Chercher le parent dans le dictionnaire AVL
            Troncon *parent = rechercher_troncon(*dico_fuites, col2);

            if (parent != NULL) {
                // Le parent existe : on ajoute le nouveau comme fils
                // Ajout en tête de liste chaînée (c'est plus simple et rapide)
                nouveau->frere = parent->fils;
                parent->fils = nouveau;
            } else {
                // Le parent n'existe pas : c'est une racine (ex: Source)
                // On l'ajoute à la liste des racines principales
                nouveau->frere = *racine_fuites;
                *racine_fuites = nouveau;
            }

            // 3. Ajouter ce NOUVEAU nœud au dictionnaire pour qu'il puisse être parent plus tard
            *dico_fuites = inserer_dico(*dico_fuites, col3, nouveau, &h);
        }
    }

    fclose(f);
    return 0;
}
