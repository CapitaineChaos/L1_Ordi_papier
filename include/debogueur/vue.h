/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\               Vue du débogueur               /================

Auteur : Sylvain Maitre     24002886

Date de création :              11/06/2026
Date de dernière modification : 20/06/2026

Fichier     : debogueur/vue.h
Description : Déclarations de la vue principale du débogueur

==============================================================================*/

#ifndef DBG_VUE_H
# define DBG_VUE_H

# include "debogueur.h"
# include <stdbool.h>

bool	compositeur_ecran(Mini_ordi *pico, Dbg *dbg, int phase, t_mcseq *mseq, int pos);

#endif
