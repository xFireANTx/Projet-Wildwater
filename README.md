# WildWater MEF2-GROUPE E

## Présentation
WildWater est un projet visant à analyser un réseau de distribution d’eau à partir d’un fichier de données.  
Le programme permet de calculer différents volumes et d’analyser les pertes liées aux fuites.  

_________________________________________________________________________________________________________________

## Fonctionnalités
- Calcul du volume maximal par usine  
- Calcul du volume capté à la source  
- Calcul du volume réel en tenant compte des fuites  
- Analyse des fuites pour une usine donnée  
- Génération de fichiers pour histogrammes  
__________________________________________________________________________________________________________________

## Fichiers importants
- bash.sh : point d’entrée du programme. Script permettant de lancer les différents modes histo et leaks.
- main.c : programme principal appelé par le script.
- avl.c / avl.h : implémentation des arbres AVL utilisés pour le tri et la recherche  
- basic_functions.c / basic_functions.h` : gestion des infrastructures et calcul des flux / fuites  
- avl_histo_traitement.c / avl_histo_traitement.j : calcul des volumes maximaux et sources  
- avl_histo_reel.c : calcul des volumes réels 
- Makefile : compilation du projet  
___________________________________________________________________________________________________________________

## Compilation et exécution LINUX

make / make clean

- ./bash.sh "Nom_fichier.dat" histo max
- ./bash.sh "Nom_fichier.dat" histo src
- ./bash.sh "Nom_fichier.dat" histo reel
- ./bash.sh "Nom_fichier.dat" leaks "usine#identifiant"

PS: si le script n'est pas exécutable --> chmod a+x bash.sh
___________________________________________________________________________________________________________________

## Limite de notre programme

Le mode `leaks` peut avoir un temps d’exécution élevé et le traitement complet peut prendre jusqu’à environ 4 minutes.
____________________________________________________________________________________________________________________

## Conclusion

Ce projet nous a permis de travailler sur des structures de données avancées en C et d’optimiser un programme 
pour le traitement de grands volumes de data à l'aide d'un script Shell et d'un programme C utilisant des AVL.


