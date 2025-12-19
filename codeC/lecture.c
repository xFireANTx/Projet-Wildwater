#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lecture.h"
#include "structure.h"
#include "avl.h"

// --- FONCTIONS UTILITAIRES ---

void recuperer_colonne(char *ligne, int num_colonne, char *buffer) {
    int col_actuelle = 1;
    int i = 0, j = 0;
    buffer[0] = '\0';

    while (ligne[i] != '\0' && ligne[i] != '\n') {
        if (ligne[i] == ';') {
            if (col_actuelle == num_colonne) {
                buffer[j] = '\0';
                return;
            }
            col_actuelle++;
            i++;
            continue;
        }
        if (col_actuelle == num_colonne) {
            buffer[j] = ligne[i];
            j++;
        }
        i++;
    }
    buffer[j] = '\0';
}

void trim(char *str) {
    int n = strlen(str);
    while (n > 0 && (str[n-1] == ' ' || str[n-1] == '\t' || str[n-1] == '\r' || str[n-1] == '\n')) {
        str[n-1] = '\0';
        n--;
    }
}

// --- FONCTION PRINCIPALE DE LECTURE ---

int traiter_fichier(const char *chemin_fichier, NoeudAVL_Histo **racine_histo, Troncon **racine_fuites, NoeudAVL_Dico **dico_fuites) {
    FILE *f = fopen(chemin_fichier, "r");
    if (f == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier %s\n", chemin_fichier);
        return 1;
    }

    char ligne[1024]; 
    char col1[256], col2[256], col3[256], col4[256], col5[256];

    // Saut de l'en-tête
    if (fgets(ligne, sizeof(ligne), f)) {
        if (strstr(ligne, "Station") == NULL && strstr(ligne, "Type") == NULL && strstr(ligne, ";") == NULL) {
            rewind(f);
        }
    }

    while (fgets(ligne, sizeof(ligne), f)) {
        recuperer_colonne(ligne, 1, col1);
        recuperer_colonne(ligne, 2, col2);
        recuperer_colonne(ligne, 3, col3);
        recuperer_colonne(ligne, 4, col4);
        recuperer_colonne(ligne, 5, col5);
        trim(col1); trim(col2); trim(col3); trim(col4); trim(col5);

        if (strlen(col2) == 0) continue;

        // ============================================================
        // CAPTURE UNIVERSELLE DES STATIONS (Histogramme)
        // On cherche le '#' qui identifie un ID unique (Plant, Unit, Facility...)
        // ============================================================
        if (strstr(col2, "#") != NULL) {
            long valeur = atol(col4);
            if (valeur > 0) {
                int h = 0;
                // On utilise col2 comme identifiant unique pour l'AVL
                // Grâce au cumul dans avl.c, toutes les lignes du même ID seront sommées
                *racine_histo = inserer_station(*racine_histo, col2, valeur, &h);
            }
        }

        // ============================================================
        // GESTION DES CONNEXIONS (Arbre N-aire des Fuites)
        // ============================================================
        if (strcmp(col2, "-") != 0 && strcmp(col3, "-") != 0 && strlen(col3) > 0) {
            Troncon *nouveau = malloc(sizeof(Troncon));
            if (nouveau == NULL) return 1;

            strcpy(nouveau->id, col3);
            nouveau->pourcentage_fuite = atof(col5);
            nouveau->volume = atol(col4); // On stocke le volume pour les calculs de flux
            nouveau->fils = NULL;
            nouveau->frere = NULL;

            Troncon *parent = rechercher_troncon(*dico_fuites, col2);
            if (parent != NULL) {
                nouveau->frere = parent->fils;
                parent->fils = nouveau;
            } else {
                nouveau->frere = *racine_fuites;
                *racine_fuites = nouveau;
            }
            int h = 0;
            *dico_fuites = inserer_dico(*dico_fuites, col3, nouveau, &h);
        }
    }

    fclose(f);
    return 0;
}