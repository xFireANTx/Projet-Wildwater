#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>   // getpid()

#include "basic_functions.h"
#include "avl.h"
#include "avl_histo_reel.h"
#include "avl_histo_traitement.h"

static void strip_newline(char *s) {
    if (!s) return;
    s[strcspn(s, "\r\n")] = '\0';
}

static const char *extract_code_after_hash(const char *full_id) {
    if (!full_id) return NULL;
    const char *p = strchr(full_id, '#');
    if (!p || !*(p + 1)) return NULL;
    return p + 1;
}

/*L'exécutable prend 6 argument: 
Argv attendu : 0: ./exe 
1: fichier_complet.csv 
	2: fichier_entree_filtré.dat 
	3: fichier_sortie.dat
	4: histo | leaks 
	5: max | src | reel  OU  "Facility complex #RH400057F"
On récupère les données qui nous intéresse dans sortie selon l'argument 4 et 5 d'entrée*/
/*pour histo, on construit un avl d'un fichier filtré passé en arg 2, les fichiers histo_max et histo_src étant identique après le filtrage,
 on utilise les mêmes fonctions hormis une différentes pour ces deux cas. Ces fonctions sont contenus dans avl_histo_traitement.
 En ce qui concerne le cas reel et leaks, on a besoin du même fichier de sortie créés à partir de l'avl_histo_reel. Dans le cas de reel
 on renvoie directement le fichier sur la sortie, pour leaks on effectue plus de traitement*/
int main(int argc, char* argv[]) {
    if (argc != 6) {
        fprintf(stderr,
                "Erreur: mauvais nombre d'arguments.\n"
                "Format: ./exe {fichier_complet.csv} {fichier_entree.dat} {fichier_sortie.dat} {histo|leaks} {max|src|reel| 'Facility complex #...'}\n");
        return 1;
    }

    FILE *entree = fopen(argv[2], "r");
    FILE *sortie = fopen(argv[3], "w");
    if (!entree || !sortie) {
        fprintf(stderr, "Erreur ouverture fichier entree/sortie.\n");
        if (entree) fclose(entree);
        if (sortie) fclose(sortie);
        return 1;
    }
	//partie histo 
    if (strcmp(argv[4], "histo") == 0) {

        char ligne_courante[256];
        double vol_courant = 0.0;
        double perte = 0.0;

        if (strcmp(argv[5], "max") == 0) {
            Volume_traitement* arbre_max = NULL;
			//recupere le fichier filtrer et creer un avl
            while (fscanf(entree, "%255[^;];%lf\n", ligne_courante, &vol_courant) == 2) {
                arbre_max = ajouter_vol_traitement(arbre_max, ligne_courante, vol_courant);
            }
			//trie dans l'ordre lexicographique inverse
            infixe_traitement_inverse(arbre_max, sortie);
			freetree_traitement(arbre_max);
        }
        else if (strcmp(argv[5], "src") == 0) {
            Volume_traitement* arbre_src = NULL;

            while (fscanf(entree, "%255[^;];%lf\n", ligne_courante, &vol_courant) == 2) {
                arbre_src = ajouter_vol_source(arbre_src, ligne_courante, vol_courant);
            }
            infixe_traitement_inverse(arbre_src, sortie);
			freetree_traitement(arbre_src);
        }
        else if (strcmp(argv[5], "reel") == 0) {
            Volume_reel* arbre_reel = NULL;

            while (fscanf(entree, "%255[^;];%lf;%lf\n", ligne_courante, &vol_courant, &perte) == 3) {
                arbre_reel = ajouter_vol_reel(arbre_reel, ligne_courante, vol_courant, perte);
            }
            infixe_reel_inverse(arbre_reel, sortie);
			freetree_reel(arbre_reel);
        }
        else {
            fprintf(stderr, "Erreur: mode histo attendu: max | src | reel\n");
            fclose(entree);
            fclose(sortie);
            return 1;
        }

        fclose(entree);
        fclose(sortie);
        return 0;
    }

	//partie leaks
    if (strcmp(argv[4], "leaks") == 0) {
		//Même processus que dans histo reel
        FILE *fichier = fopen(argv[1], "r");
        if (!fichier) {
            fprintf(stderr, "Erreur ouverture fichier complet: %s\n", argv[1]);
            fclose(entree);
            fclose(sortie);
            return 1;
        }

        char tmpname[64];
        snprintf(tmpname, sizeof(tmpname), "temporaire_reel_%ld.tmp", (long)getpid());

        FILE *histo_reel = fopen(tmpname, "w");
        if (!histo_reel) {
            fprintf(stderr, "Erreur création fichier temporaire.\n");
            fclose(fichier);
            fclose(entree);
            fclose(sortie);
            return 1;
        }

        char ligne_courante[256];
        double vol_courant = 0.0;
        double perte = 0.0;

        Volume_reel* arbre_reel = NULL;
        while (fscanf(entree, "%255[^;];%lf;%lf\n", ligne_courante, &vol_courant, &perte) == 3) {
            arbre_reel = ajouter_vol_reel(arbre_reel, ligne_courante, vol_courant, perte);
        }

        infixe_reel_inverse(arbre_reel, histo_reel);
		freetree_reel(arbre_reel);
        fclose(histo_reel);
        fclose(entree);
        entree = NULL;

		//Nouveau traitement à partir du fichier obtenu
        FILE *flux = fopen(tmpname, "r");
        if (!flux) {
            fprintf(stderr, "Erreur réouverture du fichier temporaire.\n");
            fclose(fichier);
            fclose(sortie);
            remove(tmpname);
            return 1;
        }
		
        arbre *root = NULL;
        char ligne_flux[256];
        char tmp[256];

        while (fgets(ligne_flux, sizeof(ligne_flux), flux)) {
            strip_newline(ligne_flux);
            strncpy(tmp, ligne_flux, sizeof(tmp) - 1);
            tmp[sizeof(tmp) - 1] = '\0';
            root = ajouter_avl_flux(root, tmp);
        }

        fclose(flux);
        flux = NULL;

        if (remove(tmpname) != 0) {
            perror("Warning: suppression fichier temporaire");
        }
        char ligne[512];

        while (fgets(ligne, sizeof(ligne), fichier)) {
            strncpy(tmp, ligne, sizeof(tmp) - 1);
            tmp[sizeof(tmp) - 1] = '\0';

            int type = detect_type(tmp);
            if (type == 0 || type == 1 || type == 2) {
                continue; // ignorés ici
            }

            arbres_fuites *p1 = createNode(ligne, type);
            if (!p1 || !p1->structure) {
                continue;
            }

            racine *r = chercher_avl(p1->structure->code_usine, root);
            if (r) {
                ajouter_arbre_usine(r, p1);
            } else {
                // usine non trouvée dans l'AVL flux
            }
        }

        fclose(fichier);
        fichier = NULL;
		

        float total_fuite = -1.0f;

        const char *code = extract_code_after_hash(argv[5]);
        if (code) {
            racine *ru = chercher_avl(code, root);
            if (ru) {
                calcule_fuites(ru);
                total_fuite = recuperer_fuites(ru);
            } else {
                total_fuite = -1.0f;
            }
        } else {
            total_fuite = -1.0f;
        }

        fprintf(sortie, "%s;%f\n", argv[5], total_fuite);

        fclose(sortie);
        return 0;
    }

    fprintf(stderr, "Erreur: 4e argument attendu: histo | leaks\n");

    fclose(entree);
    fclose(sortie);
    return 1;
}
