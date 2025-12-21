#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl_histo_reel.h"

int max_reel(int a, int b){
	return (a > b)? a:b ;
}

int hauteur_reel(Volume_reel *n){
    if(n == NULL) return 0;
    return n->hauteur;
}

int facteur_equilibre_reel(Volume_reel *n){
    if (n == NULL)
        return 0;

    return hauteur_reel(n->fg) - hauteur_reel(n->fd);
}

Volume_reel* create_vol_reel(char *id, double volume, double fuite){
	if(id == NULL){
		printf("vol_reel: id n'est pas définit\n");
		exit(1);
	}

	Volume_reel* nouv = malloc(sizeof(Volume_reel));
	if(nouv == NULL){
		printf("Erreur allocation memoire vol_reel\n");
		exit(1);
	}
	
	int longueur_id = strlen(id);
	nouv->identifiant = malloc((longueur_id+1)*sizeof(char));
	if(nouv->identifiant == NULL){
		printf("vol_reel: Erreur allocation memoire identifiant\n");
		exit(1);
	}
	strcpy(nouv->identifiant,id);
	nouv->fuite = fuite;
	nouv->volume = volume*(1.0-(fuite/100.0))/1000.0;
	nouv->fg = NULL;
	nouv->fd = NULL;
	nouv->hauteur = 1;
	
	return nouv;
}

Volume_reel *rotation_droite_reel(Volume_reel *a){

	if (a == NULL ||a->fg == NULL) return a;
    Volume_reel *b = a->fg;
    Volume_reel *temp = b->fd;
    b->fd = a;
    a->fg = temp;
    
    a->hauteur = 1 + max_reel(hauteur_reel(a->fg), hauteur_reel(a->fd));
    b->hauteur = 1 + max_reel(hauteur_reel(b->fg), hauteur_reel(b->fd));
    return b;
}

Volume_reel *rotation_gauche_reel(Volume_reel *a){

	if (a == NULL ||a->fd == NULL) return a;
    Volume_reel *b = a->fd;
    Volume_reel *temp = b->fg;
    b->fg = a;
    a->fd = temp;
    
    a->hauteur = 1 + max_reel(hauteur_reel(a->fg), hauteur_reel(a->fd));
    b->hauteur = 1 + max_reel(hauteur_reel(b->fg), hauteur_reel(b->fd));
    return b;
}

Volume_reel *rotation_droite_gauche_reel(Volume_reel *a){
    a->fd = rotation_droite_reel(a->fd);
    return rotation_gauche_reel(a);
}

Volume_reel *rotation_gauche_droite_reel(Volume_reel *a){
    a->fg = rotation_gauche_reel(a->fg);
    return rotation_droite_reel(a);
}

Volume_reel *equilibrer_reel(Volume_reel *n){
    if (n == NULL)
        return NULL;
	
    int balance = facteur_equilibre_reel(n);

    if (balance > 1 && facteur_equilibre_reel(n->fg) >= 0){
        return rotation_droite_reel(n);
    }

    if (balance > 1 && facteur_equilibre_reel(n->fg) < 0){
    	return rotation_gauche_droite_reel(n);
    }

    if (balance < -1 && facteur_equilibre_reel(n->fd) <= 0){
        return rotation_gauche_reel(n);
    }
    if (balance < -1 && facteur_equilibre_reel(n->fd) > 0){
        return rotation_droite_gauche_reel(n);
    }

    return n; // déjà équilibré
}


Volume_reel* ajouter_vol_reel(Volume_reel* arbre, char* id, double volume, double fuite){

	if(id == NULL){
		printf("ajou_vol_reel: Erreur id vide\n"); 
		exit(1);
	}
	
	if(arbre == NULL){
		return create_vol_reel(id, volume, fuite);
	}
	
	int comp_id = strcmp(arbre->identifiant,id);
	
	if(!comp_id){
		arbre->volume += (volume*(1-(fuite/100.0)) / 1000.0);
		return arbre;
	}
	else if(comp_id < 0){
		arbre->fg = ajouter_vol_reel(arbre->fg, id, volume, fuite);
	}
	else {
		arbre->fd = ajouter_vol_reel(arbre->fd, id, volume, fuite);
	}

	arbre->hauteur = 1 + max_reel(hauteur_reel(arbre->fg), hauteur_reel(arbre->fd));
	return equilibrer_reel(arbre); 
}


void infixe_reel_inverse(Volume_reel* arbre, FILE* sortie){
	if(arbre == NULL){
		return;
	}
	infixe_reel_inverse(arbre->fg,sortie);
	fprintf(sortie,"%s;%lf\n",arbre->identifiant,arbre->volume);
	infixe_reel_inverse(arbre->fd,sortie);
}



