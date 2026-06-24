/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\         Parser des commandes debug           /================

Auteur : Sylvain Maitre     24002886

Date de création :              12/06/2026
Date de dernière modification : 22/06/2026

Fichier     : debogueur/analyseur.h
Description : Déclarations du parser de commandes debug

==============================================================================*/

#ifndef DBG_ANALYSEUR_H
# define DBG_ANALYSEUR_H

# include "debogueur.h"

dbg_cmd	parser_commande(Mini_ordi *pico, Dbg *dbg, const char *str);

#endif
