#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"
#include "avl.h"

// --- GESTION DE LA SORTIE HISTOGRAMME ---

// Parcours récursif inverse (Droite -> Racine -> Gauche)
// Cela permet d'obtenir l'ordre alphabétique décroissant (Z vers A) demandé
void ecrire_histo_recur(NoeudAVL_Histo *n, FILE *f) {
    if (n == NULL) return;

    // 1. On descend d'abord à droite (les valeurs plus grandes alphabétiquement)
    ecrire_histo_recur(n->fd, f);

    // 2. On écrit les données du noeud courant
    // Format : Identifier;Capacity;Source;Treated
    // Note : Les volumes source et traité seront calculés dans le main avant l'appel
    fprintf(f, "%s;%ld;%ld;%ld\n", n->data->id, n->data->capacite, n->data->vol_source, n->data->vol_traite);

    // 3. On finit par la gauche (les valeurs plus petites)
    ecrire_histo_recur(n->fg, f);
}

// Fonction wrapper pour lancer l'écriture depuis le main
void generer_fichier_histo(NoeudAVL_Histo *racine, char *nom_fichier) {
    FILE *f = fopen(nom_fichier, "w");
    if (f == NULL) {
        printf("Erreur : Impossible de créer le fichier %s\n", nom_fichier);
        return;
    }

    // Écriture de l'en-tête obligatoire pour Gnuplot
    fprintf(f, "identifier;capacity;source;treated\n");

    // Lancement du parcours récursif
    ecrire_histo_recur(racine, f);

    fclose(f);
}

// --- GESTION DE LA MÉMOIRE (Arbre N-aire des fuites) ---

// Libère récursivement tous les tronçons (Enfants puis Frères)
void liberer_troncons(Troncon *t) {
    if (t == NULL) return;

    // Appel récursif sur le premier fils
    liberer_troncons(t->fils);

    // Appel récursif sur le frère suivant
    liberer_troncons(t->frere);

    // Libération du noeud courant une fois que les descendants sont traités
    free(t);
}
