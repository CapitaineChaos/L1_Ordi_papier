#!/usr/bin/env python3

#===============/                    ex10.6                    \================
#=~~~~~~~~~~~~=|               Ordinateur en papier             |=~~~~~~~~~~~~~=
#===============\          Wrapper de test multithread         /================
#
# Auteur : Sylvain Maitre     24002886
# Modifié par : Wewe MAITRE   24001170
#
# Date de création :              22/06/2026
# Date de dernière modification : 22/06/2026
#
# Description : Tester en parallèle le programme de division sur tous les 
#               couples (i,j) avec 0 <= i < 256 et 1 <= j < 256
# Utilisation : ./testdiv.py
# Attention   : Il faut renseigner le chemin vers l'exécutable pico
#
# Remarque : La bonne version du programme de division
#
#===============================================================================

import os
import sys
import subprocess
from concurrent.futures import ThreadPoolExecutor

PROG = """\
70          ; adresse de début du programme
1F          ; longueur du programme

; Initialisation du quotient a à 0
; adr 70
00 00       ; LOAD 00
; adr 72
48 9C       ; STORE a

; Lecture des deux valeurs
; adr 74
49 9A       ; IN x
; adr 76
49 9B       ; IN y

; Initialisation du reste b avec x
; adr 78
40 9A       ; LOAD x
; adr 7A
48 9D       ; STORE b

; Boucle de division par soustractions successives
; adr 7C
40 9D       ; LOAD b
; adr 7E
61 9B       ; SUB y
; adr 80
11 8C       ; BRN fin

; Si b - y est positif ou nul, on met à jour b
; adr 82
48 9D       ; STORE b

; a = a + 1
; adr 84
40 9C       ; LOAD a
; adr 86
20 01       ; ADD 1
; adr 88
48 9C       ; STORE a

; Retour au début de la boucle
; adr 8A
10 7C       ; JUMP boucle

; Fin : affichage du quotient
; adr 8C
41 9C       ; OUT a
; adr 8E
FF          ; HALT

; Variables
; adr 9A
            ; x : dividende
; adr 9B
            ; y : diviseur
; adr 9C
            ; a : quotient
; adr 9D
            ; b : reste temporaire
"""

PICO = "../bin/pico"
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
