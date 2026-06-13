/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\             Header entrées/sorties            /================

Auteur : Sylvain Maitre     24002886

Fichier     : io.h
Description : Déclarations publiques des entrées/sorties

==============================================================================*/

#ifndef IO_H
# define IO_H

# include "pico_types.h"

void	afficher_sortie(Mini_ordi *pico, u8 val);
void	charger_buffer_entree(Mini_ordi *pico);
u8		lire_entree(Mini_ordi *pico, u8 PC);

#endif
