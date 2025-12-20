#ifndef AVL_HISTO
	#define AVL_HISTO
#ifndef AVL_HISTO_REEL
	#define AVL_HISTO_REEL
	
	typedef struct arbre_reel{
		char* identifiant;
		double volume;
		double fuite;
		struct arbre_reel* fg;
		struct arbre_reel* fd;
		int hauteur;
	}Volume_reel;
		
	Volume_reel* create_vol_reel(char *id, double volume, double fuite);
	Volume_reel* ajouter_vol_reel(Volume_reel* arbre, char* id, double volume, double fuite);	
	void infixe_reel_inverse(Volume_reel* arbre, FILE* sortie);
	
	int max_reel(int a, int b);
	int hauteur_reel(Volume_reel *n);
	int facteur_equilibre_reel(Volume_reel *n);
	Volume_reel *equilibrer_reel(Volume_reel *n);
	
	Volume_reel *rotation_droite_reel(Volume_reel *a);
	Volume_reel *rotation_gauche_reel(Volume_reel *a);
	Volume_reel *rotation_droite_gauche_reel(Volume_reel *a);
	Volume_reel *rotation_gauche_droite_reel(Volume_reel *a);
	

#endif

#endif
