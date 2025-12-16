#include <stdio.h>
#include <stdlib.h>
#include "basic_functions.h"
#include <string.h>

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

junction = (nom #code usine (10);nom #code stockage(6);nom #code jonction(9);        ;fuite)
service =  (nom #code usine (10);nom #code jonction(9);nom #code service(9) ;        ;fuite)
menage =   (nom #code usine (10);nom #code service(9) ;nom #code menage(10) ;        ;fuite)
source =   (                    ;nom #code source(10) ;nom #code usine(10)  ;capa_max;fuite)
storage =  (                    ;nom #code usine (10) ;nom #code stockage(6);        ;fuite)
usine =    (                    ;nom #code usine (10) ;                     ;capa_max;     )*/

usine *remplir_usine(FILE* file){
    usine *new = malloc(sizeof(usine));
    if(new == NULL){ 
        printf("erreur d'allocation memoire");
        exit(1);
    }

    next_semi(file);                        //vide

    next_hash(file);
    for(int i = 0; i<10; i++){
        new->code_u[i] = fgetc(file);
    }                                       //code iden 1
    next_semi(file);

    next_semi(file);                        //case vide

    fscanf(file, "%d", new->capa_max);      //capa max
    next_semi(file); 

    next_line(file);                        //case vide
    return new;
}

jonction *remplir_jonction(FILE* file){
    jonction *new = malloc(sizeof(jonction));
    if(new == NULL){ 
        printf("erreur d'allocation memoire");
        exit(1);
    }

    next_hash(file);
    for(int i = 0; i<10; i++){
        new->code_u[i] = fgetc(file);
    }                                     //code iden 1
    next_semi(file);

    next_hash(file);
    for(int i = 0; i<6; i++){
        new->code_st[i] = fgetc(file);
    }                                     //code iden 2
    next_semi(file);

    next_hash(file);
    for(int i = 0; i<9; i++){
        new->code_j[i] = fgetc(file);
    }                                     //code iden 3
    next_semi(file);

    next_semi(file);                      //case vide

    fscanf(file, "%f", new->fuite);       //fuites
    next_line(file);
    return new;
}

service *remplir_service(FILE* file){
    service *new = malloc(sizeof(service));
    if(new == NULL){ 
        printf("erreur d'allocation memoire");
        exit(1);
    }

    next_hash(file);
    for(int i = 0; i<10; i++){
        new->code_u[i] = fgetc(file);
    }                                     //code iden 1
    next_semi(file);

    next_hash(file);
    for(int i = 0; i<9; i++){
        new->code_j [i] = fgetc(file);
    }                                     //code iden 2
    next_semi(file);

    next_hash(file);
    for(int i = 0; i<9; i++){
        new->code_s[i] = fgetc(file);
    }                                     //code iden 3
    next_semi(file);

    next_semi(file);                      //case vide

    fscanf(file, "%f", new->fuite);       //fuites
    next_line(file);
    return new;
}

menage *remplir_menage(FILE* file){
    menage *new = malloc(sizeof(menage));
    if(new == NULL){ 
        printf("erreur d'allocation memoire");
        exit(1);
    }

    next_hash(file);
    for(int i = 0; i<10; i++){
        new->code_u[i] = fgetc(file);
    }                                     //code iden 1
    next_semi(file);

    next_hash(file);
    for(int i = 0; i<9; i++){
        new->code_s [i] = fgetc(file);
    }                                     //code iden 2
    next_semi(file);
    
    next_hash(file);
    for(int i = 0; i<9; i++){
        new->code_m[i] = fgetc(file);
    }                                     //code iden 3
    next_semi(file);
    
    next_semi(file);                      //case vide

    fscanf(file, "%f", new->fuite);       //fuites
    next_line(file);
    return new;
}

storage *remplir_storage(FILE* file){
    storage *new = malloc(sizeof(storage));
    if(new == NULL){ 
        printf("erreur d'allocation memoire");
        exit(1);
    }
    next_semi(file);                      //case vide
    next_hash(file);
    for(int i = 0; i<10; i++){
        new->code_u[i] = fgetc(file);
    }                                     //code iden 1
    next_semi(file);
    next_hash(file);
    for(int i = 0; i<6; i++){
        new->code_st [i] = fgetc(file);
    }                                     //code iden 2
    next_semi(file);                   

    next_semi(file);                      //case vide

    fscanf(file, "%f", new->fuite);       //fuites
    next_line(file);
    return new;
}

source *remplir_source(FILE* file){
    source *new = malloc(sizeof(source));
    if(new == NULL){ 
        printf("erreur d'allocation memoire");
        exit(1);
    }
    next_semi(file);                      //case vide
    next_hash(file);
    for(int i = 0; i<10; i++){
        new->code_w[i] = fgetc(file);
    }                                     //code iden 1
    next_semi(file);
    next_hash(file);
    for(int i = 0; i<10; i++){
        new->code_u [i] = fgetc(file);
    }                                     //code iden 2
    next_semi(file);               

    fscanf(file, "%d", new->capa_max); //capa max
    next_semi(file);

    fscanf(file, "%f", new->fuite);       //fuites
    next_line(file);
    return new;
}

int empty(const char *s) {
    return s == NULL || strcmp(s, "-") == 0;
}

int code_len(const char *s) {
    const char *p = strchr(s, '#');
    if (!p) return 0;
    return strlen(p + 1);
}

int detect_type(char *line) {
    char *col[5] = {0};
    int i = 0;

    char *tok = strtok(line, ";");
    while (tok && i < 5) {
        col[i++] = tok;
        tok = strtok(NULL, ";");
    }

    int l1 = empty(col[0]) ? 0 : code_len(col[0]);
    int l2 = empty(col[1]) ? 0 : code_len(col[1]);
    int l3 = empty(col[2]) ? 0 : code_len(col[2]);
    int c4 = !empty(col[3]);
    if (l1 == 0 && l2 == 9 && l3 == 9 && c4) //source
        return 1;

    if (l1 == 0 && l2 == 9 && l3 == 0 && c4) //usine
        return 2;

    if (l1 == 0 && l2 == 9 && l3 == 5) // storage
        return 3;

    if (l1 == 9 && l2 == 5 && l3 == 8) // jonction
        return 4;

    if (l1 == 9 && l2 == 8 && l3 == 9) // service
        return 5;

    if (l1 == 9 && l2 == 9 && l3 == 10) // menage
        return 6;
    return 0;
}



void next_hash(FILE* file){         //deplace curseur vers prochain (#) (pour skip les noms)
    if(file == NULL){exit(1);}
    int ch;
    while((ch = getc(file)) != '#'){
        if(ch == EOF || ch == '\n'){exit(1);}
    }
}

void next_semi(FILE* file){         //deplace curseur vers prochain (;) (changer de colonne)
    if(file == NULL){exit(1);}
    int ch;
    while((ch = getc(file)) != ';'){
        if(ch == EOF || ch == '\n'){exit(1);}
    }
}

void next_line(FILE* file){         //deplace curseur vers prochain retour a la ligne (changer de ligne)
    if(file == NULL){exit(1);}
    int ch;
    while((ch = getc(file)) != '\n'){
        if(ch == EOF){exit(1);}
    }
}

