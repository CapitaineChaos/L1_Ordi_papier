/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\              Viewer du débogueur              /================

Auteur : Sylvain Maitre     24002886

Date de création :              11/06/2026
Date de dernière modification : 12/06/2026

Fichier     : dbg/viewer.h
Description : Déclarations du viewer du débogueur

==============================================================================*/

#ifndef DBG_VIEWER_H
# define DBG_VIEWER_H

# include "dbg.h"
# include <stdbool.h>

bool	compositeur_ecran(Mini_ordi *pico, Dbg *dbg, int phase, t_mcseq *mseq, int pos);
bool	compositeur_stdin(Mini_ordi *pico, Dbg *dbg);
bool	compositeur_output(Mini_ordi *pico, Dbg *dbg);

#endif
