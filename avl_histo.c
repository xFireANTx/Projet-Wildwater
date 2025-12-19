#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*On utilise la même structure pour construire l'avl volume_traitement et volume_source car les fichiers ont exactement le même format la différence se fera au moment de la contruction des avl (plus précisément dans la partie ajoutez un noeud à l'arbre)*/
typedef struct arbre_volume_traitement{
	char* identifiant;
	double volume;
	struct arbre_volume_traitement* fg;
	struct arbre_volume_traitement* fd;
}Volume_traitement;

typedef struct arbre_reel{
	char* identifiant;
	double volume;
	double fuite;
	struct arbre_reel* fg;
	struct arbre_reel* fd;
}Volume_reel;


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
	
	return nouv;
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
	nouv->volume = volume/1000.0;
	nouv->fuite = fuite;
	nouv->fg = NULL;
	nouv->fd = NULL;
	
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
	}
	else if(comp_id < 0){
		arbre->fg = ajouter_vol_reel(arbre->fg, id, volume, fuite);
	}
	else {
		arbre->fd = ajouter_vol_reel(arbre->fd, id, volume, fuite);
	}
	return arbre ; 
}


void infixe_reel_inverse(Volume_reel* arbre, FILE* sortie){
	if(arbre == NULL){
		return;
	}
	infixe_reel_inverse(arbre->fg,sortie);
	fprintf(sortie,"%s;%.5lf\n",arbre->identifiant,arbre->volume);
	infixe_reel_inverse(arbre->fd,sortie);
}

void infixe_traitement_inverse(Volume_traitement* arbre,FILE* sortie){
	if(arbre == NULL){
		return;
	}
	infixe_traitement_inverse(arbre->fg,sortie);
	fprintf(sortie,"%s;%.5lf\n",arbre->identifiant,arbre->volume);
	infixe_traitement_inverse(arbre->fd,sortie);
}
