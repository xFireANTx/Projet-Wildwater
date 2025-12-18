#ifndef BASIC_FUNCTIONS_H_  
#define BASIC_FUNCTIONS_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//arbres

//traitement
typedef struct Infra{
    int type;
    char code_usine[11];
    char code_precedent[11];
    char code_actuel[11];
    float fuite;
    float flux; // pas besoin de capa max
}infra;

typedef struct Arbres_fuites{
    infra *structure;
    struct Arbres_fuites *premierf;
    struct Arbres_fuites *suivantf;
}arbres_fuites;

typedef struct Racine{
    char code_usine[11];
    float flux;
    struct Arbres_fuites *premierf;
    struct Arbres_fuites *suivantf;
}racine;

typedef struct Arbre{
    racine *usine; //pour trier l arbre utilise arbre->racine->code usine       
    struct Arbre *fg;
    struct Arbre *fd;
}arbre;

infra *remplir_infra(char *line, int type);         //marche pour 3,4,5,6 jsp si je veut utiliser la meme chose pour usine et source n est pas utile
void freeTree(arbres_fuites *node);
void addChild(arbres_fuites *parent, arbres_fuites *child);
arbres_fuites *createNode(char *line, int type);

//lecture fichier
int detect_type(char* line); // 1 = source, 2 = usine, 3 = stockage, 4 = jonction, 5 = service, 6 = menages
int code_len(const char *s);
int empty(const char *s);
void next_hash(FILE* file);
void next_semi(FILE* file);
void next_line(FILE* file);

#endif 
