/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\              Menus du débogueur              /================

Auteur : Sylvain Maitre     24002886

Date de création :              12/06/2026
Date de dernière modification : 12/06/2026

Fichier     : dbg/menu.h
Description : Navigation dans les menus debug

==============================================================================*/

#ifndef DBG_MENU_H
# define DBG_MENU_H

# include "dbg.h"

dbg_cmd	attendre_menu(Mini_ordi *pico, Dbg *dbg, dbg_cmd menu);

#endif
