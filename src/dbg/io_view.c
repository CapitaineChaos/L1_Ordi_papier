/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\              Vues IO du debug                /================

Auteur : Sylvain Maitre     24002886

Date de création :              12/06/2026
Date de dernière modification : 12/06/2026

Fichier     : dbg/io_view.c
Description : Affichage des vues d'entrée et de sortie du débogueur

==============================================================================*/

#include "dbg/viewer.h"
#include "dbg/display.h"
#include "dbg/render.h"
#include "messages.h"
#include <ctype.h>

/**
 * @brief Affiche une ligne d'octets dans le buffer du débogueur
 * @param dbg Le débogueur
 * @param buffer Le buffer contenant les octets
 * @param len La longueur du buffer
 * @param pos La position de départ dans le buffer
 * @param line La ligne à afficher
 * @param adresse Indique si l'adresse doit être affichée
 */
static void	afficher_ligne_octets(Dbg *dbg, const char *buffer, size_t len, size_t pos, int line, bool adresse) {
	size_t	i;
	int		c;

	render_set_line(dbg, line, DBG_FOND_BK);
	if (adresse)
		render_lg_col(dbg, line, 3, STDIN_ADR(pos));
	i = 0;
	while (i < 16 && pos + i < len) {
		c = (unsigned char)buffer[pos + i];
		render_lg_col(dbg, line, 12 + i * 3, STDIN_OCTET(c));
		if (isprint(c))
			render_lg_col(dbg, line, 63 + i, STDIN_ASC(c));
		else
			render_lg_col(dbg, line, 63 + i, STDIN_NON_PRINT(c));
		i++;
	}
}

/**
 * @brief Helper - Affiche une ligne du buffer d'entrée du mini-ordinateur
 * @param dbg Le débogueur
 * @param pico Le mini-ordinateur
 * @param pos La position de départ dans le buffer
 * @param line La ligne à afficher
 */
static void	afficher_ligne_buffer(Dbg *dbg, Mini_ordi *pico, size_t pos, int line) {
	afficher_ligne_octets(dbg, pico->IO.buffer, pico->IO.buffer_len, pos, line, true);
}

/**
 * @brief Helper - Affiche une ligne du buffer de sortie du mini-ordinateur
 * @param dbg Le débogueur
 * @param pico Le mini-ordinateur
 * @param pos La position de départ dans le buffer
 * @param line La ligne à afficher
 */
static void	afficher_ligne_output(Dbg *dbg, Mini_ordi *pico, size_t pos, int line) {
	afficher_ligne_octets(dbg, pico->IO.output, pico->IO.output_len, pos, line, false);
}

/**
 * @brief Affiche le contenu de l'entrée standard du mini-ordinateur
 * @param pico Le mini-ordinateur
 * @param dbg Le débogueur
 */
void	viewer_stdin(Mini_ordi *pico, Dbg *dbg) {
	size_t	pos;
	int		line;

	if (!dbg_get_tty(dbg))
		return;
	if (!dbg_display_terminal_ok(dbg)) {
		dbg_display_trop_petit(dbg);
		return;
	}
	dbg->terminal.petit_affiche = false;
	dbg_render_clear(dbg);
	render_top_menu(pico, dbg);
	render_set_line(dbg, 2, DBG_FOND_WH);
	render_lg_col(dbg, 2, 3,
		MSG_STDIN_RESTANT(pico->IO.buffer_len, pico->IO.buffer_pos));
	render_lg_col(dbg, 2, 63, MSG_STDIN_ASC);
	pos = pico->IO.buffer_pos;
	line = 3;
	while (line < DBG_STATE_LINE && pos < pico->IO.buffer_len) {
		afficher_ligne_buffer(dbg, pico, pos, line);
		pos += 16;
		line++;
	}
	if (pos < pico->IO.buffer_len) {
		render_set_line(dbg, DBG_STATE_LINE - 1, DBG_FOND_WH);
		render_lg_col(dbg, DBG_STATE_LINE - 1, 3,
			MSG_STDIN_IGNORE(pico->IO.buffer_len, pos));
	}
	render_viewer_bottom(dbg, line);
	dbg_display_draw(dbg);
}

/**
 * @brief Affiche le contenu de la sortie standard du mini-ordinateur
 * @param pico Le mini-ordinateur
 * @param dbg Le débogueur
 */
void	viewer_output(Mini_ordi *pico, Dbg *dbg) {
	size_t	pos;
	int		line;

	if (!dbg_get_tty(dbg))
		return;
	if (!dbg_display_terminal_ok(dbg)) {
		dbg_display_trop_petit(dbg);
		return;
	}
	dbg->terminal.petit_affiche = false;
	dbg_render_clear(dbg);
	render_top_menu(pico, dbg);
	render_set_line(dbg, 2, DBG_FOND_WH);
	render_lg_col(dbg, 2, 3, MSG_OUTPUT_TOTAL(pico->IO.output_len));
	render_lg_col(dbg, 2, 63, MSG_STDIN_ASC);
	pos = 0;
	line = 3;
	while (line < DBG_STATE_LINE && pos < pico->IO.output_len) {
		afficher_ligne_output(dbg, pico, pos, line);
		pos += 16;
		line++;
	}
	if (pos < pico->IO.output_len) {
		render_set_line(dbg, DBG_STATE_LINE - 1, DBG_FOND_WH);
		render_lg_col(dbg, DBG_STATE_LINE - 1, 3,
			MSG_OUTPUT_IGNORE(pico->IO.output_len, pos));
	}
	render_viewer_bottom(dbg, line);
	dbg_display_draw(dbg);
}
