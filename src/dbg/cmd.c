/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\            Commandes du débogueur             /================

Auteur : Sylvain Maitre     24002886

Date de création :              11/06/2026
Date de dernière modification : 20/06/2026

Fichier     : dbg/cmd.c
Description : Lecture et interprétation des commandes debug

==============================================================================*/

#include "dbg/cmd.h"
#include "dbg/cmd_parse.h"
#include "dbg/compositeur.h"
#include "dbg/display.h"
#include "dbg/input.h"
#include "dbg/keys.h"
#include "dbg/menu.h"
#include "dbg/render.h"
#include "messages.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Vide l'état temporaire du débogueur
 * @param dbg Le débogueur
 * @param err Indique si une erreur s'est produite
 */
void	vider_etat_temporaire(Dbg *dbg, bool err) {
	dbg->texte.lg_status[0] = '\0';
	if (!err)
		dbg->erreur_prog = 0;
	(void)err;
	render_set_line(dbg, DBG_STATE_LINE, DBG_STATE_FOND);
}

/**
 * @brief Compose puis affiche les 3 lignes de fin pour une attente de commande
 * @param pico Le mini-ordinateur
 * @param dbg Le débogueur
 * @param err Si la ligne de status doit signaler une commande inconnue
 * @param str La commande en cours de saisie, affichée après l'invite
 */
static void	afficher_attente(Mini_ordi *pico, Dbg *dbg, bool err, const char *str) {
	if (!compositeur_overlay_ok(dbg))
		return;
	compositeur_attente(pico, dbg, err, str);
	// Le viewer différentiel ne redessine que les lignes modifiées (les 3 du bas)
	dbg_display_draw(dbg);
}

/**
 * @brief Lit une ligne de commande saisie par l'utilisateur
 * @param dbg Le débogueur
 * @param err Si la ligne de status doit afficher une erreur ou pas
 * @param str La chaîne à afficher dans la ligne de commande après l'invite
 * @param max_size La taille de la chaîne str
 * @return Le code de commande lu
 * @note La saisie brute (raw, EOF, redimensionnement, échappements) est déléguée
 * à dbg_input_touche() ; on ne traite ici que les touches significatives.
 */
static dbg_cmd	lire_ligne_commande(Mini_ordi *pico, Dbg *dbg, bool err, char *str, size_t max_size) {
	FILE	*tty;
	size_t	len;
	int		c;

	tty = dbg_get_tty(dbg);
	if (!tty)
		return (DBG_CMD_STOP_DBG);
	len = 0;
	str[0] = '\0';
	dbg_input_debut(dbg);
	afficher_attente(pico, dbg, err, str);
	while (true) {
		c = dbg_input_touche(dbg, tty);
		if (c == DBG_KEY_EOF) {
			dbg_input_fin(dbg);
			return (DBG_CMD_STOP_DBG);
		}
		if (c == DBG_KEY_RESIZE) {
			// Retour d'un terminal trop petit à une taille correcte : on relance
			// l'écran complet depuis l'appelant
			if (dbg->terminal.petit_affiche && dbg_display_terminal_ok(dbg) && len == 0) {
				dbg_input_fin(dbg);
				return (DBG_CMD_REFRESH);
			}
			afficher_attente(pico, dbg, err, str);
			continue;
		}
		if (c == DBG_KEY_ENTER)
			break;
		if (len == 0) {
			dbg_cmd	direct = dbg_key_directe(pico, c);
			if (direct != DBG_CMD_NONE) {
				dbg_input_fin(dbg);
				return (direct);
			}
		}
		if (c == DBG_KEY_BACKSPACE) {
			if (len > 0) {
				str[--len] = '\0';
				afficher_attente(pico, dbg, err, str);
			}
			continue;
		}
		if (isprint(c) && len + 1 < max_size) {
			str[len++] = (char)c;
			str[len] = '\0';
			afficher_attente(pico, dbg, err, str);
		}
	}
	dbg_input_fin(dbg);
	vider_etat_temporaire(dbg, err);
	return (DBG_CMD_NONE);
}

dbg_cmd	attente_commande(Mini_ordi *pico, Dbg *dbg, bool err) {
	FILE	*tty = dbg_get_tty(dbg);
	dbg_cmd	ret;
	char	str[64];

	if (!tty)
		return (DBG_CMD_STOP_DBG);
	ret = lire_ligne_commande(pico, dbg, err, str, sizeof(str));
	if (ret == DBG_CMD_QUIT) {
		dbg_display_leave(dbg);
		exit(0);
	}
	if (ret == DBG_CMD_SHOW_HELP || ret == DBG_CMD_SHOW_INPUT || ret == DBG_CMD_SHOW_OUTPUT)
		return (attendre_menu(pico, dbg, ret));
	if (ret != DBG_CMD_NONE)
		return (ret);
	if (str[0] == '\0' || str[0] == '\n')
		return (DBG_CMD_AUTO);
	fprintf(tty, RST);
	ret = parser_commande(pico, dbg, str);
	if (ret == DBG_CMD_QUIT) {
		dbg_display_leave(dbg);
		exit(0);
	}
	return (ret);
}
