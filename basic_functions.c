#include <stdio.h>
#include <stdlib.h>

typedef struct arbre{
    char* identifiant;
    struct arbre *fg;
    struct arbre *fd;
}Arbre;

Arbre *rotation_droite(Arbre* a){
    Arbre *b = a->fg;
    a->fg = b->fg;
    b->fd = a;
    return b;
}

Arbre *rotation_gauche(Arbre* a){
    Arbre *b = a->fd;
    a->fd = b->fd;
    b->fg = a;
    return b;
}

Arbre *rotation_droite_gauche(Arbre *a){
    a->fd = rotation_droite(a->fd);
    return rotation_gauche(a);
}

Arbre *rotation_gauche_droite(Arbre *a){
    a->fg = rotation_gauche(a->fg);
    return rotation_droite(a);
}