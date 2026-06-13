/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\              Helpers ANSI debug              /================

Auteur : Sylvain Maitre     24002886

Date de création :              11/06/2026
Date de dernière modification : 11/06/2026

Fichier     : dbg/ansi.h
Description : Mesures de chaînes contenant des séquences ANSI

==============================================================================*/

#ifndef DBG_ANSI_H
# define DBG_ANSI_H

# include <stddef.h>

size_t	ansi_sequence_len(const char *str);
size_t	ansi_visible_len(const char *str);

#endif
