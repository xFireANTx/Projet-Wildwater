#ifndef BASIC_FUNCTIONS_H_  
#define BASIC_FUNCTIONS_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//arbres
typedef struct arbre{
    arbres_fuites *arbre_fuites; //pour trier l arbre utilise arbre->arbre_fuites->infra->code_usine
    struct arbre *fg;
    struct arbre *fd;
}Arbre;

Arbre *rotation_droite(Arbre* a);
Arbre *rotation_gauche(Arbre* a);
Arbre *rotation_droite_gauche(Arbre *a);
Arbre *rotation_gauche_droite(Arbre *a);

//traitement
typedef struct Infra{
    int type;
    char code_usine[11];
    char code_precedent[11];
    char code_actuel[11];
    int capa_max;
    float fuite;
}infra;

typedef struct Arbre_fuites{
    infra *structure;
    struct Arbre_fuites *premierf;
    struct Arbre_fuites *suivantf;
}arbres_fuites;

infra *remplir_infra(char *line, int type);
void freeTree(arbres_fuites *node);
void addChild(arbres_fuites *parent, arbres_fuites *child);
arbres_fuites *createNode(infra *new);

//lecture fichier
int detect_type(char* line); // 1 = source, 2 = usine, 3 = stockage, 4 = jonction, 5 = service, 6 = menages
int code_len(const char *s);
int empty(const char *s);
void next_hash(FILE* file);
void next_semi(FILE* file);
void next_line(FILE* file);

#endif 
