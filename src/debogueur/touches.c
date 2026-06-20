/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\             Touches du débogueur             /================

Auteur : Sylvain Maitre     24002886

Date de création :              12/06/2026
Date de dernière modification : 20/06/2026

Fichier     : debogueur/touches.c
Description : Gestion des touches directes

==============================================================================*/

#include "debogueur/touches.h"
#include <ctype.h>

/**
 * @brief Gère les touches directes du débogueur
 * @param pico Le mini-ordinateur
 * @param c Le caractère saisi
 * @return La commande correspondante
 */
dbg_cmd	dbg_key_directe(Mini_ordi *pico, int c) {
	c = tolower((unsigned char)c);
	if (c == 'q')
		return (DBG_CMD_QUIT);
	if (c == 's')
		return (DBG_CMD_STOP_DBG);
	if (c == 'h' || c == '?')
		return (DBG_CMD_SHOW_HELP);
	if (c == 'i') {
		if (IO_STDIN_AVAILABLE)
			return (DBG_CMD_SHOW_INPUT);
		return (DBG_CMD_UNAVAILABLE);
	}
	if (c == 'o') {
		if (IO_OUTPUT_AVAILABLE)
			return (DBG_CMD_SHOW_OUTPUT);
		return (DBG_CMD_UNAVAILABLE);
	}
	return (DBG_CMD_NONE);
}
