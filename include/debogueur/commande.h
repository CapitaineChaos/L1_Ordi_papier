/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\            Commandes du débogueur             /================

Auteur : Sylvain Maitre     24002886

Date de création :              11/06/2026
Date de dernière modification : 20/06/2026

Fichier     : debogueur/commande.h
Description : Déclarations du parser de commandes debug

==============================================================================*/

#ifndef DBG_COMMANDE_H
# define DBG_COMMANDE_H

# include "debogueur.h"
# include <stdbool.h>

dbg_cmd	attente_commande(Mini_ordi *pico, Dbg *dbg, bool err);
void	vider_etat_temporaire(Dbg *dbg);

#endif
