adresse		code		mnémonique		commentaire
00			49			IN 20			Entrez l’adresse de chargement du programme
01			20			
02			49			IN 22			Entrez la taille du programme
03			22			
04			40			LOAD 20			A ←adresse du programme
05			20			
06			48			STORE 21		Adresse courante ←A
07			21			
08			C9			IN * 21			Entrez une instruction ou une valeur
09			21			
0A			40			LOAD 22			A ←Nb. d’octets restant à charger
0B			22			
0C			21			SUB #1			Décrémenter le Nb. d’octets
0D			01			
0E			12			BRZ 1F			Exécuter le programme si tout est chargé
0F			1F			
10			48			STORE 22		Ranger le nouveau Nb. d’octets
11			22			
12			40			LOAD 21			Reprendre l’adresse courante dans A
13			21			
14			20			ADD #1			l’incrémenter
15			01			
16			10			JUMP 06			Continuer à charger
17			06			
18			00
19          00
1A          00
1B          00
1C          00
1D          00
1E          00
1F			10			JUMP ??			Brancher sur ...
20			??							l’adresse de début
21			??							Adresse courante
22			??							Taille (ou Nb. d’octets)

Les adresses 00 à 1F sont censées être en mémoire morte et la mémoire
vive commence à 20.