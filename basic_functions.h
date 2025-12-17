#ifndef BASIC_FUNCTIONS_H_  
#define BASIC_FUNCTIONS_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//arbres
typedef struct arbre{
    char* identifiant;
    struct arbre *fg;
    struct arbre *fd;
}Arbre;

Arbre *rotation_droite(Arbre* a);
Arbre *rotation_gauche(Arbre* a);
Arbre *rotation_droite_gauche(Arbre *a);
Arbre *rotation_gauche_droite(Arbre *a);

//traitement
typedef struct Infra{
    char code_usine[11];
    char code_precedent[11];
    char code_actuel[11];
    int capa_max;
    float fuite;
}infra;
infra *remplir_infra(char *line, int type);
void freeTree(arbres_fuites *node);
void addChild(arbres_fuites *parent, arbres_fuites *child);
arbres_fuites *createNode(char code[10]);

typedef struct Arbre_fuites{
    char code_usine[11];
    infra *structure;
    struct Arbre_fuites *premierf;
    struct Arbre_fuites *suivantf;
}arbres_fuites;

//lecture fichier
int detect_type(char* line); // 1 = source, 2 = usine, 3 = stockage, 4 = jonction, 5 = service, 6 = menages
int code_len(const char *s);
int empty(const char *s);
void next_hash(FILE* file);
void next_semi(FILE* file);
void next_line(FILE* file);

#endif 
