/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\             Saisie clavier debug             /================

Auteur : Sylvain Maitre     24002886

Date de création :              20/06/2026
Date de dernière modification : 22/06/2026

Fichier     : debogueur/saisie.c
Description : Saisie clavier centralisée du débogueur

Centralise le squelette commun aux boucles de saisie (commande, hexa, menu) :
mode raw, gestion EOF/EINTR/redimensionnement, Ctrl-C/Ctrl-D, et filtrage des
séquences d'échappement ANSI. Les appelants ne traitent plus que les touches
significatives renvoyées par dbg_input_touche().

==============================================================================*/

#include "debogueur/saisie.h"
#include "debogueur/affichage.h"
#include "debogueur/redim.h"
#include <errno.h>
#include <stdlib.h>

/**
 * @brief Démarre une session de saisie
 * @param dbg Le débogueur
 * @note Arme la détection de redimensionnement et passe le terminal en mode raw
 */
void	dbg_input_debut(Dbg *dbg) {
	dbg_resize_init();
	dbg_display_raw_enter(dbg);
}

/**
 * @brief Termine une session de saisie
 * @param dbg Le débogueur
 * @note Rend le terminal à son mode canonique
 */
void	dbg_input_fin(Dbg *dbg) {
	dbg_display_raw_leave(dbg);
}

/**
 * @brief Lit une touche significative au clavier
 * @param dbg Le débogueur
 * @param stream Le flux de lecture (tty, ou flux d'entrée utilisateur)
 * @return L'octet saisi, ou un code DBG_KEY_* normalisé
 * @note Ré-arme le mode raw en tête (le retour depuis l'affichage « trop petit »
 * a pu le quitter). Absorbe les EINTR sans redimensionnement, les séquences
 * d'échappement ANSI (flèches, molette...) et les sorties Ctrl-C/Ctrl-D.
 */
int	dbg_input_touche(Dbg *dbg, FILE *stream) {
	int	c;
	int	echappement = 0;

	dbg_display_raw_enter(dbg);
	while (true) {
		c = fgetc(stream);
		if (c == EOF) {
			if (ferror(stream) && errno == EINTR) {
				clearerr(stream);
				if (dbg_resize_recu()) {
					dbg_resize_reset();
					return (DBG_KEY_RESIZE);
				}
				continue;
			}
			return (DBG_KEY_EOF);
		}
		// Ctrl-C ou Ctrl-D : quitter le débogueur
		if (c == 3 || c == 4) {
			dbg_input_fin(dbg);
			dbg_display_leave(dbg);
			exit(0);
		}
		// Filtrage des séquences d'échappement ANSI (flèches, molette...)
		if (c == 27) {
			echappement = 1;
			continue;
		}
		if (echappement == 1) {
			echappement = (c == '[' || c == 'O') ? 2 : 0;
			continue;
		}
		if (echappement == 2) {
			if (c >= '@' && c <= '~')
				echappement = 0;
			continue;
		}
		if (c == '\n' || c == '\r')
			return (DBG_KEY_ENTER);
		if (c == 127 || c == '\b')
			return (DBG_KEY_BACKSPACE);
		return (c);
	}
}
