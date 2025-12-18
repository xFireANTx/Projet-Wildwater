#include "basic_functions.h"

int max(int a, int b) {
    return (a > b) ? a : b;
}

int get_hauteur(Arbre* n) {
    if (n == NULL) return 0;
    return n->hauteur;
}

int get_equilibre(Arbre* n) {
    if (n == NULL) return 0;
    return get_hauteur(n->fg) - get_hauteur(n->fd);
}

// --- Rotations (Logique standard) ---

Arbre *rotation_droite(Arbre* y) {
    Arbre *x = y->fg;
    Arbre *T2 = x->fd;

    x->fd = y;
    y->fg = T2;

    y->hauteur = max(get_hauteur(y->fg), get_hauteur(y->fd)) + 1;
    x->hauteur = max(get_hauteur(x->fg), get_hauteur(x->fd)) + 1;

    return x;
}

Arbre *rotation_gauche(Arbre* x) {
    Arbre *y = x->fd;
    Arbre *T2 = y->fg;

    y->fg = x;
    x->fd = T2;

    x->hauteur = max(get_hauteur(x->fg), get_hauteur(x->fd)) + 1;
    y->hauteur = max(get_hauteur(y->fg), get_hauteur(y->fd)) + 1;

    return y;
}

Arbre *rotation_droite_gauche(Arbre *a) {
    a->fd = rotation_droite(a->fd);
    return rotation_gauche(a);
}

Arbre *rotation_gauche_droite(Arbre *a) {
    a->fg = rotation_gauche(a->fg);
    return rotation_droite(a);
}

// --- Gestion Arbre ---

Arbre* creer_noeud(char* id) {
    Arbre* node = (Arbre*)malloc(sizeof(Arbre));
    if (node == NULL) exit(1); // Sécurité alloc
    
    node->identifiant = strdup(id);
    node->fg = NULL;
    node->fd = NULL;
    node->hauteur = 1;
    node->capacity = 0;
    node->vol_src = 0;
    node->vol_real = 0;
    return node;
}

Arbre* insertion_avl(Arbre* node, char* id, long cap, double v_src, double v_real) {
    // 1. Insertion ABR classique
    if (node == NULL)
        return creer_noeud(id);

    int cmp = strcmp(id, node->identifiant);

    if (cmp < 0)
        node->fg = insertion_avl(node->fg, id, cap, v_src, v_real);
    else if (cmp > 0)
        node->fd = insertion_avl(node->fd, id, cap, v_src, v_real);
    else {
        // Le noeud existe : on cumule les données
        node->capacity += cap;
        node->vol_src += v_src;
        node->vol_real += v_real;
        return node;
    }

    // 2. Mise à jour hauteur
    node->hauteur = 1 + max(get_hauteur(node->fg), get_hauteur(node->fd));

    // 3. Equilibrage
    int balance = get_equilibre(node);

    if (balance > 1 && strcmp(id, node->fg->identifiant) < 0)
        return rotation_droite(node);

    if (balance < -1 && strcmp(id, node->fd->identifiant) > 0)
        return rotation_gauche(node);

    if (balance > 1 && strcmp(id, node->fg->identifiant) > 0)
        return rotation_gauche_droite(node);

    if (balance < -1 && strcmp(id, node->fd->identifiant) < 0)
        return rotation_droite_gauche(node);

    return node;
}

void parcours_infixe_inverse(Arbre* a, FILE* sortie) {
    if (a != NULL) {
        parcours_infixe_inverse(a->fd, sortie); // Droite (Z->A)
        fprintf(sortie, "%s;%ld;%f;%f\n", a->identifiant, a->capacity, a->vol_src, a->vol_real);
        parcours_infixe_inverse(a->fg, sortie); // Gauche
    }
}

void liberer_arbre(Arbre* a) {
    if (a != NULL) {
        liberer_arbre(a->fg);
        liberer_arbre(a->fd);
        free(a->identifiant);
        free(a);
    }
}
