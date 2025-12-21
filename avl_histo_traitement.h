#ifndef AVL_HISTO_TRAIT
	#define AVL_HISTO_TRAIT
	

	typedef struct arbre_volume_traitement{
		char* identifiant;
		double volume;
		struct arbre_volume_traitement* fg;
		struct arbre_volume_traitement* fd;
		int hauteur;
	}Volume_traitement;
	
	Volume_traitement* create_vol_trait(char *id, double volume);
	Volume_traitement* ajouter_vol_traitement(Volume_traitement* arbre,char *id, double volume);
	Volume_traitement* ajouter_vol_source(Volume_traitement* arbre,char *id, double volume);
	
	void infixe_traitement_inverse(Volume_traitement* arbre,FILE* sortie);	
	
	int max_traitement(int a, int b);
	int hauteur_traitement(Volume_traitement *n);
	int facteur_equilibre_traitement(Volume_traitement *n);
	Volume_traitement *equilibrer_traitement(Volume_traitement *n);

	Volume_traitement *rotation_droite_traitement(Volume_traitement *a);
	Volume_traitement *rotation_gauche_traitement(Volume_traitement *a);
	Volume_traitement *rotation_droite_gauche_traitement(Volume_traitement *a);
	Volume_traitement *rotation_gauche_droite_traitement(Volume_traitement *a);

	void freetree_traitement(Volume_traitement* arbre);



#endif
