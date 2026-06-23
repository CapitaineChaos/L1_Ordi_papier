#!/usr/bin/env python3

#===============/                    ex10.6                    \================
#=~~~~~~~~~~~~=|               Ordinateur en papier             |=~~~~~~~~~~~~~=
#===============\          Wrapper de test multithread         /================
#
# Auteur : Sylvain Maitre     24002886
#
# Date de création :              22/06/2026
# Date de dernière modification : 23/06/2026
#
# Description : Teste un programme de division (fourni en .hex) sur tous les
#               couples (i, j) avec 0 <= i < 256 et 1 <= j < 256, et compare
#               la sortie de pico au quotient calculé par Python.
# Utilisation : ./test_div.py <programme.hex>
#
#===============================================================================

import os
import sys
import subprocess
from concurrent.futures import ThreadPoolExecutor

PICO = "../bin/pico"
NPROC = os.cpu_count()           # threads simultanés
LARGEUR = 128                    # marques par ligne


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


def div(prog, i, j):
    try:
        p = subprocess.run(
            [PICO],
            # Ajouter i et j au stdin en hexa pour remplacer le clavier
            input=f"{prog} {i:02X} {j:02X}",
            capture_output=True, text=True, timeout=5,
        )
        return "." if p.stdout.strip() == str(i // j) else "x"
    except subprocess.TimeoutExpired:
        return "t"


def main():
    prog = lire_programme()
    nb_ok = nb_ko = 0
    with ThreadPoolExecutor(max_workers=NPROC) as executor:
        for i in range(256):
            marques = "-" + "".join(executor.map(lambda j: div(prog, i, j), range(1, 256)))
            nb_ok += marques.count(".")
            nb_ko += marques.count("x") + marques.count("t")
            entete = f" {i:3d} : "
            espaces = " " * len(entete)
            # Afficher largeur par largeur
            for k in range(0, len(marques), LARGEUR):
                print((entete if k == 0 else espaces) + marques[k:k + LARGEUR])
    print(f"Nombre de tests OK : {nb_ok}")
    print(f"Nombre de tests KO : {nb_ko}")


if __name__ == "__main__":
    main()
