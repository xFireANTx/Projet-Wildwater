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

base=$(basename "$datafile" .dat)
output="${base}.png"

case $base in
    histo_max)
        title="Volume maximal des usines"
        ;;
    histo_src)
        title="Volume total capté par les usines"
        ;;
    histo_real)
        title="Volume réel traité par les usines"
        ;;
    *)
        echo "Nom de fichier non reconnu"
        exit 1
        ;;
esac

gnuplot -e "input='$datafile';output='$output';title='$title'" scripts/histo.gnuplot

echo "Histogramme généré : $output"
