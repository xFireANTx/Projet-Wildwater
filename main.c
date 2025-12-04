#include <stdio.h>

int meilleur_scanf(int *a,int min,int max){
  do{
      if(scanf("%d",a) != 1){
          printf("Mauvaise valeur entree\n");
          a=min-1;
      }
  }while(*a >max || *a<min);
  return a;
}
