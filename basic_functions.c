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
junction = branchement 1 des tuyaux (stockage -> service)  (nom #code usine (10);nom #code stockage(6);nom #code jonction(9);        ;fuite)
service = branchement 2 des tuyaux (service -> menage)     (nom #code usine (10);nom #code jonction(9);nom #code service(10);        ;fuite)
well/ well field/ fountain/ resurgence = source            (                    ;nom #code source(10) ;nom #code usine  (10);capa_max;fuite)
storage = endroit de stockage                              (                    ;nom #code usine (10) ;nom #code stockage(6);        ;fuite)
unit/ module/ plant/ = usine                               (                    ;nom #code usine (10) ;                     ;capa_max;     )
cust = menages








*/
//unit/ module/ plant/ = usine 
//(nom + #code;vide;capa_max;vide)
typedef struct Usine{
    char code_u[10];
    int capa_max;
}usine;

//junction = branchement 1 des tuyaux 
//(stockage -> services) (nom + #code usine; nom + code stockage; code jonction; vide; fuite)
typedef struct Jonction{
    char code_u[10];
    char code_st[6];
    char code_j[9];
    float fuite;
}jonction;

//service = branchement 2 des tuyaux 
//(service -> menage) (nom + #code usine; nom + code jonction; code service; vide; fuite)
typedef struct Service{
    char code_u[10];
    char code_j[9];
    char code_s[10];
    float fuite;
}service;

//storage = endroit de stockage 
//(usine -> stockage) (nom + #code usine; nom + #code stockage; vide; fuite)
typedef struct Storage{
    char code_u[10];
    char code_st[6];
    float fuite;
}storage;

//source/ well/ well field/ fountain/ resurgence = la source d'eau 
//(source -> usine) (nom + #code source; nom + #code usine; capa_max; fuite)
typedef struct Source{
    char code_w[10];
    char code_u[10];
    int capa_max;
    float fuite;
}source;

//lecture fichier csv
int recup_type(FILE* file){  
    if(file == NULL){exit (1);}
    int ch;
    int count_hash = 0; //compteur (#)
    int count_semi = 0; //compteur (;)
    int col2 = 0;      //compteur (longueur code colonne 2)
    int col3 = 0;      //compteur (longueur code colonne 3)
    long pos = ftell(file);
    while(ch = getc(file) != EOF && ch != '\n'){
        if(ch == '#'){
            count_hash++;
        }
        if(ch == ';'){
            count_semi++;
        }
        if(count_hash == 2 && count_semi == 1){
            col2++;
        }
        if(count_hash == 2 && count_semi == 2){
            col3++;
        }
    }
    fseek(file, pos, SEEK_SET); // SEEK_SET -> fseek cherche par rapport au debut du doc (SEEK_CUR; a partir du curseur/SEEK_END; a partir de la fin;)
    if(count_hash == 1){
        return ?; //valeur pour usine
    }
    if(count_hash == 2){
        if(col3 == 6){
            return ?; //valeur pour stackage
        }
        if(col3 == 10){
            return ?; //valeur pour source
        }
        else{
            printf("erreur de categorisation");
            exit(1);
        }
    }
    if(count_hash == 3){
        if(col2 == 6){
            return ?; //valeur pour jonction
        }
        if(col2 == 9){
            return ?; //valeur pour service
        }
        else{
            printf("erreur de categorisation");
            exit(1);  
        }
    }
    else{
        printf("erreur de categorisation");
        exit(1); 
    }
}

void next_hash(FILE* file){         //deplace curseur vers prochain (#) (pour skip les noms)
    if(file == NULL){exit(1);}
    int ch;
    while((ch = getc(file)) != '#'){
        if(ch == EOF || ch == '\n'){exit(1);}
    }
}

void next_semi(FILE* file){         //deplace curseur vers prochain (;)
    if(file == NULL){exit(1);}
    int ch;
    while((ch = getc(file)) != ';'){
        if(ch == EOF || ch == '\n'){exit(1);}
    }
}

