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
    int capa_max;
    float fuite;
}infra;

typedef struct Arbres_fuites{
    infra *structure;
    struct Arbres_fuites *premierf;
    struct Arbres_fuites *suivantf;
}arbres_fuites;

typedef struct Arbre{
    arbres_fuites *arbre_fuites; //pour trier l arbre utilise arbre->arbre_fuites->infra->code_usine
    struct Arbre *fg;
    struct Arbre *fd;
}arbre;

arbre *rotation_droite(arbre* a);
arbre *rotation_gauche(arbre* a);
arbre *rotation_droite_gauche(arbre *a);
arbre *rotation_gauche_droite(arbre *a);


infra *remplir_infra(char *line, int type);         //marche pour 3,4,5,6 jsp si je veut utiliser la meme chose pour usine et source n est pas utile
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
