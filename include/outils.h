/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\                  Utilitaires                 /================

Auteur : Sylvain Maitre     24002886

Date de création :              12/06/2026
Date de dernière modification : 22/06/2026

Fichier     : outils.h
Description : Fonctions utilitaires partagées

==============================================================================*/

#ifndef OUTILS_H
# define OUTILS_H

# include "pico_types.h"
# include <stdbool.h>

bool	pico_parse_u8(const char *str, u8 *val);

#endif
