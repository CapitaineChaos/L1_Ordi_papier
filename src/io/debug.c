/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\                 Entrée debug                 /================

Auteur : Sylvain Maitre     24002886

Date de création :              12/06/2026
Date de dernière modification : 20/06/2026

Fichier     : io/debug.c
Description : Saisie spéciale en mode débogueur

==============================================================================*/

#include "internal.h"
#include "dbg/compositeur.h"
#include "dbg/display.h"
#include "dbg/input.h"
#include "dbg/render.h"
#include "messages.h"
#include "pico.h"
#include <ctype.h>

/**
 * @brief Affiche l'invite de saisie en mode débogueur
 * @param pico Le mini-ordinateur
 * @param hex La chaîne hexadécimale à afficher
 */
static void	afficher_invite_debug_entree(Mini_ordi *pico, const char *hex) {
	Dbg	*dbg = pico->dbg;

	if (!compositeur_overlay_ok(dbg))
		return;
	compositeur_invite_entree(pico, dbg, hex);
	// Le viewer différentiel gère aussi le retour depuis l'état « trop petit »
	// (init_screen réinitialise le buffer précédent et force un redessin complet)
	dbg_display_draw(dbg);
}



/**
 * @brief Lit une entrée hexadécimale de l'utilisateur en mode débogueur
 * @param pico Le mini-ordinateur
 * @return L'octet hexadécimal lu
 */
static u8	lire_entree_debug_utilisateur(Mini_ordi *pico) {
	Dbg		*dbg = pico->dbg;
	FILE	*stream;
	char	hex[3];
	size_t	len;
	int		c;
	u8		val;

	if (!dbg_get_tty(dbg))
		return (0);
	stream = io_flux_entree_utilisateur(pico);
	hex[0] = '0';
	hex[1] = '0';
	hex[2] = '\0';
	len = 0;
	dbg_input_debut(dbg);
	afficher_invite_debug_entree(pico, hex);
	while (true) {
		c = dbg_input_touche(dbg, stream);
		if (c == DBG_KEY_EOF)
			break;
		if (c == DBG_KEY_RESIZE) {
			afficher_invite_debug_entree(pico, hex);
			continue;
		}
		// Backspace pour effacer le dernier chiffre entré
		if (c == DBG_KEY_BACKSPACE) {
			if (len > 0) {
				hex[1] = hex[0];
				hex[0] = '0';
				len--;
				afficher_invite_debug_entree(pico, hex);
			}
			continue;
		}
		// Entrée valide (au moins un chiffre saisi) pour construire l'octet
		if (c == DBG_KEY_ENTER) {
			if (len > 0)
				break;
			continue;
		}
		if (isxdigit(c) && len < 2) {
			hex[0] = hex[1];
			hex[1] = (char)toupper(c);
			len++;
			afficher_invite_debug_entree(pico, hex);
		}
	}
	dbg_input_fin(dbg);
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
	dbg_display_draw(pico->dbg);
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
