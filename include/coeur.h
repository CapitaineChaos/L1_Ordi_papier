/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\                Header du coeur               /================

Auteur : Sylvain Maitre     24002886

Date de création :              08/10/2025
Date de dernière modification : 06/06/2026

Fichier     : coeur.h
Description : Déclarations des fonctions du coeur du mini-ordinateur

==============================================================================*/

#ifndef COEUR_H
# define COEUR_H

#include "pico_types.h"

int	exec_microcode(Mini_ordi *pico, Dbg *dbg, int pos);
int	coeur(Mini_ordi *pico, Dbg *dbg);

#endif
