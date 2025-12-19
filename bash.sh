#!/bin/bash

# 1. Vérification des arguments [cite: 2025-12-19]
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

# 2. Vérification du fichier d'entrée [cite: 2025-12-19]
if [ ! -f "$DATA_FILE" ]; then
    echo "Erreur : Le fichier $DATA_FILE n'existe pas."
    exit 1
fi

# 3. Compilation via Makefile [cite: 2025-12-19]
make > /dev/null
if [ $? -ne 0 ]; then
    echo "Erreur de compilation."
    exit 1
fi

# Chronomètre de performance [cite: 2025-12-19]
start_time=$(date +%s%N)

# --- TRAITEMENT COMMANDE HISTO ---
if [ "$COMMAND" = "histo" ]; then
    if [ -z "$PARAM" ]; then
        echo "Erreur : Paramètre manquant pour histo (max, src, ou real)."
        exit 1
    fi

    OUTPUT_FILE="vol_${PARAM}.dat"
    IMAGE_FILE="vol_${PARAM}.png"

    # Exécution du programme C
    ./c-wildwater "$DATA_FILE" "histo" "$PARAM" "$OUTPUT_FILE"
    
    if [ $? -ne 0 ]; then
        echo "Erreur lors de l'exécution du programme C."
        exit 1
    fi

    # 4. Préparation des données pour Gnuplot [cite: 2025-12-19]
    # On récupère les 10 plus GRANDS (Tri numérique décroissant sur col 2) [cite: 2025-12-19]
    tail -n +2 "$OUTPUT_FILE" | sort -t';' -k2,2nr | head -n 10 > max_10.tmp

    # On récupère les 50 plus PETITS (Tri numérique croissant sur col 2) [cite: 2025-12-19]
    tail -n +2 "$OUTPUT_FILE" | sort -t';' -k2,2n | head -n 50 > min_50.tmp

    # 5. Génération du graphique avec Gnuplot
    gnuplot << EOF
    set terminal png size 1200,1000
    set output '${IMAGE_FILE}'
    set datafile separator ";"
    
    set multiplot layout 2,1 title "Statistiques Station: ${PARAM}"

    # Top 10 [cite: 2025-12-19]
    set title "Top 10 : Plus grandes valeurs"
    set style data histograms
    set style fill solid 0.5 border -1
    set boxwidth 0.8
    set xtics rotate by -45 font ",9"
    set ylabel "Volume"
    set grid y
    plot 'max_10.tmp' using 2:xtic(1) notitle linecolor rgb "forest-green"

    # Bottom 50 [cite: 2025-12-19]
    set title "Top 50 : Plus petites valeurs"
    set style data histograms
    set style fill solid 0.5 border -1
    set boxwidth 0.8
    set xtics rotate by -90 font ",7"
    set ylabel "Volume"
    set grid y
    plot 'min_50.tmp' using 2:xtic(1) notitle linecolor rgb "red"

    unset multiplot
EOF

    # Nettoyage
    rm -f max_10.tmp min_50.tmp
    echo "Traitement terminé. Image générée : $IMAGE_FILE" [cite: 2025-12-19]

# --- TRAITEMENT COMMANDE LEAKS ---
elif [ "$COMMAND" = "leaks" ]; then
    echo "Commande leaks en cours de développement..." [cite: 2025-12-19]
else
    echo "Commande inconnue : $COMMAND"
    exit 1
fi

# Fin du chrono [cite: 2025-12-19]
end_time=$(date +%s%N)
duration=$(( (end_time - start_time) / 1000000 ))
echo "Durée totale : $duration ms" 