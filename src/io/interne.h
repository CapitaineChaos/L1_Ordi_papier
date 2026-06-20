/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\             Internes des entrées             /================

Auteur : Sylvain Maitre     24002886

Date de création :              12/06/2026
Date de dernière modification : 20/06/2026

Fichier     : io/interne.h
Description : Fonctions internes partagées des entrées/sorties

==============================================================================*/

#ifndef IO_INTERNAL_H
# define IO_INTERNAL_H

# include "pico_types.h"
# include <stdbool.h>
# include <stdio.h>

bool	io_lire_octet_hex(FILE *stream, u8 *val, bool clavier);
bool	io_lire_octet_dec(FILE *stream, u8 *val);
bool	io_parse_hex_byte(const char *hex, u8 *val);
bool	io_parse_dec_byte(const char *dec, u8 *val);
FILE	*io_flux_entree_utilisateur(Mini_ordi *pico);
u8		io_lire_entree_debug(Mini_ordi *pico);

#endif
