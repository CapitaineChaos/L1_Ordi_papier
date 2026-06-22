#!/usr/bin/env python3

#===============/                    ex10.6                    \================
#=~~~~~~~~~~~~=|               Ordinateur en papier             |=~~~~~~~~~~~~~=
#===============\          Wrapper de test multithread         /================
#
# Auteur : Sylvain Maitre     24002886
#
# Date de création :              22/06/2026
# Date de dernière modification : 22/06/2026
#
# Description : Tester en parallèle le programme de division sur tous les 
#               couples (i,j) avec 0 <= i < 256 et 1 <= j < 256
# Utilisation : ./testdiv.py
# Attention   : Il faut renseigner le chemin vers l'exécutable pico
#
#===============================================================================

import os
import sys
import subprocess
from concurrent.futures import ThreadPoolExecutor

PICO = "./bin/pico"


PROG = """\

;     Informations de chargement (en hexa) :
#############################################
50 ; Adresse de début du programme
3B ; Taille du programme
#############################################

;==|= hexa =|=== adresse ===|===== Mnémonique ======|================== Commentaire ====================|

;Début:
     49 66  ;     50 51             IN α      a         (66) ← IN             Saisie du dividende  (a)
     49 67  ;     52 53             IN α      b         (67) ← IN             Saisie du diviseur   (b)
                                                                                                                 
     00 00  ;     54 55             LOAD #    00        A ← 0                                                     
     48 68  ;     56 57             STORE α   Q         (68) ← A              Q ← 0
                                                                                                                 
     40 67  ;     58 59             LOAD α    b         A ← (67)                                                     
     12 88  ;     5A 5B             BRZ α     aff       Si A=0, PC ← 88   S   i b = 0,     aller à aff
                                                                                                                 
     40 66  ;     5C 5D             LOAD α    a         A ← (66)                                                                    
     48 69  ;     5E 5F             STORE α   R         (69) ← A              R ← a
                                                                                                             
;t1:                                                                                                            
     40 69  ;     60 61             LOAD α    R         A ← (69)                                                     
     11 6A  ;     62 63             BRN α               Si A<0, PC ← 6A       Si R < 0,     aller à t2
     10 70  ;     64 65             JUMP α              PC ← 70               Sinon         aller à t3
                                                                                                                 
     00     ;     66                                                          variable a
     00     ;     67                                                          variable b
     00     ;     68                                                          variable Q
     00     ;     69                                                          variable R
                                                                                                            
;t2:                                                                                                            
     40 67  ;     6A 6B             LOAD α              A ← (67)                                                                    
     11 74  ;     6C 6D             BRN α               Si A<0, PC ← 74       Si B < 0,     aller à t4
     10 7A  ;     6E 6F             JUMP α              PC ← 7A               Sinon         aller à tok;
                                                                                                            
;t3:                                                                                                            
     40 67  ;     70 71             LOAD α              A ← (67)                                                                    
     11 88  ;     72 73             BRN α               Si A<0, PC ← 88       Si b < 0,     aller à aff
                                                                                                            
;t4:                                                                                                            
     40 69  ;     74 75             LOAD α              A ← (69)                                                                    
     61 67  ;     76 77             SUB α               A ← A - (67)                                                            
     11 88  ;     78 79             BRN α               Si A<0, PC ← 88       Si R - B < 0, aller à aff
                                                                                                            
;tok:                                                                                                            
     40 69  ;     7A 7B             LOAD α              A ← (69)                                                                    
     61 67  ;     7C 7D             SUB α               A ← A - (67)                                                            
     48 69  ;     7E 7F             STORE α             (69) ← A              R ← R - B
     40 68  ;     80 81             LOAD α              A ← (68)                                                                   
     20 01  ;     82 83             ADD #               A ← A + 1                                                                   
     48 68  ;     84 85             STORE α             (68) ← A              Q ← Q + 1
     10 60  ;     86 87             JUMP α              PC ← 60               Aller à t1
                                                                                                            
;aff:                                                                                                            
     41 68  ;     88 89             OUT α               Sortie ← (68)         Afficher Q
     FF     ;     9A                STOP                                      Arrêt du programme
  
"""


# Threads 
NPROC = os.cpu_count()
# Nb marques par ligne
LARGEUR = 128

def div(i, j):
    try:
        p = subprocess.run(
            [PICO],
            # Ajouter i et j au stdin en hexa
            input=f"{PROG} {i:02X} {j:02X}",
            capture_output=True, text=True, timeout=5,
        )
        resultat = p.stdout.strip()
        div = str(i // j)
        return "." if resultat == div else "x"
    except subprocess.TimeoutExpired:
        return "t"

def main():
    nb_ok = 0
    nb_ko = 0
    with ThreadPoolExecutor(max_workers=NPROC) as executor:
        for i in range(0, 256):
            marques = "-"
            for j in range(1, 256):
                marque = div(i, j)
                if marque == ".":
                    nb_ok += 1
                else:
                    nb_ko += 1
                marques += marque
            entete = f" {i:3d} : "
            indent = " " * len(entete)
            # Afficher largeur par largeur
            for k in range(0, len(marques), LARGEUR):
                tranche = marques[k:k + LARGEUR]
                print((entete if k == 0 else indent) + tranche)
    print(f"Nombre de tests OK : {nb_ok}")
    print(f"Nombre de tests KO : {nb_ko}")

if __name__ == "__main__":
    main()
