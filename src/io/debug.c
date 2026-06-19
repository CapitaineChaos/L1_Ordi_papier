/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\              Entrée debug                    /================

Auteur : Sylvain Maitre     24002886

Date de création :              12/06/2026
Date de dernière modification : 13/06/2026

Fichier     : io/debug.c
Description : Saisie spéciale en mode débogueur

==============================================================================*/

#include "internal.h"
#include "dbg/display.h"
#include "dbg/render.h"
#include "dbg/resize.h"
#include "messages.h"
#include "pico.h"
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>

/**
 * @brief Affiche l'invite de saisie en mode débogueur
 * @param pico Le mini-ordinateur
 * @param hex La chaîne hexadécimale à afficher
 */
static void	afficher_invite_debug_entree(Mini_ordi *pico, const char *hex) {
	Dbg	*dbg;
	bool	redessiner;

	dbg = pico->dbg;
	if (!dbg_display_terminal_ok(dbg)) {
		dbg_display_trop_petit(dbg);
		return;
	}
	redessiner = dbg->terminal.petit_affiche;
	dbg->terminal.petit_affiche = false;
	render_set_line(dbg, DBG_PROMPT_LINE, LG_COLOR(FOND_FONCE, B_WH));
	if (pico->PC < 32)
		render_lg_col(dbg, DBG_PROMPT_LINE, 1, MODE_FONCE BLD MSG_INPUT_BOOTSTRAP RST);
	else
		render_lg_col(dbg, DBG_PROMPT_LINE, 1, MODE_FONCE BLD MSG_INPUT_PROGRAM RST);
	render_set_line(dbg, DBG_COMMAND_LINE, DBG_FOND_BK);
	render_lg_col(dbg, DBG_COMMAND_LINE, 1, DBG_INVITE(false));
	render_append_to_line(dbg, DBG_COMMAND_LINE, "%s", hex);
	if (redessiner)
		dbg_display_draw(dbg);
	else
		dbg_display_interaction(dbg);
}



/**
 * @brief Lit une entrée hexadécimale de l'utilisateur en mode débogueur
 * @param pico Le mini-ordinateur
 * @return L'octet hexadécimal lu
 */
static u8	lire_entree_debug_utilisateur(Mini_ordi *pico) {
	FILE	*stream;
	FILE	*tty;
	char	hex[3];
	size_t	len;
	int		c;
	int		echappement;
	u8		val;

	tty = dbg_get_tty(pico->dbg);
	if (!tty)
		return (DBG_CMD_STOP_DBG);

	stream = io_flux_entree_utilisateur(pico);
	hex[0] = '0';
	hex[1] = '0';
	hex[2] = '\0';
	len = 0;
	echappement = 0;
	// Initialiser la gestion du redimensionnement du terminal
	dbg_resize_init();
	dbg_display_raw_enter(pico->dbg);
	afficher_invite_debug_entree(pico, hex);
	while (true) {
		c = fgetc(stream);
		if (c == EOF) {
			if (ferror(stream) && errno == EINTR && dbg_resize_recu()) {
				clearerr(stream);
				dbg_resize_reset();
				if (dbg_resize_recu()) {
					afficher_invite_debug_entree(pico, hex);
					dbg_display_raw_enter(pico->dbg);
				}
				continue;
			}
			continue;
		}
		// Ctrl-C ou Ctrl-D pour quitter
		if (c == 3 || c == 4) {
			dbg_display_raw_leave(pico->dbg);
			dbg_display_leave(pico->dbg);
			exit(0);
		}
		// Gérer les séquences d'échappement (flèches ...)
		// pour ne pas les afficher dans la ligne de commande
		if (c == 27) {
			echappement = 1;
			continue;
		}
		// Gestion des bizarreries comme la molette ou autres
		// Si on est dans une séquence d'échappement, on ignore les caractères suivants jusqu'à la fin de la séquence
		if (echappement == 1) {
			echappement = (c == '[' || c == 'O') ? 2 : 0;
			continue;
		}
		// Si on est dans une séquence d'échappement, on ignore les caractères suivants jusqu'à la fin
		if (echappement == 2) {
			if (c >= '@' && c <= '~')
				echappement = 0;
			continue;
		}
		// Backspace pour effacer le dernier caractère entré
		if ((c == 127 || c == '\b') && len > 0) {
			hex[1] = hex[0];
			hex[0] = '0';
			len--;
			afficher_invite_debug_entree(pico, hex);
			continue;
		}
		// Entrée valide (hexadécimale) pour construire l'octet à saisir
		if ((c == '\n' || c == '\r') && len > 0 && len < 3)
			break;
		if (isxdigit((unsigned char)c) && len < 2) {
			hex[0] = hex[1];
			hex[1] = (char)toupper((unsigned char)c);
			len++;
			afficher_invite_debug_entree(pico, hex);
		}
	}
	// dbg_display_raw_leave(pico->dbg);
	io_parse_hex_byte(hex, &val);
	return (val);
}

/**
 * @brief Savegarder dans le status du débogueur la source de l'entrée utilisée
 * @param pico Le mini-ordinateur
 * @param is_stdin Si la provenance de l'entrée est stdin ou l'utilisateur
 */
static void	source_entree_status(Mini_ordi *pico, bool is_stdin) {
	if (!pico->modes.debogage)
		return;

	int sz = sizeof(pico->dbg->texte.lg_status);

	if (pico->PC < 32) {
		if (is_stdin)
			render_set_text(pico->dbg->texte.lg_status, sz, DBG_STATE_BOOTSTRAP_STDIN);
		else
			render_set_text(pico->dbg->texte.lg_status, sz, DBG_STATE_BOOTSTRAP_USER);
	}
	else {
		if (is_stdin)
			render_set_text(pico->dbg->texte.lg_status, sz, DBG_STATE_PROGRAM_STDIN);
		else
			render_set_text(pico->dbg->texte.lg_status, sz, DBG_STATE_PROGRAM_USER);
	}
	render_set_line(pico->dbg, DBG_STATE_LINE, DBG_STATE_FOND);
	render_lg_col(pico->dbg, DBG_STATE_LINE, 1, pico->dbg->texte.lg_status);
	dbg_display_interaction(pico->dbg);
}

/**
 * @brief Lit un octet en entrée en mode débogage
 * @param pico Le mini-ordinateur
 * @return L'octet lu
 */
u8	io_lire_entree_debug(Mini_ordi *pico) {
	u8		val = 0;
	bool	std_in = IO_STDIN_AVAILABLE;

	if (std_in)
		val = (unsigned char)pico->IO.buffer[pico->IO.buffer_pos++];
	else
		val = lire_entree_debug_utilisateur(pico);
		
	source_entree_status(pico, std_in);
	return (val);
}
