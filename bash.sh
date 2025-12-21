#!/bin/bash

debut_chrono=$(date +%s%3N)
#les 4 fonctions dépendent du dernière argument, on effectue un filtrage différents selon les arguments pour envoyer un fichier filtré avec les données qui nous intéressent
#au programme C qui va se charger de les trier et d'appliquer les opérations nécessaire pour avoir le bon fihcier en sortie.
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

volume_fuite(){
	local source="$1"
	local id="$2"
	awk -F';' '$1 == "-" && $3 != "-" && $4 != "-"' "$source" | cut -d';' -f3-5 > temp_fuite
	./exe "$1" temp_fuite out_fuite leaks "$id"
	cat out_fuite
	rm out_fuite temp_fuite
}

# Vérification arguments
if [ $# -ne 3 ]; then
    echo "Attendu: $0 fichier histo|leaks max|src|reel"
    exit 1
fi

fichier_source="$1"
histo_leaks="$2"
mode="$3"

case "$histo_leaks" in
histo)
	Nom_fichier="${histo_leaks}_${mode}.dat"
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
	echo "identifier;$Nom_colonne" > "$Nom_fichier"
	echo "$donnee" >> "$Nom_fichier"
	echo "Nom du fichier créé: $Nom_fichier"
	

#On trie le fichier de tel sorte à avoir les usines triées de la plus grande valeur à la plus petite
#On utilise head et tail pour garder les 50 plus petites et les 10 plus grandes dans 2 fichiers distincts
#On passe les deux fichiers dans deux script bash qui vont générés les histogrammes.png
#On supprime les fichiers temporaires utilisés pour ne garder que les png
	{
		head -n 1 "$Nom_fichier"
		tail -n +2 "$Nom_fichier" | sort -t';' -k2,2 -n -r
	} > temp_val

	head -n 11 temp_val| tail -n 10 > temp_"$mode"10.dat
	tail -n 50 temp_val > temp_"$mode"50.dat
	#Permet au programme d exécuter le script'
	chmod a+x affichage_max.sh
	chmod a+x affichage_min.sh
	./affichage_max.sh temp_"$mode"10.dat
	./affichage_min.sh temp_"$mode"50.dat
	rm temp_val temp_"$mode"10.dat temp_"$mode"50.dat
	
    ;;
leaks)
	donnee=$(volume_fuite "$fichier_source" "$mode")
	if [ -f "fuites.dat" ]; then
		echo "$donnee" >> fuites.dat
		echo "Données ajoutées dans fuites.dat"
	else
		Nom_fichier="fuites.dat"
    		Nom_colonne="leaks_volume(M.m3/an)"
    		echo "identifier;$Nom_colonne" > "$Nom_fichier"
    		echo "$donnee" >> "$Nom_fichier"
    		echo "Nom du fichier créé: $Nom_fichier"
    fi
    ;;
*)
    echo "Type invalide"
    exit 1
    ;;
esac

fin_chrono=$(date +%s%3N)
temps_execution=$((fin_chrono - debut_chrono))

echo "Temps d'execution: $temps_execution ms"
