#!/bin/bash

# Vérification des arguments
if [ "$#" -lt 2 ] || [ "$#" -gt 3 ]; then
    echo "Usage : $0 <fichier_donnees> <commande> [parametre]"
    echo "Commandes :"
    echo "  histo max|src|real : Génère l'histogramme correspondant"
    echo "  leaks <identifiant>: Analyse les fuites pour une usine donnée"
    exit 1
fi

DATA_FILE="$1"
COMMAND="$2"
PARAM="$3"

# Vérification fichier données
if [ ! -f "$DATA_FILE" ]; then
    echo "Erreur : Le fichier $DATA_FILE n'existe pas."
    exit 1
fi

# Compilation (via Makefile)
# On redirige la sortie de make vers /dev/null pour ne pas polluer l'affichage, 
# sauf en cas d'erreur
make > /dev/null
if [ $? -ne 0 ]; then
    echo "Erreur de compilation."
    exit 1
fi

# Début du chrono
start_time=$(date +%s%N)

# --- TRAITEMENT COMMANDE HISTO ---
if [ "$COMMAND" = "histo" ]; then
    if [ -z "$PARAM" ]; then
        echo "Erreur : Paramètre manquant pour histo (max, src, ou real)."
        exit 1
    fi

    # 1. Définition du nom de fichier
    OUTPUT_FILE="vol_${PARAM}.dat"
    IMAGE_FILE="vol_${PARAM}.png"

    # 2. Exécution du programme C
    # Le C va créer le fichier .dat trié par IDENTIFIANT (Z->A)
    ./c-wildwater "$DATA_FILE" "histo" "$PARAM" "$OUTPUT_FILE"
    
    if [ $? -ne 0 ]; then
        echo "Erreur lors de l'exécution du programme C."
        exit 1
    fi

    # 3. Traitement des données pour Gnuplot
    # Le fichier est trié par ID, mais Gnuplot veut les min/max VALEURS.
    # On utilise sort en ligne de commande pour préparer deux fichiers temporaires.
    
    # Récupérer les 10 plus GRANDS (Tri numérique décroissant sur col 2)
    tail -n +2 "$OUTPUT_FILE" | sort -t';' -k2,2nr | head -n 10 > max_10.tmp

    # Récupérer les 50 plus PETITS (Tri numérique croissant sur col 2)
    # Attention : on exclut les valeurs nulles ou négatives si besoin, 
    # mais ici on prend tout ce qui est positif.
    tail -n +2 "$OUTPUT_FILE" | sort -t';' -k2,2n | head -n 50 > min_50.tmp

    # 4. Génération du graphique avec Gnuplot
    gnuplot << EOF
    set terminal png size 1000,1200
    set output '${IMAGE_FILE}'
    set datafile separator ";"
    
    set multiplot layout 2,1 title "Statistiques Station: ${PARAM}"

    # Graphique 1 : Top 10 (Barres Vertes)
    set title "Top 10 : Plus grandes valeurs"
    set style data histograms
    set style fill solid 1.0 border -1
    set boxwidth 0.7
    set xtics rotate by -45
    set ylabel "Volume (M.m3)"
    set grid y
    plot 'max_10.tmp' using 2:xtic(1) notitle linecolor rgb "forest-green"

    # Graphique 2 : Bottom 50 (Barres Rouges)
    set title "Top 50 : Plus petites valeurs"
    set style data histograms
    set style fill solid 1.0 border -1
    set boxwidth 0.7
    set xtics rotate by -90 font ",8"
    set ylabel "Volume (M.m3)"
    set grid y
    plot 'min_50.tmp' using 2:xtic(1) notitle linecolor rgb "red"

    unset multiplot
EOF

    # Nettoyage des fichiers temporaires
    rm -f max_10.tmp min_50.tmp
    
    echo "Fichiers générés : $OUTPUT_FILE et $IMAGE_FILE"

# --- TRAITEMENT COMMANDE LEAKS (A venir) ---
elif [ "$COMMAND" = "leaks" ]; then
    echo "Commande leaks en construction..."
    # ./c-wildwater "$DATA_FILE" "leaks" "$PARAM" "leaks.log"
    # ... traitement gnuplot ...
else
    echo "Commande inconnue : $COMMAND"
    exit 1
fi

# Fin du chrono
end_time=$(date +%s%N)
duration=$(( (end_time - start_time) / 1000000 ))

echo "Durée totale : $duration ms"