Membres du projet :
	Orlina Joly, elle s'est occupée de tout ce qui concerne le parsing du fichier d'entrée de l'implémentation du dictionnaire, et de la création du fichier de sortie
	Paul Kamano, il s'est occupé de tout ce qui concerne le dijsktra et le tas binaire, de l'algorithme de prétraitement et de la libération de mémoir.

Fonctionnalités : 
	Permet de calculer le plus court chemin entre 2 coordonnées géographique sur les Pays de la Loire (plus si affinités). Il ressort un fichier lisible sur umap, et peut aussi crée un fichier pour prétraiter les données 
(Il faut mettre en commentaire dans le main.c #define PRETRAITEMENT)

Temps d'execution + Mémoire consommée
	J'en ai aucune idée

Commandes de compilation 
	gcc -Wall *c -g -lm (en étant dans le bon dossier)
Commandes d'execution sous Valgrind
	valgrind -v --leak-check=full ./a.out
	
