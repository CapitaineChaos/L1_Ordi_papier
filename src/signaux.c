/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\              Gestion des signaux             /================

Auteur : Sylvain Maitre     24002886

Date de création :              13/06/2026
Date de dernière modification : 23/06/2026

Fichier     : signaux.c
Description : Gestion des signaux du programme

==============================================================================*/

#include "signaux.h"
#include "ansi.h"
#include "messages.h"
#include <signal.h>
#include <stddef.h>
#include <unistd.h>

static volatile sig_atomic_t	g_debug_display = 0;

static void	signal_write(const char *str) {
	size_t	len;

	len = 0;
	while (str[len])
		len++;
	write(STDERR_FILENO, str, len);
}

static const char	*signal_message(int sig) {
	if (sig == SIGINT)
		return (MSG_SIG_INT);
	if (sig == SIGTERM)
		return (MSG_SIG_TERM);
	if (sig == SIGQUIT)
		return (MSG_SIG_QUIT);
	if (sig == SIGHUP)
		return (MSG_SIG_HUP);
	if (sig == SIGTSTP)
		return (MSG_SIG_STP);
	return (MSG_SIG_AUTRE);
}

void	signal_set_debug_display(int actif) {
	g_debug_display = (actif != 0);
}

/**
 * @brief Handler pour intercepter les signaux d'interruption.
 * @param sig Signal reçu.
 * @note Gère SIGINT (CTRL+C), SIGTERM, SIGQUIT (CTRL+\), SIGHUP et SIGTSTP (CTRL+Z).
 * @note Réinitialise les couleurs du terminal avant de quitter proprement.
 */
void	signal_handler(int sig) {
	if (g_debug_display)
		signal_write(RST D_WRAP D_SHOW_CURSOR D_MAIN_SCREEN E_LINE_ALL "\r");
	signal_write("\n");
	signal_write(signal_message(sig));
	_exit(128 + sig);
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
