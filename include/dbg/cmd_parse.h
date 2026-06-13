/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\         Parser des commandes debug           /================

Auteur : Sylvain Maitre     24002886

Date de création :              12/06/2026
Date de dernière modification : 12/06/2026

Fichier     : dbg/cmd_parse.h
Description : Déclarations du parser de commandes debug

==============================================================================*/

#ifndef DBG_CMD_PARSE_H
# define DBG_CMD_PARSE_H

# include "dbg.h"

dbg_cmd	parser_commande(Mini_ordi *pico, Dbg *dbg, const char *str);

#endif
