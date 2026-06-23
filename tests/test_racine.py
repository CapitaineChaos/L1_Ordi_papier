#!/usr/bin/env python3

#===============/                    ex10.8                    \================
#=~~~~~~~~~~~~=|               Ordinateur en papier             |=~~~~~~~~~~~~~=
#===============\          Wrapper de test multithread         /================
#
# Auteur : Sylvain Maitre     24002886
#
# Date de création :              22/06/2026
# Date de dernière modification : 23/06/2026
#
# Description : Teste un programme de racine carrée entière (fourni en .hex)
#               pour tout i de 0 à 255, et compare la sortie de pico à
#               floor(sqrt(i)) calculé par Python.
# Utilisation : ./test_racine.py <programme.hex>
#
#===============================================================================

import os
import sys
import math
import subprocess
from concurrent.futures import ThreadPoolExecutor

PICO = "../bin/pico"
# Nombre de threads
NPROC = os.cpu_count()
# nombre de marques par ligne
LARGEUR = 64


def lire_programme():
    if len(sys.argv) < 2:
        print(f"Usage : {sys.argv[0]} <programme.hex>", file=sys.stderr)
        print("Erreur : fichier .hex uniquement", file=sys.stderr)
        sys.exit(1)
    chemin = sys.argv[1]
    try:
        with open(chemin) as f:
            return f.read()
    except OSError as e:
        print(f"Erreur : impossible de lire {e}", file=sys.stderr)
        sys.exit(1)


def rac(prog, i):
    try:
        p = subprocess.run(
            [PICO],
            # Ajouter i au stdin en hexa pour remplacer le clavier
            input=f"{prog} {i:02X}",
            # PAs besoin d'un grand timeout
            capture_output=True, text=True, timeout=1,
        )
        return "." if p.stdout.strip() == str(math.floor(i ** 0.5)) else "x"
    except subprocess.TimeoutExpired:
        return "t"


def main():
    prog = lire_programme()
    with ThreadPoolExecutor(max_workers=NPROC) as executor:
        # i = 0..255 en parallèle, map garde l'ordre
        marques = "".join(executor.map(lambda i: rac(prog, i), range(256)))
    nb_ok = marques.count(".")
    nb_ko = marques.count("x") + marques.count("t")
    # Chaque ligne est étiquetée par l'indice de départ
    for k in range(0, len(marques), LARGEUR):
        print(f" {k:3d} : " + marques[k:k + LARGEUR])
    print(f"Nombre de tests OK : {nb_ok}")
    print(f"Nombre de tests KO : {nb_ko}")


if __name__ == "__main__":
    main()
