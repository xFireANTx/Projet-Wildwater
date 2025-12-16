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

set terminal png size 1200,600
set output output
set datafile separator ";"
set style data histograms
set style fill solid 0.8 border -1
set boxwidth 0.9
set xtics rotate by -45
set grid ytics
set title title
set ylabel "Volume (M.m3/an)"
set xlabel "Usines"
plot input using 2:xtic(1) notitle
