#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage : $0 histo_xxx.dat"
    exit 1
fi

datafile=$1

if [ ! -f "$datafile" ]; then
    echo "Fichier $datafile introuvable"
    exit 1
fi
#on récupère  la partie temp_max10 dans base
base=$(basename "$datafile" .dat)

mode=${base#temp_}
mode=${mode%10}

output="histo_${mode}10.png"

case $base in
    temp_max10)
        title="Volume maximal de traitement des usines "
        ;;
    temp_src10)
        title="Volume total capté par les usines (sans les pertes)"
        ;;
    temp_reel10)
        title="Volume réel traité par les usines (capté avec les pertes)"
        ;;
    *)
        echo "Nom de fichier non reconnu"
        exit 1
        ;;
esac

gnuplot << EOF
set terminal png size 1800,900
set output "$output"
set datafile separator ";"

set style data histograms
set style fill solid 0.8 border -1
set boxwidth 0.9

set xtics rotate by -45
set grid ytics

set title "$title"
set ylabel "Volume (M.m3/an)"
set xlabel "Usines"

plot "$datafile" using 2:xtic(1) notitle
EOF

echo "Histogramme généré : $output"
