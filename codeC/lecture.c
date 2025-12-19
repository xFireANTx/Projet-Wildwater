#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lecture.h"
#include "structure.h"
#include "avl.h"

// --- FONCTIONS UTILITAIRES ---

// Récupère le contenu d'une colonne (1 à 5) dans une ligne CSV
// Gère les champs vides et les points-virgules successifs
void recuperer_colonne(char *ligne, int num_colonne, char *buffer) {
    int col_actuelle = 1;
    int i = 0, j = 0;
    
    // Initialisation du buffer
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

// Nettoie les espaces invisibles (espaces, retours chariot) à la fin des chaînes
// Indispensable pour comparer correctement les chaînes comme "-" ou les IDs
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

    // Saut de l'en-tête (si détecté)
    long pos_avant = ftell(f); // On garde la position
    if (fgets(ligne, sizeof(ligne), f)) {
        if (strstr(ligne, "Station") == NULL && strstr(ligne, "Type") == NULL) {
            // Si la première ligne ne ressemble pas à un titre, on revient au début
            fseek(f, pos_avant, SEEK_SET);
        }
    }

    while (fgets(ligne, sizeof(ligne), f)) {
        // Nettoyage rapide de la ligne brute
        ligne[strcspn(ligne, "\r\n")] = 0;
        if (strlen(ligne) == 0) continue; 

        // Extraction des colonnes
        recuperer_colonne(ligne, 1, col1);
        recuperer_colonne(ligne, 2, col2);
        recuperer_colonne(ligne, 3, col3);
        recuperer_colonne(ligne, 4, col4);
        recuperer_colonne(ligne, 5, col5);
        
        // Nettoyage précis des valeurs extraites
        trim(col1); trim(col2); trim(col3); trim(col4); trim(col5);

        // ============================================================
        // CAS 1 : C'EST UNE USINE (Pour l'Histogramme)
        // Critère : Type (Col2) contient "Plant" ET Capacité (Col4) n'est pas vide
        // ============================================================
        if (strstr(col2, "Plant") != NULL) {
            // Vérification que c'est bien une ligne de définition (Col3 vide ou "-")
            if (strcmp(col3, "-") == 0 || strcmp(col3, "") == 0) {
                long capacite = atol(col4);
                
                // On n'ajoute que si on a une vraie capacité
                if (capacite > 0) {
                    int h = 0;
                    
                    // LOGIQUE D'IDENTIFIANT ROBUSTE :
                    // Si Col1 est "-" (cas fréquent dans ton fichier), l'ID est dans Col2.
                    // Sinon, l'ID est dans Col1.
                    char *id_final = col1;
                    if (strcmp(col1, "-") == 0 || strlen(col1) == 0) {
                        id_final = col2;
                    }

                    *racine_histo = inserer_station(*racine_histo, id_final, capacite, &h);
                }
            }
        }

        // ============================================================
        // CAS 2 : C'EST UNE CONNEXION (Pour les Fuites)
        // Critère : Col2 (Source) et Col3 (Destination) existent
        // ============================================================
        if (strcmp(col2, "-") != 0 && strcmp(col3, "-") != 0 && strcmp(col3, "") != 0) {
            // C'est un tuyau entre Col2 et Col3
            float fuite = atof(col5); 
            int h = 0;

            // 1. Création du nœud tronçon (Enfant)
            Troncon *nouveau = malloc(sizeof(Troncon));
            if (nouveau == NULL) {
                printf("Erreur d'allocation mémoire\n");
                fclose(f);
                return 1;
            }
            strcpy(nouveau->id, col3); // L'ID du tuyau/station destination
            nouveau->pourcentage_fuite = fuite;
            nouveau->fils = NULL;
            nouveau->frere = NULL;

            // 2. Recherche du Parent (Source) dans le dictionnaire
            // Attention : ici le parent est Col2 (ex: Pipe A), pas Col1
            Troncon *parent = rechercher_troncon(*dico_fuites, col2);

            if (parent != NULL) {
                // Ajout en tête de liste des fils du parent
                nouveau->frere = parent->fils;
                parent->fils = nouveau;
            } else {
                // Parent introuvable => C'est une racine de réseau (ex: une Source ou une Usine de départ)
                nouveau->frere = *racine_fuites;
                *racine_fuites = nouveau;
            }

            // 3. Enregistrement du NOUVEAU tronçon dans le dictionnaire
            // (Pour qu'il puisse être retrouvé s'il est parent à la ligne suivante)
            *dico_fuites = inserer_dico(*dico_fuites, col3, nouveau, &h);
        }
    }

    fclose(f);
    return 0;
}