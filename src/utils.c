/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\                  Utilitaires                 /================

Auteur : Sylvain Maitre     24002886

Date de création :              12/06/2026
Date de dernière modification : 12/06/2026

Fichier     : utils.c
Description : Fonctions utilitaires partagées

==============================================================================*/

#include "utils.h"
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>

/**
 * @brief Analyse une chaîne de chars pour extraire un int non signé sur 8 bits.
 *
 * @param str Chaîne de caractères à analyser.
 * @param val Pointeur vers la variable où stocker la valeur analysée.
 * @return true si l'analyse a réussi, false sinon.
 */
bool	pico_parse_u8(const char *str, u8 *val) {
	char			*end;
	unsigned long	nb;

	if (!str || !val)
		return (false);
	while (isspace((unsigned char)*str))
		str++;
	if (*str == '\0')
		return (false);
	errno = 0;
	nb = strtoul(str, &end, 0);
	while (isspace((unsigned char)*end))
		end++;
	if (errno || *end != '\0' || nb > UCHAR_MAX)
		return (false);
	*val = (u8)nb;
	return (true);
}
