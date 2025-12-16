#!/bin/bash

##On élimine les lignes dont la colonnes 3 n'est pas vide ('-')(pas une usine), on récupère les colonnes 2 et 4 car elles contiennent les usines avec vol_trait_max, on les trie
##TODO supprimer les lignes qui ne sont pas des usines
volume_max(){
	local source="$1"
	awk -F';' '$3 == "-"' "$source" |cut -d';' -f2,4 | sort -t';' -k1,1 -d -r
}

volume_source(){
	local source="$1"
	awk -F';' '$1 == "-"' "$source" | awk -F';' '$3 != "-"' | awk -F';' '$4 !="-"' | cut -d';' -f3,4 |sort -t';' -k1,1 -d -r > temp
}

volume_reel(){
	return 0
}

##Vérification du bon nombre d'argument et du bon format de ceux-ci
if [ $# -ne 3 ] 
then
	echo "Pas le bon nombre d'arguments, 3 arguments sont attendues (1: 'nom_fichier, 2: 'histo', 3: 'max' ou 'src' ou 'real')."
	exit 1
fi


fichier_source=$1
histo=$2
mode=$3

if [ "$histo" != "histo" ] 
then
	echo "Commande $nom_argument2 incorrecte, 'histo' est attendue en 2ème argument."
	exit 1
fi

Nom_fichier="histo_${mode}.dat"

##On récupère dans données les informations du fichier trié dans le bon ordre selon le 3ème argument entré par l'utilisateur
case $mode in 
'max')  Nom_colonne="volume max(M.m3/an)" 
	donnee=$(volume_max "$fichier_source")
	;;
'src')  Nom_colonne="volume source(M.m3/an)" 
	donnee=$(volume_source "$fichier_source")
	;;
'real') Nom_colonne="volume reel(M.m3/an)" 
	donnee=$(volume_reel "$fichier_source")
	;;
*) echo "Commande $mode incorrecte, 'max' ou 'src' ou 'real'sont attendue en 3ème argument." 
	exit 1
	;;
esac

##On met dans notre fichier contenu sur le DD toutes les données obtenues plus tôt
echo -n "identifier;" > "$Nom_fichier" 
echo "$Nom_colonne" >> "$Nom_fichier"
echo "$donnee" >> "$Nom_fichier"

echo "Nom du fichier créé: $Nom_fichier"
cat $Nom_fichier
#TODO écrire les 3 fonctions des usines selon le max volume,source volume,reel volume (pas forcément)
