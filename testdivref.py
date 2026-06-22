#!/usr/bin/env python3

import os
import sys
import subprocess
from concurrent.futures import ThreadPoolExecutor

PROG = """\

;     Informations de chargement (en hexa) :
#############################################
50 ; Adresse de début du programme
2B ; Taille du programme
#############################################

;==|= hexa =|=== adresse ===|===== Mnémonique ======|================== Commentaire ====================|

;Début:
     49 74  ;     50 51             IN α      a         (74) ← IN             Saisie du dividende  (a)
     49 75  ;     52 53             IN α      b         (75) ← IN             Saisie du diviseur   (b)
                                                                                                                 
     00 00  ;     54 55             LOAD #    00        A ← 0                                                     
     48 76  ;     56 57             STORE α   Q         (76) ← A              Q ← 0
                                                                                                                 
     40 75  ;     58 59             LOAD α    b         A ← (75)                                                     
     12 78  ;     5A 5B             BRZ α     aff       Si A=0, PC ← 78   S   i b = 0,     aller à aff
                                                                                                                 
     40 74  ;     5C 5D             LOAD α    a         A ← (74)                                                                    
     48 77  ;     5E 5F             STORE α   R         (77) ← A              R ← a
                                                                                                                                                                                                                        
;t1:                                                                                                            
     40 77  ;     60 61             LOAD α              A ← (77)                                                                    
     61 75  ;     62 63             SUB α               A ← A - (75)                                                            
     11 78  ;     64 65             BRN α               Si A<0, PC ← 78       Si R - B < 0, aller à aff
                                                                                                            
;tok:                                                                                                            
     40 77  ;     66 67             LOAD α              A ← (77)                                                                    
     61 75  ;     68 69             SUB α               A ← A - (75)                                                            
     48 77  ;     6A 6B             STORE α             (77) ← A              R ← R - B
     40 76  ;     6C 6D             LOAD α              A ← (76)                                                                   
     20 01  ;     6E 6F             ADD #               A ← A + 1                                                                   
     48 76  ;     70 71             STORE α             (76) ← A              Q ← Q + 1
     10 60  ;     72 73             JUMP α              PC ← 60               Aller à t1

     00     ;     74                                                          variable a
     00     ;     75                                                          variable b
     00     ;     76                                                          variable Q
     00     ;     77                                                          variable R

;aff:                                                                                                            
     41 76  ;     78 79             OUT α               Sortie ← (76)         Afficher Q
     FF     ;     7A                STOP                                      Arrêt du programme
  
"""

PICO = "./bin/pico"
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
    with ThreadPoolExecutor(max_workers=NPROC) as ex:
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
