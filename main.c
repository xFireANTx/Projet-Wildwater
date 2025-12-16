#include <stdio.h>
#include <stdlib.h>
#include "basic_functions.h"
#include <string.h>

void main(){
    FILE *fichier = fopen("c-wildwater_v0.dat", "r");
    int boucle_principale = 0;
    int type = 0;
    char line[256];
    char tmp[256];
    while(boucle_principale == 0){
        while(fgets(line, sizeof(line), fichier)){
            strcpy(tmp, line); // strtok modifie la ligne
            int type = detect_type(tmp);
            printf("Type = %d\n", type);
        }
        switch(type){
            case(0):
                printf("end\n");
                return;
            break;
            case(1):// source
                printf("1\n");
                //remplir_source(fichier);
            break;
            case(2):// usine
                printf("2\n");
                //remplir_usine(fichier);
            break;
            case(3):// stockage
                printf("3\n");
                //remplir_storage(fichier);
            break;
            case(4)://jonction
                printf("4\n");
                //remplir_jonction(fichier);
            break;
            case(5)://service
                printf("5\n");
                //remplir_service(fichier);
            break;
            case(6)://menage
                printf("6\n");
                //remplir_menage(fichier);
            break;
        }
    }    
    fclose(fichier);
}

char line[256];


