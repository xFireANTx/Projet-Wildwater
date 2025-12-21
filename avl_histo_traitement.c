#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl_histo_traitement.h"
//Les fonctions utilisés ici sont très semblable à celle d'avl_histo_reel.c. Référez-vous donc aux commentaires d'avl_histo_reel.c
int max_traitement(int a, int b){
	return (a > b)? a:b ;
}

Volume_traitement* create_vol_trait(char *id, double volume){
	if(id == NULL){
		printf("vol_trait: id n'est pas définit\n");
		exit(1);
	}

	Volume_traitement* nouv = malloc(sizeof(Volume_traitement));
	if(nouv == NULL){
		printf("Erreur allocation memoire vol_trait\n");
		exit(1);
	}
	
	int longueur_id = strlen(id);
	nouv->identifiant = malloc((longueur_id+1)*sizeof(char));
	if(nouv->identifiant == NULL){
		printf("vol_trait: Erreur allocation memoire identifiant\n");
		exit(1);
	}
	strcpy(nouv->identifiant,id);
	nouv->volume = volume/1000;
	nouv->fg = NULL;
	nouv->fd = NULL;
	nouv->hauteur = 1;
	
	return nouv;
}



Volume_traitement* ajouter_vol_traitement(Volume_traitement* arbre,char *id, double volume){
	if(id == NULL){
		printf("ajou_vol_trait: Erreur id vide\n"); 
		exit(1);
	}
	
	if(arbre == NULL){
		return create_vol_trait(id, volume);
	}
	int cmp= strcmp(arbre->identifiant,id);
	if( cmp < 0){
		arbre->fg = ajouter_vol_traitement(arbre->fg, id, volume);
	}
	else if(cmp > 0){
		arbre->fd = ajouter_vol_traitement(arbre->fd, id, volume);
	}
	return arbre ; 
}

Volume_traitement* ajouter_vol_source(Volume_traitement* arbre,char *id, double volume){
	if(id == NULL){
		printf("ajou_vol_source: Erreur id vide\n"); 
		exit(1);
	}
	
	if(arbre == NULL){
		return create_vol_trait(id, volume);
	}
	
	int comp_id = strcmp(arbre->identifiant,id);
	
	if(!comp_id){
		arbre->volume += (volume /1000.0);
	}
	else if(comp_id < 0){
		arbre->fg = ajouter_vol_source(arbre->fg, id, volume);
	}
	else {
		arbre->fd = ajouter_vol_source(arbre->fd, id, volume);
	}
	return arbre ; 
}

void infixe_traitement_inverse(Volume_traitement* arbre,FILE* sortie){
	if(arbre == NULL){
		return;
	}
	infixe_traitement_inverse(arbre->fg,sortie);
	fprintf(sortie,"%s;%lf\n",arbre->identifiant,arbre->volume);
	infixe_traitement_inverse(arbre->fd,sortie);
}


int hauteur_traitement(Volume_traitement *n){
    if(n == NULL) return 0;
    return n->hauteur;
}

Volume_traitement *rotation_droite_traitement(Volume_traitement *a){

    if (a == NULL ||a->fg == NULL) return a;
    Volume_traitement *b = a->fg;
    Volume_traitement *temp = b->fd;
    b->fd = a;
    a->fg = temp;
    
    a->hauteur = 1 + max_traitement(hauteur_traitement(a->fg), hauteur_traitement(a->fd));
    b->hauteur = 1 + max_traitement(hauteur_traitement(b->fg), hauteur_traitement(b->fd));
    return b;
}

Volume_traitement *rotation_gauche_traitement(Volume_traitement *a){

    if (a == NULL ||a->fd == NULL) return a;
    Volume_traitement *b = a->fd;
    Volume_traitement *temp = b->fg;
    b->fg = a;
    a->fd = temp;
    
    a->hauteur = 1 + max_traitement(hauteur_traitement(a->fg), hauteur_traitement(a->fd));
    b->hauteur = 1 + max_traitement(hauteur_traitement(b->fg), hauteur_traitement(b->fd));
    return b;
}

Volume_traitement *rotation_droite_gauche_traitement(Volume_traitement *a){
    a->fd = rotation_droite_traitement(a->fd);
    return rotation_gauche_traitement(a);
}

Volume_traitement *rotation_gauche_droite_traitement(Volume_traitement *a){
    a->fg = rotation_gauche_traitement(a->fg);
    return rotation_droite_traitement(a);
}

int facteur_equilibre_traitement(Volume_traitement *n){
    if (n == NULL)
        return 0;

    return hauteur_traitement(n->fg) - hauteur_traitement(n->fd);
}

Volume_traitement *equilibrer_traitement(Volume_traitement *n){
    if (n == NULL)
        return NULL;
    n->hauteur = 1 + max_traitement(hauteur_traitement(n->fg), hauteur_traitement(n->fd));
    int balance = facteur_equilibre_traitement(n);

    if (balance > 1 && facteur_equilibre_traitement(n->fg) >= 0){
        return rotation_droite_traitement(n);
    }

    if (balance > 1 && facteur_equilibre_traitement(n->fg) < 0){
    return rotation_gauche_droite_traitement(n);
    }

    if (balance < -1 && facteur_equilibre_traitement(n->fd) <= 0){
        return rotation_gauche_traitement(n);
    }
    if (balance < -1 && facteur_equilibre_traitement(n->fd) > 0){
        return rotation_droite_gauche_traitement(n);
    }

    return n; // déjà équilibré
}
