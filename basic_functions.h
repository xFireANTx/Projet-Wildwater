#ifndef BASIC_FUNCTIONS_H_  
#define BASIC_FUNCTIONS_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//arbres

//traitement
#define CODE_SIZE 11
typedef struct Infra{
    int type;
    char code_usine[CODE_SIZE];
    char code_precedent[CODE_SIZE];
    char code_actuel[CODE_SIZE];
    float fuite;
    float flux; // pas besoin de capa max
}infra;

typedef struct Arbres_fuites{
    infra *structure;
    struct Arbres_fuites *suivant;
    struct Arbres_fuites *actuelf;
}arbres_fuites;

typedef struct Racine{
    char code_usine[CODE_SIZE];
    float flux;
    struct Racine *suivant;
    struct Arbres_fuites *actuelf;
}racine;

typedef struct Arbre{
    racine *usine; //pour trier l arbre utilise arbre->racine->code usine       
    struct Arbre *fg;
    struct Arbre *fd;
}arbre;

infra *remplir_infra(char *line, int type);         //marche pour 3,4,5,6 jsp si je veut utiliser la meme chose pour usine et source n est pas utile
arbres_fuites *createNode(char *line, int type);
arbres_fuites *addChildfuites(arbres_fuites *parent, arbres_fuites *child);


//lecture fichier
int detect_type(char* line); // 1 = source, 2 = usine, 3 = stockage, 4 = jonction, 5 = service, 6 = menages
int code_len(const char *s);
int empty(const char *s);


void calcule_fuites(racine *usine);
float recuperer_fuites(racine *usine);
racine *ajouter_arbre_usine(racine *node, arbres_fuites *new);
racine *chercher_avl(const char *code_usine, arbre *root);

// Traverse the AVL in-order and perform per-node action
void traverse_avl(arbre *root);
void free_arbre_usine(racine *root);
static void free_arbres_recursive(arbres_fuites *head);

#endif 
