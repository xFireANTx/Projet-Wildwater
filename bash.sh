#!/bin/bash

volume_max(){
	return 0
}

volume_source(){
	return 0
}

volume_reel(){
	return 0
}

nombre_arguments=$# 
echo "nb_argument : $nombre_arguments"
Nom_fichier="histo_${2}.dat"
Nom_colonne=""

touch $Nom_fichier
echo -n "identifier;" > $Nom_fichier

if [ $nombre_arguments -ne 2 ]
then
	echo "Pas le bon nombre d'argument, 2 arguments sont attendues (1: 'histo', 2: 'max' ou 'src' ou 'real')."
else
	nom_argument1=$1
	nom_argument2=$2
	if [ "$nom_argument1" != "histo" ]
	then
		echo "Commande $nom_argument1 incorrecte, 'histo' est attendue en 1ère argument."
	else
		case $nom_argument2 in 
		'max')  Nom_colonne="volume max(M.m3/an)" ;;
		'src')  Nom_colonne="volume source(M.m3/an)" ;;
		'real') Nom_colonne="volume reel(M.m3/an)" ;;
		*) echo "Commande $nom_argument2 incorrecte, 'max' ou 'src' ou 'real' sont attendue en 2ème argument." ;;
		esac
		echo "$Nom_colonne" >> $Nom_fichier
	fi
fi		
echo "Nom du fichier créer: $Nom_fichier"
cat $Nom_fichier
#TODO écrire les 3 fonctions des usines selon le max volume,source volume,reel volume (pas forcément)



