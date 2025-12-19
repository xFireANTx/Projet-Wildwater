#!/bin/bash

# 1. Démarrage du chronomètre (en millisecondes)

start_time=$(date +%s%3N)

# Fonction pour afficher la durée et quitter proprement
fin_script() {
    end_time=$(date +%s%3N)
    duration=$((end_time - start_time))
    echo "Durée totale : ${duration} ms"
    exit $1
}

# 2. Vérification de l'aide
if [ "$1" == "-h" ]; then
    echo "Usage : $0 <fichier_donnees> <commande> [parametre]"
    echo "Commandes :"
    echo "  histo max|src|real : Génère l'histogramme correspondant"
    echo "  leaks <identifiant>: Analyse les fuites pour une usine donnée"
    fin_script 0
fi

# 3. Vérification du nombre d'arguments
if [ $# -lt 2 ] || [ $# -gt 3 ]; then
    echo "Erreur : Nombre d'arguments incorrect."
    echo "Usage : $0 <fichier.csv> <histo|leaks> [parametre]"
    fin_script 1
fi

fichier_data="$1"
commande="$2"
parametre="$3"

# 4. Vérification de l'existence du fichier de données
if [ ! -f "$fichier_data" ]; then
    echo "Erreur : Le fichier '$fichier_data' est introuvable."
    fin_script 1
fi

# 5. Vérification de la compilation (Consigne explicite)
# Si l'exécutable n'existe pas, on lance make
if [ ! -x "c-wildwater" ]; then
    echo "Exécutable introuvable. Compilation en cours..."
    make
    # Vérification si la compilation a réussi
    if [ $? -ne 0 ]; then
        echo "Erreur : La compilation a échoué."
        fin_script 1
    fi
fi

# 6. Traitement des arguments et lancement du C
case "$commande" in
    "histo")
        # Vérification du 3ème argument obligatoire pour histo
        if [[ "$parametre" != "max" && "$parametre" != "src" && "$parametre" != "real" ]]; then
            echo "Erreur : Pour 'histo', le 3ème argument doit être 'max', 'src' ou 'real'."
            fin_script 1
        fi
        
        # Appel du programme C (on passe les 3 arguments)
        ./c-wildwater "$fichier_data" "histo" "$parametre"
        code_retour=$?
        ;;

    "leaks")
        # Vérification du 3ème argument obligatoire pour leaks (l'identifiant)
        if [ -z "$parametre" ]; then
            echo "Erreur : Pour 'leaks', vous devez fournir un identifiant d'usine."
            fin_script 1
        fi

        # Appel du programme C
        ./c-wildwater "$fichier_data" "leaks" "$parametre"
        code_retour=$?
        ;;

    *)
        echo "Erreur : Commande '$commande' inconnue. Utilisez 'histo' ou 'leaks'."
        fin_script 1
        ;;
esac

# 7. Vérification du retour du programme C
if [ $code_retour -ne 0 ]; then
    echo "Erreur : Le programme C a rencontré un problème."
    fin_script 1
fi

echo "Traitement terminé avec succès."

# 8. Affichage du temps (appelé via la fonction fin_script)
fin_script 0
