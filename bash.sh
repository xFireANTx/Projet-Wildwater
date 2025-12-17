#!/bin/bash

##Le processus des 3 fonctions servant à générer les fichiers histo.dat sont similaire. On remarque que chaque histo demandé représente une étape du circuit et que selon 
##l'étape, on a un motif différent de '-';txt;'-'etc. Ainsi, on effectue un filtrage en colonne pour récupérer les informations dont on a besoin et on trie le tout
##Finalement on effectue les opérations nécessaire en c sur le fichier triés et filtrés

##On élimine les lignes dont la colonnes 3 n'est pas vide ('-')(pas une usine), on récupère les colonnes 2 et 4 car elles contiennent les usines avec vol_trait_max, on les trie
volume_max(){
#TODO convertir en million de m3/an (en c ou en shell si possible) (optionnelle créer un unique fichier.c qui regroupe les 2 somme car très similaire)
	local source="$1"
	awk -F';' '$3 == "-"' "$source" |cut -d';' -f2,4 | sort -t';' -k1,1 -d -r > temp_volume
	
	gcc -o exe volume.c
	./exe temp_volume out_volume
	
	cat out_volume
	rm temp_volume out_volume

	
}

volume_source(){
	local source="$1"
	awk -F';' '$1 == "-"' "$source" | awk -F';' '$3 != "-"' | awk -F';' '$4 !="-"' | cut -d';' -f3,4 |sort -t';' -k1,1 -d -r > temp_source
	
	gcc -o exe somme_source.c
	./exe temp_source out_source
	
	cat out_source
	rm temp_source
	rm out_source
}

volume_reel(){
	local source="$1"
	awk -F';' '$1 == "-"' "$source" | awk -F';' '$3 != "-"' | awk -F';' '$4 !="-"' | cut -d';' -f3-5 |sort -t';' -k1,1 -d -r > temp_reel
	
	gcc -o exe somme_reel.c
	./exe temp_reel out_reel
	
	cat out_reel
	rm temp_reel
	rm out_reel
}

##Vérification du bon nombre d'argument et du bon format de ceux-ci
if [ $# -ne 3 ] 
then
	echo "Pas le bon nombre d'arguments, 3 arguments sont attendues (1: 'nom_fichier, 2: 'histo', 3: 'max' ou 'src' ou 'real')."
	exit 1
fi

#TODO ajoutez le choix leaks pour le deuxième argument
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
