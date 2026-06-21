#!/usr/bin/env python3

import os
import sys
import subprocess
from concurrent.futures import ThreadPoolExecutor

PROG = """\

; PROG

50 3D

;Début:
	49 66 ;  50 51      IN α      a
	49 67 ;  52 53      IN α      b
	00 00 ;  54 55      LOAD #    0
	48 68 ;  56 57      STORE α   Q
	40 67 ;  58 59      LOAD α    b
	12 8A ;  5A 5B      BRZ α     aff
	40 66 ;  5C 5D      LOAD α    a


	48 69 ;  5E 5F      STORE α   R

;t1:
	40 69 ;  60 61      LOAD α    R
	11 6A ;  62 63      BRN α	  t2
	10 70 ;  64 65      JUMP α    t3

	00    ;  66           a
	00    ;  67           b
	00    ;  68           Q
	00    ;  69           R

;t2:
	40 67 ;  6A 6B      LOAD α    B
	11 74 ;  6C 6D      BRN α     t4
	10 7C ;  6E 6F      JUMP α    tok;



;t3:
	40 67 ;  70 71      LOAD α    B
	11 8A ;  72 73      BRN α     aff

;t4:
	40 69 ;  74 75      LOAD α    R
	61 67 ;  76 77      SUB α     B
	11 8A ;  78 79      BRN α     aff
	10 7C ;  7A 7B      JUMP α    tok


;tok:
	40 69 ;  7C 7D      LOAD α    R
	61 67 ;  7E 7F      SUB α     B
	48 69 ;  80 81      STORE α   R
	40 68 ;  82 83      LOAD α    Q
	20 01 ;  84 85      ADD #     01
	48 68 ;  86 87      STORE α   Q
	10 60 ;  88 89      JUMP α    t1

;aff:
	41 68 ;  8A 8B      OUT α     Q
	FF    ;  8C         STOP
	
	
	
"""

PICO = "./bin/pico"
# Threads 
NPROC = os.cpu_count() or 4
# Nb marques par ligne
LARGEUR = 128

def div(i, j):
    try:
        p = subprocess.run(
            [PICO],
            # Ajouter i et j au stdin en hexa
            input=f"{PROG}\n{i:02X} {j:02X}\n",
            capture_output=True, text=True, timeout=5,
        )
        resultat = p.stdout.strip()
        div = str(i // j)
        return "." if resultat == div else "x"
    except subprocess.TimeoutExpired:
        return "t"

def main():
    with ThreadPoolExecutor(max_workers=NPROC) as ex:
        for i in range(0, 256):
            marques = "-" + "".join(ex.map(lambda j: div(i, j), range(1, 256)))
            entete = f" {i:3d} : "
            indent = " " * len(entete)
            # Afficher largeur par largeur
            for k in range(0, len(marques), LARGEUR):
                tranche = marques[k:k + LARGEUR]
                print((entete if k == 0 else indent) + tranche)


if __name__ == "__main__":
    main()
