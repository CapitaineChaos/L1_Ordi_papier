/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\              Menus du débogueur              /================

Auteur : Sylvain Maitre     24002886

Date de création :              12/06/2026
Date de dernière modification : 20/06/2026

Fichier     : dbg/menu.c
Description : Navigation dans les menus debug

==============================================================================*/

#include "dbg/menu.h"
#include "dbg/compositeur.h"
#include "dbg/display.h"
#include "dbg/input.h"
#include "dbg/keys.h"
#include "dbg/render.h"
#include "dbg/viewer.h"
#include "messages.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Affiche une ligne d'aide du débogueur
 * @param dbg Le débogueur
 * @param line Le numéro de la ligne à afficher
 * @param debut Le début de la chaîne d'aide
 * @param fin La fin de la chaîne d'aide
 * @note On aurait pu mettre des \0 pour remplacer les \\n
 * mais render_lg_col() demandait déjà la longueur 
 */
static void	afficher_ligne_aide(Dbg *dbg, int line,
	const char *debut, const char *fin) {
	render_set_line(dbg, line, DBG_FOND_BK);
	render_lg_col(dbg, line, 1, DBG_HELP_TEXT, (int)(fin - debut), debut);
}

/**
 * @brief Compose l'aide du débogueur dans le buffer
 * @param pico Le mini-ordinateur
 * @param dbg Le débogueur
 * @return true si l'écran a été composé, false si le terminal n'est pas affichable
 */
static bool	compositeur_aide(Mini_ordi *pico, Dbg *dbg) {
	const char	*debut;
	const char	*fin;
	int			line;

	if (!compositeur_debut(dbg))
		return (false);
	compositeur_top_menu(pico, dbg);
	render_set_line(dbg, 2, DBG_FOND_WH);
	debut = MSG_CMDS;
	line = 3;
	// Découper l'aide en lignes par '\n' et afficher chaque ligne
	// On parcourt d'abord selon la place disponible
	while (*debut && line < DBG_STATE_LINE) {
		fin = debut;
		while (*fin && *fin != '\n')
			fin++;
		afficher_ligne_aide(dbg, line, debut, fin);
		line++;
		debut = fin;
		if (*debut == '\n')
			debut++;
	}
	compositeur_bottom(dbg, line);
	return (true);
}

/**
 * @brief Compose puis affiche le menu du débogueur en fonction de la commande
 * @param pico Le mini-ordinateur
 * @param dbg Le débogueur
 * @param menu Le menu actuel
 * @note Orchestre la composition (compositeur_*) puis l'affichage différentiel unique.
 */
static void	afficher_menu(Mini_ordi *pico, Dbg *dbg, dbg_cmd menu) {
	bool	compose = false;

	if (menu == DBG_CMD_SHOW_HELP)
		compose = compositeur_aide(pico, dbg);
	else if (menu == DBG_CMD_SHOW_INPUT && IO_STDIN_AVAILABLE)
		compose = compositeur_stdin(pico, dbg);
	else if (menu == DBG_CMD_SHOW_OUTPUT && IO_OUTPUT_AVAILABLE)
		compose = compositeur_output(pico, dbg);
	if (compose)
		dbg_display_draw(dbg);
}

/**
 * @brief Attend une commande dans le menu du débogueur
 * @param pico Le mini-ordinateur
 * @param dbg Le débogueur
 * @param menu Le menu actuel
 * @return La commande reçue
 * @note Il y a 2 types de commandes :
 * @note - Les commandes menu (directes après un appui sur une touche)
 * @note - Les commandes normales (après avoir appuyé sur Entrée)
 */
dbg_cmd	attendre_menu(Mini_ordi *pico, Dbg *dbg, dbg_cmd menu) {
	FILE	*tty;
	int		c;
	dbg_cmd	cmd;

	tty = dbg_get_tty(dbg);
	if (!tty)
		return (DBG_CMD_STOP_DBG);
	afficher_menu(pico, dbg, menu);
	dbg_input_debut(dbg);
	while (true) {
		c = dbg_input_touche(dbg, tty);
		if (c == DBG_KEY_EOF) {
			dbg_input_fin(dbg);
			return (DBG_CMD_STOP_DBG);
		}
		if (c == DBG_KEY_RESIZE) {
			afficher_menu(pico, dbg, menu);
			continue;
		}
		cmd = dbg_key_directe(pico, c);
		if (cmd == DBG_CMD_QUIT || cmd == DBG_CMD_STOP_DBG)
			cmd = DBG_CMD_NONE;
		if (cmd == DBG_CMD_SHOW_HELP || cmd == DBG_CMD_SHOW_INPUT || cmd == DBG_CMD_SHOW_OUTPUT) {
			menu = cmd;
			afficher_menu(pico, dbg, menu);
			continue;
		}
		dbg_input_fin(dbg);
		return (DBG_CMD_HELP);
	}
}
