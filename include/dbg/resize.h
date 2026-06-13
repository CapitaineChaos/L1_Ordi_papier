/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\         Gestion du redimensionnement         /================

Auteur : Sylvain Maitre     24002886

Date de création :              13/06/2026
Date de dernière modification : 13/06/2026

Fichier     : resize.h
Description : Gestion du redimensionnement du terminal pour le débogueur

==============================================================================*/


#ifndef DBG_RESIZE_H
# define DBG_RESIZE_H

# include <stdbool.h>

void	dbg_resize_init(void);
bool	dbg_resize_recu(void);
void	dbg_resize_reset(void);

#endif
