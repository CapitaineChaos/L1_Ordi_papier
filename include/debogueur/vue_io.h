/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\              Vues IO du débogueur            /================

Auteur : Sylvain Maitre     24002886

Date de création :              20/06/2026
Date de dernière modification : 20/06/2026

Fichier     : debogueur/vue_io.h
Description : Déclarations des vues d'entrée et de sortie du débogueur

==============================================================================*/

#ifndef DBG_VUE_IO_H
# define DBG_VUE_IO_H

# include "debogueur.h"
# include <stdbool.h>

bool	compositeur_stdin(Mini_ordi *pico, Dbg *dbg);
bool	compositeur_output(Mini_ordi *pico, Dbg *dbg);

#endif
