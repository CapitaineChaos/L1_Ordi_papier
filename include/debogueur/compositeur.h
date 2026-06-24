/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\         Composition des écrans debug         /================

Auteur : Sylvain Maitre     24002886

Date de création :              20/06/2026
Date de dernière modification : 22/06/2026

Fichier     : debogueur/compositeur.h
Description : Déclarations de la composition partagée du débogueur

==============================================================================*/

#ifndef DBG_COMPOSITEUR_H
# define DBG_COMPOSITEUR_H

# include "debogueur.h"
# include <stdbool.h>

bool	compositeur_debut(Dbg *dbg);
bool	compositeur_overlay_ok(Dbg *dbg);
void	compositeur_top_menu(Mini_ordi *pico, Dbg *dbg);
void	compositeur_bottom(Dbg *dbg, int line);
void	compositeur_attente(Mini_ordi *pico, Dbg *dbg, bool err, const char *str);
void	compositeur_invite_entree(Mini_ordi *pico, Dbg *dbg, const char *hex);

#endif
