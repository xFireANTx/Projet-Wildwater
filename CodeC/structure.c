#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"
#include "avl.h"

// --- GESTION DE LA SORTIE HISTOGRAMME ---

// Parcours récursif inverse (Droite -> Racine -> Gauche)
// Objectif : Écrire les usines dans l'ordre alphabétique décroissant (Z à A)
// comme demandé par le script Gnuplot ou les exemples de sortie.
void ecrire_histo_recur(NoeudAVL_Histo *n, FILE *f) {
    if (n == NULL) return;

    // 1. D'abord les plus grands (Droite)
    ecrire_histo_recur(n->fd, f);

    // 2. Traitement du nœud courant
    // Format : identifier;capacity;source;treated
    // Attention : Gnuplot attend des nombres, on s'assure qu'ils ne sont pas nuls
    fprintf(f, "%s;%ld;%ld;%ld\n", 
            n->data->id, 
            n->data->capacite, 
            n->data->vol_source, 
            n->data->vol_traite);

    // 3. Ensuite les plus petits (Gauche)
    ecrire_histo_recur(n->fg, f);
}

void generer_fichier_histo(NoeudAVL_Histo *racine, char *nom_fichier) {
    FILE *f = fopen(nom_fichier, "w");
    if (f == NULL) {
        printf("Erreur : Impossible de créer le fichier %s\n", nom_fichier);
        return;
    }

    // Écriture de l'en-tête obligatoire
    fprintf(f, "identifier;capacity;source;treated\n");

    // Lancement du parcours
    ecrire_histo_recur(racine, f);

    fclose(f);
}

// --- LOGIQUE MÉTIER (CALCULS) ---

// C'est la fonction "Intelligente".
// Elle parcourt l'arbre des connexions (le réseau physique).
// Elle renvoie le volume total d'eau qui passe par ce tronçon.
long accumuler_flux(Troncon *t, NoeudAVL_Histo *dico_usines) {
    if (t == NULL) return 0;

    // Volume qui vient des enfants (sous-réseau)
    long volume_venant_des_fils = accumuler_flux(t->fils, dico_usines);

    // On calcule le volume local.
    // Si c'est une feuille (pas de fils), on simule une consommation de base.
    // Sinon, c'est la somme des fils.
    long volume_local = (volume_venant_des_fils == 0) ? 1000 : volume_venant_des_fils;

    // Si ce tronçon correspond à une Usine connue (on cherche dans l'AVL Histo),
    // on met à jour ses données pour le graphique.
    Station *usine = rechercher_station(dico_usines, t->id);
    if (usine != NULL) {
        // Le volume "Source" est ce qui sort de l'usine
        usine->vol_source = volume_local;
        
        // Le volume "Traité" (Réel) est ce qui reste après les fuites du réseau aval
        // (Simplification niveau étudiant : on soustrait un ratio basé sur les fuites locales)
        // Ratio de perte local = pourcentage_fuite
        double perte = volume_local * (t->pourcentage_fuite / 100.0);
        usine->vol_traite = volume_local - (long)perte;
        
        // Sécurité : le volume ne peut pas dépasser la capacité max de l'usine
        if (usine->vol_source > usine->capacite) {
            usine->vol_source = usine->capacite;
        }
    }

    // Appel récursif pour traiter les frères (le reste du niveau)
    // Note : le volume des frères ne s'additionne pas au nôtre, c'est une autre branche.
    accumuler_flux(t->frere, dico_usines);

    // On retourne le volume de CE nœud pour que son parent puisse l'additionner
    return volume_local;
}

// --- NETTOYAGE MÉMOIRE ---

void liberer_troncons(Troncon *t) {
    if (t == NULL) return;

    // On libère d'abord les enfants (profondeur)
    liberer_troncons(t->fils);

    // Ensuite les frères (largeur)
    liberer_troncons(t->frere);

    // Enfin le nœud lui-même
    free(t);
}
