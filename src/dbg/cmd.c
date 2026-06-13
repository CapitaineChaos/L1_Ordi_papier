/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\            Commandes du débogueur             /================

Auteur : Sylvain Maitre     24002886

Date de création :              11/06/2026
Date de dernière modification : 13/06/2026

Fichier     : dbg/cmd.c
Description : Lecture et interprétation des commandes debug

==============================================================================*/

#include "dbg/cmd.h"
#include "dbg/cmd_parse.h"
#include "dbg/display.h"
#include "dbg/keys.h"
#include "dbg/resize.h"
#include "dbg/menu.h"
#include "dbg/render.h"
#include "messages.h"
#include <ctype.h>
#include <errno.h>
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
 * @brief Affiche les 3 lignes de fin dans le cadre d'une attente de commande
 * @param dbg Le débogueur
 * @param err Si la ligne de status doit afficher une erreur ou pas
 * @param str La chaîne à afficher dans la ligne de commande après l'invite
 * @note str contient la commande en cours de saisie par l'utilisateur, elle est affichée après l'invite de commande
 */
static void	afficher_attente(Mini_ordi *pico, Dbg *dbg, bool err, const char *str) {
	FILE	*tty;

	tty = dbg_get_tty(dbg);
	// Si le terminal est trop petit on bascule sur un affichage minimaliste
	if (!dbg_display_terminal_ok(dbg))
	{
		dbg_display_trop_petit(dbg);
		(void)tty;
		return;
	}
	// Si y a eu une erreur précédemment
	render_set_line(dbg, DBG_STATE_LINE, DBG_STATE_FOND);
	render_set_line(dbg, DBG_PROMPT_LINE, DBG_FOND_BK);
	render_set_line(dbg, DBG_COMMAND_LINE, DBG_FOND_BK);

	if (dbg->erreur_prog) {
		if (dbg->erreur_prog == 3)
			render_set_text(dbg->texte.lg_status, sizeof(dbg->texte.lg_status),
				MSG_ERR_MEMORY, pico->RS);
		else if (dbg->erreur_prog == 4)
			render_set_text(dbg->texte.lg_status, sizeof(dbg->texte.lg_status),
				MSG_ERR_INSTRUCTION, pico->OP);
		else if (dbg->erreur_prog == 5)
			render_set_text(dbg->texte.lg_status, sizeof(dbg->texte.lg_status),
				MSG_ERR_CORE);
		else
			render_set_text(dbg->texte.lg_status, sizeof(dbg->texte.lg_status),
				MSG_ERR_INCONNUE, dbg->erreur_prog);
		render_set_line(dbg, DBG_STATE_LINE, DBG_FOND_RD);
		render_lg_col(dbg, DBG_STATE_LINE, 17, dbg->texte.lg_status);
	}
	else if (err)
		render_lg_col(dbg, DBG_STATE_LINE, 1, DBG_STATE_UNKNOWN_CMD);
	else if (dbg->texte.lg_status[0] != '\0')
		render_lg_col(dbg, DBG_STATE_LINE, 1, dbg->texte.lg_status);


	render_lg_col(dbg, DBG_PROMPT_LINE, 0, DBG_CMD_LIST);
	render_lg_col(dbg, DBG_COMMAND_LINE, 0, DBG_INVITE(err));
	render_append_to_line(dbg, DBG_COMMAND_LINE, "%s", str ? str : "");
	dbg_display_interaction(dbg);
}

/**
 * @brief Lit une ligne de commande saisie par l'utilisateur
 * @param dbg Le débogueur
 * @param err Si la ligne de status doit afficher une erreur ou pas
 * @param str La chaîne à afficher dans la ligne de commande après l'invite
 * @param size La taille de la chaîne str
 * @note Mettre à jour à chaque touche appuyée
 * @return Le code de commande lu
 */
static dbg_cmd	lire_ligne_commande(Mini_ordi *pico, Dbg *dbg, bool err, char *str, size_t size) {
	FILE	*tty;
	size_t	len;
	int		c;
	int		echappement;

	tty = dbg_get_tty(dbg);
	if (!tty)
		return (DBG_CMD_STOP_DBG);
	len = 0;
	echappement = 0;
	str[0] = '\0';
	dbg_resize_init();
	dbg_display_raw_enter(dbg);
	afficher_attente(pico, dbg, err, str);
	while (true) {
		c = fgetc(tty);
		if (c == EOF) {
			// Si on reçoit un signal de redimensionnement du terminal, on redessine l'écran
			if (ferror(tty) && errno == EINTR && dbg_resize_recu()) {
				clearerr(tty);
				dbg_resize_reset();
				if (dbg->terminal.petit_affiche && dbg_display_terminal_ok(dbg) && len == 0) {
					dbg_display_raw_leave(dbg);
					return (DBG_CMD_REFRESH);
				}
				afficher_attente(pico, dbg, err, str);
				dbg_display_raw_enter(dbg);
				continue;
			}
			if (ferror(tty) && errno == EINTR) {
				clearerr(tty);
				continue;
			}
			if (feof(tty))
				exit(0);
			dbg_display_raw_leave(dbg);
			return (DBG_CMD_STOP_DBG);
		}
		// Ctrl+C ou Ctrl+D pour quitter le débogueur
		if (c == 3 || c == 4) {
			dbg_display_raw_leave(dbg);
			dbg_display_leave(dbg);
			exit(0);
		}
		// Ignorer les séquences d'échappement ANSI pour les touches fléchées et autres
		if (c == 27) {
			echappement = 1;
			continue;
		}
		// Si on est dans une séquence d'échappement, on ignore les caractères suivants jusqu'à la fin de la séquence
		if (echappement == 1) {
			echappement = (c == '[' || c == 'O') ? 2 : 0;
			continue;
		}
		// Si on est dans une séquence d'échappement, on ignore les caractères suivants jusqu'à la fin de la séquence
		if (echappement == 2) {
			if (c >= '@' && c <= '~')
				echappement = 0;
			continue;
		}
		if (c == '\n' || c == '\r')
			break;
		if (len == 0) {
			dbg_cmd	direct = dbg_key_directe(pico, c);
			if (direct != DBG_CMD_NONE) {
				dbg_display_raw_leave(dbg);
				return (direct);
			}
		}
		if ((c == 127 || c == '\b') && len > 0) {
			len--;
			str[len] = '\0';
			afficher_attente(pico, dbg, err, str);
			continue;
		}
		if (isprint((unsigned char)c) && len + 1 < size) {
			str[len++] = c;
			str[len] = '\0';
			afficher_attente(pico, dbg, err, str);
		}
	}
	dbg_display_raw_leave(dbg);
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
	if (ret == DBG_CMD_SHOW_HELP || ret == DBG_CMD_SHOW_INPUT
		|| ret == DBG_CMD_SHOW_OUTPUT)
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
