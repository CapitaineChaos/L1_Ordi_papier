/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\                  Utilitaires                 /================

Auteur : Sylvain Maitre     24002886

Date de création :              12/06/2026
Date de dernière modification : 12/06/2026

Fichier     : utils.h
Description : Fonctions utilitaires partagées

==============================================================================*/

#ifndef UTILS_H
# define UTILS_H

# include "pico_types.h"
# include <stdbool.h>

bool	pico_parse_u8(const char *str, u8 *val);

#endif
