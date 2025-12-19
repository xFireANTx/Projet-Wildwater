#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"
#include "structure.h"

// --- Fonctions utilitaires ---

// Retourne le maximum entre deux entiers
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Retourne la hauteur d'un noeud (0 si NULL)
// On utilise le champ 'equilibre' pour stocker la hauteur
int hauteur_histo(NoeudAVL_Histo *n) {
    if (n == NULL) return 0;
    return n->equilibre;
}

int hauteur_dico(NoeudAVL_Dico *n) {
    if (n == NULL) return 0;
    return n->equilibre;
}

// Calcule le facteur d'équilibre (Gauche - Droite)
int get_balance_histo(NoeudAVL_Histo *n) {
    if (n == NULL) return 0;
    return hauteur_histo(n->fg) - hauteur_histo(n->fd);
}

int get_balance_dico(NoeudAVL_Dico *n) {
    if (n == NULL) return 0;
    return hauteur_dico(n->fg) - hauteur_dico(n->fd);
}

// --- ROTATIONS (Identiques pour Histo et Dico) ---

// Rotation Droite pour AVL Histo
NoeudAVL_Histo* rotation_droite_histo(NoeudAVL_Histo *y) {
    NoeudAVL_Histo *x = y->fg;
    NoeudAVL_Histo *T2 = x->fd;

    // Rotation
    x->fd = y;
    y->fg = T2;

    // Mise à jour des hauteurs
    y->equilibre = max(hauteur_histo(y->fg), hauteur_histo(y->fd)) + 1;
    x->equilibre = max(hauteur_histo(x->fg), hauteur_histo(x->fd)) + 1;

    return x;
}

// Rotation Gauche pour AVL Histo
NoeudAVL_Histo* rotation_gauche_histo(NoeudAVL_Histo *x) {
    NoeudAVL_Histo *y = x->fd;
    NoeudAVL_Histo *T2 = y->fg;

    // Rotation
    y->fg = x;
    x->fd = T2;

    // Mise à jour des hauteurs
    x->equilibre = max(hauteur_histo(x->fg), hauteur_histo(x->fd)) + 1;
    y->equilibre = max(hauteur_histo(y->fg), hauteur_histo(y->fd)) + 1;

    return y;
}

// Rotation Droite pour AVL Dico
NoeudAVL_Dico* rotation_droite_dico(NoeudAVL_Dico *y) {
    NoeudAVL_Dico *x = y->fg;
    NoeudAVL_Dico *T2 = x->fd;

    x->fd = y;
    y->fg = T2;

    y->equilibre = max(hauteur_dico(y->fg), hauteur_dico(y->fd)) + 1;
    x->equilibre = max(hauteur_dico(x->fg), hauteur_dico(x->fd)) + 1;

    return x;
}

// Rotation Gauche pour AVL Dico
NoeudAVL_Dico* rotation_gauche_dico(NoeudAVL_Dico *x) {
    NoeudAVL_Dico *y = x->fd;
    NoeudAVL_Dico *T2 = y->fg;

    y->fg = x;
    x->fd = T2;

    x->equilibre = max(hauteur_dico(x->fg), hauteur_dico(x->fd)) + 1;
    y->equilibre = max(hauteur_dico(y->fg), hauteur_dico(y->fd)) + 1;

    return y;
}

// --- GESTION HISTOGRAMME (USINES) ---

NoeudAVL_Histo* creer_noeud_histo(char *id, long capacite) {
    NoeudAVL_Histo *noeud = (NoeudAVL_Histo*)malloc(sizeof(NoeudAVL_Histo));
    if (noeud == NULL) {
        printf("Erreur d'allocation memoire (Histo)\n");
        exit(1);
    }

    // Allocation et remplissage de la structure Station
    noeud->data = (Station*)malloc(sizeof(Station));
    if (noeud->data == NULL) {
        free(noeud);
        exit(1);
    }
    strcpy(noeud->data->id, id);
    noeud->data->capacite = capacite;
    noeud->data->vol_source = 0;
    noeud->data->vol_traite = 0;

    noeud->fg = NULL;
    noeud->fd = NULL;
    noeud->equilibre = 1; // Hauteur initiale de 1
    return noeud;
}

