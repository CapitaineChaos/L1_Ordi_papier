/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\             Saisie clavier debug             /================

Auteur : Sylvain Maitre     24002886

Date de création :              20/06/2026
Date de dernière modification : 22/06/2026

Fichier     : debogueur/saisie.h
Description : Déclarations de la saisie clavier centralisée du débogueur

==============================================================================*/

#ifndef DBG_SAISIE_H
# define DBG_SAISIE_H

# include "debogueur.h"
# include <stdio.h>

/* Codes de touches normalisés. Négatifs pour ne pas heurter les octets 0..255 */
enum {
	DBG_KEY_EOF       = -1,	/* flux fermé / erreur non récupérable */
	DBG_KEY_RESIZE    = -2,	/* SIGWINCH reçu pendant la lecture */
	DBG_KEY_ENTER     = -3,
	DBG_KEY_BACKSPACE = -4,
};

void	dbg_input_debut(Dbg *dbg);
void	dbg_input_fin(Dbg *dbg);
int		dbg_input_touche(Dbg *dbg, FILE *stream);

#endif
