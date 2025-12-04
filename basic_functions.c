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

/*affichage/recup csv
unit/ module/ plant/ = usine
source/ well/ well field/ fountain/ resurgence = la source d'eau
storage = endroit de stockage
junction = branchement 1 des tuyaux (stockage -> service)
service = branchement 2 des tuyaux (service -> menage)

cust = menages

*/
//unit/ module/ plant/ = usine
typedef struct Usine{
    char code[10];
    int capa_max;
}usine;

//junction = branchement 1 des tuyaux (stockage -> service)
typedef struct Jonction{
    char code[9];

}jonction;

//service = branchement 2 des tuyaux (service -> menage)
typedef struct Service{
    char code[9];

}service;

//storage = endroit de stockage
typedef struct Storage{
    char code[6];
}storage;

//source/ well/ well field/ fountain/ resurgence = la source d'eau
typedef struct Source{

}source;