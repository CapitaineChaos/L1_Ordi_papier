

Rappel :

```text
#  est un adressage immédiat
α  est un adressage absolu
*α est un adressage indirect
```

```text
00       LOAD  #          1, 13, 3
40       LOAD  α          1, 13, 6, 7, 13, 3
C0       LOAD *α          1, 13, 6, 7, 13, 6, 7, 13, 3

48       STORE  α         1, 13, 6, 7, 4, 14 
C8       STORE *α         1, 13, 6, 7, 13, 6, 7, 4, 14

49       IN  α            1, 13, 6, 7, 16, 8, 14
C9       IN *α            1, 13, 6, 7, 13, 6, 7, 16, 8, 14

41       OUT  α           1, 13, 6, 7, 13, 9
C1       OUT *α           1, 13, 6, 7, 13, 6, 7, 13, 9

20       ADD  #           10, 1, 13, 12
60       ADD  α           10, 1, 13, 6, 7, 13, 12
E0       ADD *α           10, 1, 13, 6, 7, 13, 6, 7, 13, 12

21       SUB  #           11, 1, 13, 12
61       SUB  α           11, 1, 13, 6, 7, 13, 12
E1       SUB *α           11, 1, 13, 6, 7, 13, 6, 7, 13, 12

22       NAND  #          17, 1, 13, 12
62       NAND  α          17, 1, 13, 6, 7, 13, 12
E2       NAND *α          17, 1, 13, 6, 7, 13, 6, 7, 13, 12

10       JUMP α           1, 13, 2
11       BRN α            Si A < 0  : 1, 13, 2
12       BRZ α            Si A == 0 : 1, 13, 2
```

Microcodes :

```
RS : Registre de Sélection ; adresse d'une case mémoire
RM : Registre Mot ; échange valeurs entre mémoire et UC
PC : Program Counter ; adresse mémoire de la prochaine instruction
OP : code OPératoire de l'instruction en cours
AD : Adresse Donnée à traiter
A  : Registre A

La flèche ← représente l’affectation.
() représente le contenu de...
[] représente le contenu d’une cellule d’un tableau
```


```text
1  : (RS) ← (PC)
2  : (PC) ← (RM) ; Ne pas faire la phase III.
3  : (A)  ← (RM)
4  : (RM) ← (A)
5  : (OP) ← (RM)
6  : (AD) ← (RM)
7  : (RS) ← (AD)
8  : (RM) ← (Entrée)
9  : (Sortie) ← (RM)
10 : Mettre UAL en mode ADD
11 : Mettre UAL en mode SUB
17 : Mettre UAL en mode NAND
12 : (A) ← (A) ual_mode (RM)
13 : (RM) ← mem[(RS)]
14 : mem[(RS)] ← (RM)
15 : (PC) ← (PC) + 1
16 : Attendre qu’une valeur soit écrite dans l’Entrée.
```


Phases :

```text
Phase I   : 1, 13, 5, 15
Phase II  : Microcodes[Instruction]
Phase III : 15
```

```texte
Décodez l’adresse contenue dans le registre de sélection (RS) en prenant le chiffre
gauche (ou Haut) pour numéro de colonne et le droit (ou Bas) pour numéro de
ligne. Reportez le contenu de la case mémoire ainsi désignée dans le registre mot
(RM).