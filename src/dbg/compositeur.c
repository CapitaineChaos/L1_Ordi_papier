/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\         Composition des écrans debug         /================

Auteur : Sylvain Maitre     24002886

Date de création :              20/06/2026
Date de dernière modification : 20/06/2026

Fichier     : dbg/compositeur.c
Description : Composition partagée du débogueur

La composition remplit le buffer écran (dbg->screen.courant) via les primitives
de render.c. Elle ne dessine jamais dans le terminal : l'affichage est délégué
à la vue différentielle (dbg_display_draw) par l'appelant.

==============================================================================*/

#include "dbg/compositeur.h"
#include "dbg/display.h"
#include "dbg/render.h"
#include "messages.h"

/**
 * @brief Vérifie que le terminal peut accueillir un overlay d'interaction
 * @param dbg Le débogueur
 * @return true si le terminal est affichable, false sinon
 * @note Si le terminal est trop petit, bascule sur l'affichage minimaliste et
 * renvoie false. Ne vide pas le buffer : un overlay se superpose à l'écran existant.
 */
bool	compositeur_overlay_ok(Dbg *dbg) {
	if (!dbg_get_tty(dbg))
		return (false);
	if (!dbg_display_terminal_ok(dbg)) {
		dbg_display_trop_petit(dbg);
		return (false);
	}
	dbg->terminal.petit_affiche = false;
	return (true);
}

/**
 * @brief Prépare le buffer écran pour une nouvelle composition plein écran
 * @param dbg Le débogueur
 * @return true si l'écran est composable, false si le terminal n'est pas affichable
 * @note Vide le buffer pour une composition propre.
 */
bool	compositeur_debut(Dbg *dbg) {
	if (!compositeur_overlay_ok(dbg))
		return (false);
	dbg_render_clear(dbg);
	return (true);
}

/**
 * @brief Compose le menu supérieur de l'écran du débogueur
 * @param pico Le mini-ordinateur
 * @param dbg Le débogueur
 */
void	compositeur_top_menu(Mini_ordi *pico, Dbg *dbg) {
	render_set_line(dbg, 0, DBG_FOND_BK);
	render_lg_col(dbg, 0, 4, DBG_TOP_MENU(IO_STDIN_AVAILABLE,
			IO_OUTPUT_AVAILABLE));
	render_set_line(dbg, 1, DBG_TOP_TITRE);
}

/**
 * @brief Compose le bas de l'écran du débogueur
 * @param dbg Le débogueur
 * @param line Le numéro de la ligne de départ pour le bas de l'écran
 */
void	compositeur_bottom(Dbg *dbg, int line) {
	render_fill_lines(dbg, line, DBG_STATE_LINE, DBG_FOND_BK);
	render_set_line(dbg, DBG_STATE_LINE, MSG_TOUCHE);
	render_set_line(dbg, DBG_PROMPT_LINE, DBG_FOND_BK);
	render_set_line(dbg, DBG_COMMAND_LINE, DBG_FOND_BK);
}

/**
 * @brief Compose la ligne de status d'attente de commande
 * @param pico Le mini-ordinateur
 * @param dbg Le débogueur
 * @param err Si la commande précédente était inconnue
 * @note Affiche une erreur du programme en cours le cas échéant, sinon le status temporaire
 */
static void	compositeur_status_attente(Mini_ordi *pico, Dbg *dbg, bool err) {
	size_t	sz = sizeof(dbg->texte.lg_status);

	if (dbg->erreur_prog) {
		if (dbg->erreur_prog == 3)
			render_set_text(dbg->texte.lg_status, sz, MSG_ERR_MEMORY_D, pico->RS);
		else if (dbg->erreur_prog == 4)
			render_set_text(dbg->texte.lg_status, sz, MSG_ERR_INSTRUCTION_D, pico->OP);
		else if (dbg->erreur_prog == 5)
			render_set_text(dbg->texte.lg_status, sz, MSG_ERR_CORE_D);
		else
			render_set_text(dbg->texte.lg_status, sz, MSG_ERR_INCONNUE_D, dbg->erreur_prog);
		render_set_line(dbg, DBG_STATE_LINE, DBG_FOND_RD);
		render_lg_col(dbg, DBG_STATE_LINE, 17, dbg->texte.lg_status);
	}
	else if (err)
		render_lg_col(dbg, DBG_STATE_LINE, 1, DBG_STATE_UNKNOWN_CMD);
	else if (dbg->texte.lg_status[0] != '\0')
		render_lg_col(dbg, DBG_STATE_LINE, 1, dbg->texte.lg_status);
}

/**
 * @brief Compose les 3 lignes de fin pour une attente de commande
 * @param pico Le mini-ordinateur
 * @param dbg Le débogueur
 * @param err Si la ligne de status doit signaler une commande inconnue
 * @param str La commande en cours de saisie, affichée après l'invite
 */
void	compositeur_attente(Mini_ordi *pico, Dbg *dbg, bool err, const char *str) {
	render_set_line(dbg, DBG_STATE_LINE, DBG_STATE_FOND);
	render_set_line(dbg, DBG_PROMPT_LINE, DBG_FOND_BK);
	render_set_line(dbg, DBG_COMMAND_LINE, DBG_FOND_BK);
	compositeur_status_attente(pico, dbg, err);
	render_lg_col(dbg, DBG_PROMPT_LINE, 0, DBG_CMD_LIST);
	render_lg_col(dbg, DBG_COMMAND_LINE, 0, DBG_INVITE(err));
	render_append_to_line(dbg, DBG_COMMAND_LINE, "%s", str ? str : "");
}

/**
 * @brief Compose l'invite de saisie hexadécimale en mode débogueur
 * @param pico Le mini-ordinateur
 * @param dbg Le débogueur
 * @param hex La chaîne hexadécimale en cours de saisie
 */
void	compositeur_invite_entree(Mini_ordi *pico, Dbg *dbg, const char *hex) {
	render_set_line(dbg, DBG_PROMPT_LINE, LG_COLOR(FOND_FONCE, B_WH));
	if (pico->PC < 32)
		render_lg_col(dbg, DBG_PROMPT_LINE, 1, MODE_FONCE BLD MSG_INPUT_BOOTSTRAP RST);
	else
		render_lg_col(dbg, DBG_PROMPT_LINE, 1, MODE_FONCE BLD MSG_INPUT_PROGRAM RST);
	render_set_line(dbg, DBG_COMMAND_LINE, DBG_FOND_BK);
	render_lg_col(dbg, DBG_COMMAND_LINE, 1, DBG_INVITE(false));
	render_append_to_line(dbg, DBG_COMMAND_LINE, "%s", hex);
}
