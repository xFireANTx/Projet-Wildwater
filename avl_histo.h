#ifndef AVL_HISTO
	#define AVL_HISTO
	#include <stdio.h>
	
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
	
	Volume_traitement* create_vol_trait(char *id, double volume);
	Volume_reel* create_vol_reel(char *id, double volume, double fuite);
	
	Volume_traitement* ajouter_vol_traitement(Volume_traitement* arbre,char *id, double volume);
	Volume_traitement* ajouter_vol_source(Volume_traitement* arbre,char *id, double volume);
	Volume_reel* ajouter_vol_reel(Volume_reel* arbre, char* id, double volume, double fuite);
	
	void infixe_reel_inverse(Volume_reel* arbre, FILE* sortie);
	void infixe_traitement_inverse(Volume_traitement* arbre,FILE* sortie);
#endif
