#!/bin/bash

debut_chrono = $(date +%s%3N)

volume_max(){
    local source="$1"
    awk -F';' '$3 == "-"' "$source" | cut -d';' -f2,4 > temp_volume
    ./exe "$1" temp_volume out_volume histo max
    cat out_volume
    rm temp_volume out_volume
}

volume_source(){
    local source="$1"
    awk -F';' '$1 == "-" && $3 != "-" && $4 != "-"' "$source" | cut -d';' -f3,4 > temp_source
    ./exe "$1" temp_source out_source histo src
    cat out_source
    rm temp_source out_source
}

volume_reel(){
    local source="$1"
    awk -F';' '$1 == "-" && $3 != "-" && $4 != "-"' "$source" | cut -d';' -f3-5 > temp_reel
    ./exe "$1" temp_reel out_reel histo reel
    cat out_reel
    rm temp_reel out_reel
}

# Vérification arguments
if [ $# -ne 3 ]; then
    echo "Usage: $0 fichier histo|leaks max|src|reel"
    exit 1
fi

fichier_source="$1"
histo_leaks="$2"
mode="$3"

Nom_fichier="${histo_leaks}_${mode}.dat"

case "$histo_leaks" in
histo)
    case "$mode" in
    max)
        Nom_colonne="volume max(M.m3/an)"
        donnee=$(volume_max "$fichier_source")
        ;;
    src)
        Nom_colonne="volume source(M.m3/an)"
        donnee=$(volume_source "$fichier_source")
        ;;
    reel)
        Nom_colonne="volume reel(M.m3/an)"
        donnee=$(volume_reel "$fichier_source")
        ;;
    *)
        echo "Mode invalide"
        exit 1
        ;;
    esac
    ;;
leaks)
    Nom_colonne="leaks_volume(M.m3/an)"
    donnee="empty"
    ;;
*)
    echo "Type invalide"
    exit 1
    ;;
esac

echo "identifier;$Nom_colonne" > "$Nom_fichier"
echo "$donnee" >> "$Nom_fichier"

{
    head -n 1 "$Nom_fichier"
    tail -n +2 "$Nom_fichier" | sort -t';' -k2,2 -n -r
} > temp_val

head -n 11 temp_val| tail -n 10 > temp_max
tail -n 50 temp_val > temp_min
rm temp_val

fin_chrono =$(date+%s%3N)
temps_execution = $((fin_chrono - debut_chrono))
echo "Nom du fichier créé: $Nom_fichier"
echo "Temps d'execution: $temps_execution"
