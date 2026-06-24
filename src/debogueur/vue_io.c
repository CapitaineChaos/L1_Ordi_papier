/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\              Vues IO du debug                /================

Auteur : Sylvain Maitre     24002886

Date de création :              12/06/2026
Date de dernière modification : 22/06/2026

Fichier     : debogueur/vue_io.c
Description : Affichage des vues d'entrée et de sortie du débogueur

==============================================================================*/

#include "debogueur/vue_io.h"
#include "debogueur/compositeur.h"
#include "debogueur/rendu.h"
#include "debogueur/libelles.h"
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
 * @brief Compose le contenu de l'entrée standard du mini-ordinateur dans le buffer
 * @param pico Le mini-ordinateur
 * @param dbg Le débogueur
 * @return true si l'écran a été composé, false si le terminal n'est pas affichable
 */
bool	compositeur_stdin(Mini_ordi *pico, Dbg *dbg) {
	size_t	pos;
	int		line;

	if (!compositeur_debut(dbg))
		return (false);
	compositeur_top_menu(pico, dbg);
	render_set_line(dbg, DBG_HEADER_LINE, DBG_FOND_WH);
	render_lg_col(dbg, DBG_HEADER_LINE, 3,
		MSG_STDIN_RESTANT(pico->IO.buffer_len, pico->IO.buffer_pos));
	render_lg_col(dbg, DBG_HEADER_LINE, 63, MSG_STDIN_ASC);
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
	compositeur_bottom(dbg, line);
	return (true);
}

/**
 * @brief Compose le contenu de la sortie standard du mini-ordinateur dans le buffer
 * @param pico Le mini-ordinateur
 * @param dbg Le débogueur
 * @return true si l'écran a été composé, false si le terminal n'est pas affichable
 */
bool	compositeur_output(Mini_ordi *pico, Dbg *dbg) {
	size_t	pos;
	int		line;

	if (!compositeur_debut(dbg))
		return (false);
	compositeur_top_menu(pico, dbg);
	render_set_line(dbg, DBG_HEADER_LINE, DBG_FOND_WH);
	render_lg_col(dbg, DBG_HEADER_LINE, 3, MSG_OUTPUT_TOTAL(pico->IO.output_len));
	render_lg_col(dbg, DBG_HEADER_LINE, 63, MSG_STDIN_ASC);
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
	compositeur_bottom(dbg, line);
	return (true);
}
