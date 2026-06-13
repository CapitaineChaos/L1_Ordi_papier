/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\            Commandes du débogueur             /================

Auteur : Sylvain Maitre     24002886

Date de création :              11/06/2026
Date de dernière modification : 11/06/2026

Fichier     : dbg/cmd.h
Description : Déclarations du parser de commandes debug

==============================================================================*/

#ifndef DBG_CMD_H
# define DBG_CMD_H

# include "dbg.h"
# include <stdbool.h>

dbg_cmd	attente_commande(Mini_ordi *pico, Dbg *dbg, bool err);
void	vider_etat_temporaire(Dbg *dbg, bool err);

#endif