NoeudAVL_Histo* inserer_station(NoeudAVL_Histo *node, char *id, long capacite, int *h_change) {
    // 1. Insertion normale BST
    if (node == NULL) {
        return creer_noeud_histo(id, capacite);
    }

    int cmp = strcmp(id, node->data->id);

    if (cmp < 0)
        node->fg = inserer_station(node->fg, id, capacite, h_change);
    else if (cmp > 0)
        node->fd = inserer_station(node->fd, id, capacite, h_change);
    else
        return node; // Doublons non autorisés ou déjà existant

    // 2. Mise à jour de la hauteur
    node->equilibre = 1 + max(hauteur_histo(node->fg), hauteur_histo(node->fd));

    // 3. Vérification de l'équilibre
    int balance = get_balance_histo(node);

    // Cas Gauche-Gauche
    if (balance > 1 && strcmp(id, node->fg->data->id) < 0)
        return rotation_droite_histo(node);

    // Cas Droite-Droite
    if (balance < -1 && strcmp(id, node->fd->data->id) > 0)
        return rotation_gauche_histo(node);

    // Cas Gauche-Droite
    if (balance > 1 && strcmp(id, node->fg->data->id) > 0) {
        node->fg = rotation_gauche_histo(node->fg);
        return rotation_droite_histo(node);
    }

    // Cas Droite-Gauche
    if (balance < -1 && strcmp(id, node->fd->data->id) < 0) {
        node->fd = rotation_droite_histo(node->fd);
        return rotation_gauche_histo(node);
    }

    return node;
}

Station* rechercher_station(NoeudAVL_Histo *node, char *id) {
    if (node == NULL) return NULL;

    int cmp = strcmp(id, node->data->id);
    if (cmp == 0) return node->data;
    if (cmp < 0) return rechercher_station(node->fg, id);
    return rechercher_station(node->fd, id);
}

// --- GESTION DICTIONNAIRE (FUITES) ---

NoeudAVL_Dico* creer_noeud_dico(char *id, Troncon *ptr) {
    NoeudAVL_Dico *noeud = (NoeudAVL_Dico*)malloc(sizeof(NoeudAVL_Dico));
    if (noeud == NULL) exit(1);

    strcpy(noeud->id, id);
    noeud->adresse_troncon = ptr;
    noeud->fg = NULL;
    noeud->fd = NULL;
    noeud->equilibre = 1; 
    return noeud;
}

NoeudAVL_Dico* inserer_dico(NoeudAVL_Dico *node, char *id, Troncon *ptr, int *h_change) {
    if (node == NULL) return creer_noeud_dico(id, ptr);

    int cmp = strcmp(id, node->id);

    if (cmp < 0)
        node->fg = inserer_dico(node->fg, id, ptr, h_change);
    else if (cmp > 0)
        node->fd = inserer_dico(node->fd, id, ptr, h_change);
    else
        return node; 

    node->equilibre = 1 + max(hauteur_dico(node->fg), hauteur_dico(node->fd));
    int balance = get_balance_dico(node);

    if (balance > 1 && strcmp(id, node->fg->id) < 0)
        return rotation_droite_dico(node);

    if (balance < -1 && strcmp(id, node->fd->id) > 0)
        return rotation_gauche_dico(node);

    if (balance > 1 && strcmp(id, node->fg->id) > 0) {
        node->fg = rotation_gauche_dico(node->fg);
        return rotation_droite_dico(node);
    }

    if (balance < -1 && strcmp(id, node->fd->id) < 0) {
        node->fd = rotation_droite_dico(node->fd);
        return rotation_gauche_dico(node);
    }

    return node;
}

Troncon* rechercher_troncon(NoeudAVL_Dico *node, char *id) {
    if (node == NULL) return NULL;

    int cmp = strcmp(id, node->id);
    if (cmp == 0) return node->adresse_troncon;
    if (cmp < 0) return rechercher_troncon(node->fg, id);
    return rechercher_troncon(node->fd, id);
}

// --- LIBÉRATION MÉMOIRE ---

void liberer_avl_histo(NoeudAVL_Histo *n) {
    if (n != NULL) {
        liberer_avl_histo(n->fg);
        liberer_avl_histo(n->fd);
        free(n->data); // On libère la struct Station
        free(n);
    }
}

void liberer_avl_dico(NoeudAVL_Dico *n) {
    if (n != NULL) {
        liberer_avl_dico(n->fg);
        liberer_avl_dico(n->fd);
        // On ne libère PAS le Troncon ici, car il est dans l'arbre N-aire
        free(n);
    }
}
