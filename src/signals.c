/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\              Gestion des signaux             /================

Auteur : Sylvain Maitre     24002886

Date de création :              13/06/2026
Date de dernière modification : 13/06/2026

Fichier     : signals.c
Description : Gestion des signaux du programme

==============================================================================*/

#include "signals.h"
#include "ansi.h"
#include "messages.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Handler pour intercepter les signaux d'interruption.
 * @param sig Signal reçu.
 * @note Gère SIGINT (CTRL+C), SIGTERM, SIGQUIT (CTRL+\), SIGHUP et SIGTSTP (CTRL+Z).
 * @note Réinitialise les couleurs du terminal avant de quitter proprement.
 */
void	signal_handler(int sig) {
	printf(RST D_WRAP D_SHOW_CURSOR D_MAIN_SCREEN E_LINE_ALL "\n\n");
	msg_print_signal(sig);
	printf(RST D_WRAP D_SHOW_CURSOR E_LINE_ALL "\n");
	exit(EXIT_SUCCESS);
}

/**
 * @brief Initialise les handlers pour les signaux d'interruption.
 */
void	init_sig(void) {
	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);
	signal(SIGQUIT, signal_handler);
	signal(SIGHUP, signal_handler);
	signal(SIGTSTP, signal_handler);
}
