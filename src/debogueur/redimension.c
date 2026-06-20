/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\         Gestion du redimensionnement         /================

Auteur : Sylvain Maitre     24002886

Date de création :              13/06/2026
Date de dernière modification : 20/06/2026

Fichier     : debogueur/redimension.c
Description : Gestion du redimensionnement du terminal pour le débogueur

==============================================================================*/

#include "debogueur/redim.h"
#include <signal.h>
#include <stddef.h>

static volatile sig_atomic_t	g_sigwinch = 0;

static void	sigwinch_handler(int sig) {
	(void)sig;
	g_sigwinch = 1;
}

void	dbg_resize_init(void) {
	struct sigaction	sa;
	static bool			installe = false;

	if (installe)
		return;
	sa = (struct sigaction){0};
	sa.sa_handler = sigwinch_handler;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGWINCH, &sa, NULL);
	installe = true;
}

bool	dbg_resize_recu(void) {
	return (g_sigwinch != 0);
}

void	dbg_resize_reset(void) {
	g_sigwinch = 0;
}
