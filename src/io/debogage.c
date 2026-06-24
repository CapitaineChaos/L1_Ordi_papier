/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\                 Entrée debug                 /================

Auteur : Sylvain Maitre     24002886

Date de création :              12/06/2026
Date de dernière modification : 22/06/2026

Fichier     : io/debogage.c
Description : Saisie spéciale en mode débogueur

==============================================================================*/

#include "interne.h"
#include "debogueur/compositeur.h"
#include "debogueur/affichage.h"
#include "debogueur/saisie.h"
#include "debogueur/rendu.h"
#include "debogueur/libelles.h"
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
	dbg_display_draw(dbg);
}



/**
 * @brief Indique si un caractère est un chiffre valide selon le mode de saisie
 * @param c Le caractère
 * @param hexa true si la saisie est hexadécimale, false si décimale
 * @return true si le caractère est accepté
 */
static bool	chiffre_valide(int c, bool hexa) {
	return (hexa ? isxdigit(c) : isdigit(c));
}

/**
 * @brief Lit une entrée saisie au clavier en mode débogueur
 * @param pico Le mini-ordinateur
 * @return L'octet lu (saisie décimale par défaut, hexadécimale si -x)
 */
static u8	lire_entree_debug_utilisateur(Mini_ordi *pico) {
	Dbg		*dbg = pico->dbg;
	bool	hexa = pico->modes.mode_hexa;
	size_t	max = hexa ? 2 : 3;
	FILE	*stream;
	char	saisie[4];
	size_t	len;
	int		c;
	u8		val = 0;

	if (!dbg_get_tty(dbg))
		return (0);
	stream = io_flux_entree_utilisateur(pico);
	saisie[0] = '\0';
	len = 0;
	dbg_input_debut(dbg);
	afficher_invite_debug_entree(pico, saisie);
	while (true) {
		c = dbg_input_touche(dbg, stream);
		if (c == DBG_KEY_EOF)
			break;
		if (c == DBG_KEY_RESIZE) {
			afficher_invite_debug_entree(pico, saisie);
			continue;
		}
		// Backspace pour effacer le dernier chiffre entré
		if (c == DBG_KEY_BACKSPACE) {
			if (len > 0) {
				saisie[--len] = '\0';
				afficher_invite_debug_entree(pico, saisie);
			}
			continue;
		}
		// Entrée valide (au moins un chiffre saisi) pour construire l'octet
		if (c == DBG_KEY_ENTER) {
			if (len == 0)
				continue;
			if (hexa) {
				if (len == 1) {
					saisie[1] = saisie[0];
					saisie[0] = '0';
					saisie[2] = '\0';
				}
				break;
			}
			// En décimal, on refuse une valeur dépassant 255
			if (io_parse_dec_byte(saisie, &val))
				break;
			continue;
		}
		if (chiffre_valide(c, hexa) && len < max) {
			saisie[len++] = hexa ? (char)toupper(c) : (char)c;
			saisie[len] = '\0';
			afficher_invite_debug_entree(pico, saisie);
		}
	}
	dbg_input_fin(dbg);
	if (hexa)
		io_parse_hex_byte(saisie, &val);
	else
		io_parse_dec_byte(saisie, &val);
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
