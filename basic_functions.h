#ifndef BASIC_FUNCTIONS_H_  
#define BASIC_FUNCTIONS_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct arbre{
    char* identifiant;
    struct arbre *fg;
    struct arbre *fd;
}Arbre;

Arbre *rotation_droite(Arbre* a);
Arbre *rotation_gauche(Arbre* a);
Arbre *rotation_droite_gauche(Arbre *a);
Arbre *rotation_gauche_droite(Arbre *a);


typedef struct Source{
    
    char code_w[10];
    char code_u[10];
    int capa_max;
    float fuite;
}source;
source *remplir_source(FILE* file);

typedef struct Usine{

    char code_u[10];

    int capa_max;

}usine;
usine *remplir_usine(FILE* file);

typedef struct Storage{

    char code_u[10];
    char code_st[6];

    float fuite;
}storage;
storage *remplir_storage(FILE* file);

typedef struct Jonction{
    char code_u[10];
    char code_st[6];
    char code_j[9];

    float fuite;
}jonction;
jonction *remplir_jonction(FILE* file);

typedef struct Service{
    char code_u[10];
    char code_j[9];
    char code_s[9];

    float fuite;
}service;
service *remplir_service(FILE* file);

typedef struct Menage{
    char code_u[10];
    char code_s[9];
    char code_m[10];

    float fuite;
}menage;
menage *remplir_menage(FILE* file);


int detect_type(char* line); // 1 = source, 2 = usine, 3 = stockage, 4 = jonction, 5 = service, 6 = menages
int code_len(const char *s);
int empty(const char *s);
void next_hash(FILE* file);
void next_semi(FILE* file);
void next_line(FILE* file);

#endif 
