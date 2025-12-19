#!/bin/bash

##Le processus des 3 fonctions servant à générer les fichiers histo.dat sont similaire. On remarque que chaque histo demandé représente une étape du circuit et que selon 
##l'étape, on a un motif différent de '-';txt;'-'etc. Ainsi, on effectue un filtrage en colonne pour récupérer les informations dont on a besoin
##Finalement on effectue les opérations nécessaire en c sur le fichier triés et filtrés

volume_max(){
	local source="$1"
	awk -F';' '$3 == "-"' "$source" |cut -d';' -f2,4  > temp_volume
	
	./exe temp_volume out_volume histo max
	
	cat out_volume
	rm temp_volume out_volume
}

volume_source(){
	local source="$1"
	awk -F';' '$1 == "-"' "$source" | awk -F';' '$3 != "-"' | awk -F';' '$4 !="-"' | cut -d';' -f3,4  > temp_source
	
	./exe temp_source out_source histo src
	
	cat out_source
	rm temp_source  out_source
}

volume_reel(){
	local source="$1"
	awk -F';' '$1 == "-"' "$source" | awk -F';' '$3 != "-"' | awk -F';' '$4 !="-"' | cut -d';' -f3-5  > temp_reel
	
	./exe temp_reel out_reel histo reel
	
	cat out_reel
	rm temp_reel out_reel
}

##Vérification du bon nombre d'argument et du bon format de ceux-ci
if [ $# -ne 3 ] 
then
	echo "Pas le bon nombre d'arguments, 3 arguments sont attendues (1: 'nom_fichier, 2: 'histo'/'leaks', 3: 'max' ou 'src' ou 'reel'/'Identifiant_usine')."
	exit 1
fi

fichier_source=$1
histo_leaks=$2
mode=$3


Nom_fichier="${histo_leaks}_${mode}.dat"
case $histo_leaks in
'histo')
	##On récupère dans données les informations du fichier trié dans le bon ordre selon le 3ème argument entré par l'utilisateur
	case $mode in 
	'max')  Nom_colonne="volume max(M.m3/an)" 
		donnee=$(volume_max "$fichier_source")
		;;
	'src')  Nom_colonne="volume source(M.m3/an)" 
		donnee=$(volume_source "$fichier_source")
		;;
	'reel') Nom_colonne="volume reel(M.m3/an)" 
		donnee=$(volume_reel "$fichier_source")
		;;
	*) 	echo "Commande $mode incorrecte, 'max' ou 'src' ou 'reel'sont attendue en 3ème argument." 
		exit 1
		;;
	esac
	echo -n "identifier;" > "$Nom_fichier" 
	echo "$Nom_colonne" >> "$Nom_fichier"
;;

'leaks')
	donnee = "empty"
	;;
*) 	echo "Commande $histo_leaks incorrecte, 'histo' ou 'leaks sont attendus en 2ème argument."
	exit 1
	;;
esac

##On met dans notre fichier contenu sur le DD toutes les données obtenues plus tôt

echo "$donnee" >> "$Nom_fichier"
echo "Nom du fichier créé: $Nom_fichier"
