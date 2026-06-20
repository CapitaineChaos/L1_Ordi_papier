/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\          Affichage différentiel debug        /================

Auteur : Sylvain Maitre     24002886

Date de création :              11/06/2026
Date de dernière modification : 20/06/2026

Fichier     : debogueur/affichage.h
Description : Déclarations du display différentiel du débogueur

==============================================================================*/

#ifndef DBG_AFFICHAGE_H
# define DBG_AFFICHAGE_H

# include "debogueur.h"
# include <stdbool.h>
# include <stdio.h>

FILE	*dbg_get_tty(Dbg *dbg);
void	dbg_display_enter(Dbg *dbg);
void	dbg_display_leave(Dbg *dbg);
void	dbg_display_raw_enter(Dbg *dbg);
void	dbg_display_raw_leave(Dbg *dbg);
bool	dbg_display_terminal_ok(Dbg *dbg);
void	dbg_display_draw(Dbg *dbg);
void	dbg_display_trop_petit(Dbg *dbg);

#endif
