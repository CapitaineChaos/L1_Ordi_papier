/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\              Décodage affichage              /================

Auteur : Sylvain Maitre     24002886

Date de création :              12/06/2026
Date de dernière modification : 12/06/2026

Fichier     : dbg/decode.h
Description : Déclarations du décodage textuel du débogueur

==============================================================================*/

#ifndef DBG_DECODE_H
# define DBG_DECODE_H

# include "dbg.h"

void	decode_microcode(int microcode, Dbg *dbg);
void	decode_instruction(Mini_ordi *pico, Dbg *dbg);

#endif
