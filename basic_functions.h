#ifndef BASIC_FUNCTIONS_H_
#define BASIC_FUNCTIONS_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CODE_SIZE 11

typedef struct Infra {
    int type;
    char code_usine[CODE_SIZE];
    char code_precedent[CODE_SIZE];
    char code_actuel[CODE_SIZE];
    float fuite;
    float flux;
} infra;

typedef struct Arbres_fuites {
    infra *structure;
    struct Arbres_fuites *suivant;  
    struct Arbres_fuites *actuelf;  
} arbres_fuites;

typedef struct Racine {
    char code_usine[CODE_SIZE];
    float flux;
    struct Racine *suivant;         
    struct Arbres_fuites *actuelf;  
} racine;

typedef struct Arbre {
    racine *usine;
    struct Arbre *fg;
    struct Arbre *fd;
    int hauteur;
} arbre;


infra *remplir_infra(char *line, int type);
arbres_fuites *createNode(char *line, int type);


arbres_fuites *addChildfuites(arbres_fuites *parent, arbres_fuites *child);

int detect_type(char *line);
int code_len(const char *s);
int empty(const char *s);


racine *ajouter_arbre_usine(racine *node, arbres_fuites *newn);
racine *chercher_avl(const char *code_usine, arbre *root);


void calcule_fuites(racine *usine);
float recuperer_fuites(racine *usine);
void traverse_avl(arbre *root);

#endif /* BASIC_FUNCTIONS_H_ */
