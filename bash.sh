#!/bin/bash

# 1. Vérification des arguments [cite: 2025-12-19]
if [ "$#" -lt 2 ] || [ "$#" -gt 3 ]; then
    echo "Usage : $0 <fichier_donnees> <commande> [parametre]"
    echo "Commandes :"
    echo "  histo max|src|real : Génère les deux histogrammes correspondants"
    echo "  leaks <identifiant>: Analyse les fuites (en cours)"
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

start_time=$(date +%s%N)

# --- TRAITEMENT COMMANDE HISTO ---
if [ "$COMMAND" = "histo" ]; then
    if [ -z "$PARAM" ]; then
        echo "Erreur : Paramètre manquant pour histo (max, src, ou real)."
        exit 1
    fi

    OUTPUT_FILE="vol_${PARAM}.dat"
    IMAGE_MAX="vol_${PARAM}_top10.png"
    IMAGE_MIN="vol_${PARAM}_bottom50.png"

    # Exécution du programme C [cite: 2025-12-19]
    ./c-wildwater "$DATA_FILE" "histo" "$PARAM" "$OUTPUT_FILE"
    
    if [ $? -ne 0 ]; then
        echo "Erreur lors de l'exécution du programme C."
        exit 1
    fi

    # 4. Préparation des données filtrées (On enlève les valeurs à 0) [cite: 2025-12-19]
    # Cela permet d'avoir un vrai Bottom 50 avec des valeurs visibles [cite: 2025-12-19]
    awk -F';' '$2 > 0 {print $0}' "$OUTPUT_FILE" > data_filtered.tmp

    # Top 10 (Tri décroissant) [cite: 2025-12-19]
    sort -t';' -k2,2nr data_filtered.tmp | head -n 10 > max_10.tmp
    # Bottom 50 (Tri croissant) [cite: 2025-12-19]
    sort -t';' -k2,2n data_filtered.tmp | head -n 50 > min_50.tmp

    # 5. Génération Image 1 : TOP 10 [cite: 2025-12-19]
    gnuplot << EOF
    set terminal png size 1000,600
    set output '${IMAGE_MAX}'
    set datafile separator ";"
    set title "Top 10 : Plus grandes stations (Mode ${PARAM})"
    set style data histograms
    set style fill solid 0.5 border -1
    set boxwidth 0.7
    set xtics rotate by -45 font ",10"
    set ylabel "Volume (M.m3)"
    set grid y
    plot 'max_10.tmp' using 2:xtic(1) notitle linecolor rgb "forest-green"
EOF

    # 6. Génération Image 2 : BOTTOM 50 [cite: 2025-12-19]
    gnuplot << EOF
    set terminal png size 1400,900
    set output '${IMAGE_MIN}'
    set datafile separator ";"
    set title "50 plus petites stations (Mode ${PARAM})"
    set style data histograms
    set style fill solid 0.5 border -1
    set boxwidth 0.8
    set xtics rotate by -90 font ",7"
    set ylabel "Volume (M.m3)"
    set grid y
    plot 'min_50.tmp' using 2:xtic(1) notitle linecolor rgb "red"
EOF

    # Nettoyage
    rm -f max_10.tmp min_50.tmp data_filtered.tmp
    echo "Traitement terminé. Images générées : $IMAGE_MAX et $IMAGE_MIN"

elif [ "$COMMAND" = "leaks" ]; then
    echo "Commande leaks en cours de développement..." [cite: 2025-12-19]
else
    echo "Commande inconnue : $COMMAND"
    exit 1
fi

end_time=$(date +%s%N)
duration=$(( (end_time - start_time) / 1000000 ))
echo "Durée totale : $duration ms" [cite: 2025-12-19]