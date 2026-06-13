/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\          Affichage différentiel debug        /================

Auteur : Sylvain Maitre     24002886

Date de création :              11/06/2026
Date de dernière modification : 13/06/2026

Fichier     : dbg/display.c
Description : Display différentiel 80x27 du débogueur

==============================================================================*/

#include "dbg/display.h"
#include "messages.h"
#include <stdarg.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

/**
 * @brief Récupère le fichier de terminal du débogueur
 * @param dbg Le débogueur
 * @return Le fichier de terminal
 */
FILE	*dbg_get_tty(Dbg *dbg) {
	if (!dbg->terminal.tty)
		dbg->terminal.tty = fopen("/dev/tty", "r+");
	return (dbg->terminal.tty);
}

/**
 * @brief Initialise l'écran du débogueur
 * @param dbg Le débogueur
 */
static void	init_screen(Dbg *dbg) {
	if (!dbg->screen.initialise) {
		fprintf(dbg->terminal.tty, D_HIDE_CURSOR D_HOME E_SCREEN_ALL);
		memset(dbg->screen.precedent, 0, sizeof(dbg->screen.precedent));
		dbg->screen.initialise = true;
	}
}

/**
 * @brief Passe le terminal en mode d'affichage du débogueur
 * @param dbg Le débogueur
 */
void	dbg_display_enter(Dbg *dbg) {
	if (!dbg->terminal.tty || dbg->terminal.viewer_actif)
		return;
	fprintf(dbg->terminal.tty, D_ALT_SCREEN D_NO_WRAP D_HIDE_CURSOR D_HOME E_SCREEN_ALL);
	fflush(dbg->terminal.tty);
	dbg->terminal.viewer_actif = true;
	dbg->screen.initialise = false;
}

/**
 * @brief Quitte le mode d'affichage du débogueur
 * @param dbg Le débogueur
 */
void	dbg_display_leave(Dbg *dbg) {
	if (!dbg->terminal.tty)
		return;
	dbg_display_raw_leave(dbg);
	fprintf(dbg->terminal.tty, RST D_WRAP D_SHOW_CURSOR);
	if (dbg->terminal.viewer_actif)
		fprintf(dbg->terminal.tty, D_MAIN_SCREEN);
	fprintf(dbg->terminal.tty, E_LINE_ALL "\n");
	fflush(dbg->terminal.tty);
	dbg->terminal.viewer_actif = false;
	dbg->screen.initialise = false;
}

/**
 * @brief Passe le terminal en mode non canonique
 * @param dbg Le débogueur
 * @note Chaque caractère devient lisible immédiatement, sans attendre Entrée
 * @note L'écho des caractères est désactivé
 * @note Les caractères spéciaux comme Ctrl+C ne génèrent plus de signal
 * et sont transmis au programme comme des octets
 * @note Les touches de fonction et les flèches sont reçues sous forme
 * de séquences d'échappement
 */
void	dbg_display_raw_enter(Dbg *dbg) {
	struct termios	raw;

	if (!dbg->terminal.tty || dbg->terminal.raw_actif)
		return;
	if (tcgetattr(fileno(dbg->terminal.tty), &dbg->terminal.orig) == -1)
		return;
	raw = dbg->terminal.orig;
	raw.c_lflag &= ~(ICANON | ECHO | ISIG);
	raw.c_cc[VMIN] = 1;
	raw.c_cc[VTIME] = 0;
	if (tcsetattr(fileno(dbg->terminal.tty), TCSANOW, &raw) == -1)
		return;
	dbg->terminal.raw_actif = true;
}

/**
 * @brief Quitte le mode raw du terminal
 * @param dbg Le débogueur
 */
void	dbg_display_raw_leave(Dbg *dbg) {
	if (!dbg->terminal.tty || !dbg->terminal.raw_actif)
		return;
	tcsetattr(fileno(dbg->terminal.tty), TCSANOW, &dbg->terminal.orig);
	dbg->terminal.raw_actif = false;
}

/**
 * @brief Vérifie si le terminal est suffisamment grand pour afficher le débogueur
 * @param dbg Le débogueur
 * @return true si le terminal est suffisamment grand, false sinon
 */
bool	dbg_display_terminal_ok(Dbg *dbg) {
	struct winsize	ws;
	FILE			*tty;

	tty = dbg_get_tty(dbg);
	if (!tty)
		return (false);
	if (ioctl(fileno(tty), TIOCGWINSZ, &ws) == -1)
		return (false);
	return (ws.ws_col >= 80 && ws.ws_row >= DBG_SCREEN_LINES);
}

/**
 * @brief Affiche une ligne spécifique de l'écran du débogueur
 * @param dbg Le débogueur
 * @param tty Le flux de sortie du terminal
 * @param line La ligne à afficher
 * @param content Le contenu à afficher
 */
static void	draw_line(Dbg *dbg, FILE *tty, int line, const char *content) {
	if (strcmp(content, dbg->screen.precedent[line]) == 0)
		return;
	fprintf(tty, RST D_LINE_START_FMT E_LINE_ALL "%s" RST,
		line + 1, content);
	strncpy(dbg->screen.precedent[line], content, DBG_LINE_SIZE - 1);
	dbg->screen.precedent[line][DBG_LINE_SIZE - 1] = '\0';
}

/**
 * @brief Affiche une ligne spécifique de l'écran du débogueur
 * @param dbg Le débogueur
 * @param line La ligne à afficher
 */
void	dbg_display_draw_line(Dbg *dbg, int line) {
	FILE	*tty;

	if (line < 0 || line >= DBG_SCREEN_LINES)
		return;
	tty = dbg_get_tty(dbg);
	if (!tty)
		return;
	dbg_display_enter(dbg);
	init_screen(dbg);
	draw_line(dbg, tty, line, dbg->screen.courant[line]);
	fflush(tty);
}


/**
 * @brief Affiche l'ensemble de l'écran du débogueur
 * @param dbg Le débogueur
 * @note Affiche l'ensemble de l'écran du débogueur en ne redessinant
 * que les lignes qui ont changé depuis le dernier affichage pour éviter le scintillement
 * et les rafraichissements inutiles lors des mouvements de sélection
 */
void	dbg_display_draw(Dbg *dbg) {
	FILE	*tty;

	tty = dbg_get_tty(dbg);
	if (!tty)
		return;
	dbg_display_enter(dbg);
	init_screen(dbg);
	for (int i = 0; i < DBG_SCREEN_LINES; i++)
		draw_line(dbg, tty, i, dbg->screen.courant[i]);
	fflush(tty);
}

/**
 * @brief Affiche l'interaction du débogueur
 * @param dbg Le débogueur
 * @note Ligne de status : affichage divers
 * @note Ligne de prompt : commanndes ou demande de saisies
 * @note Ligne de saisie de commande : interaction utilisateur
 */
void	dbg_display_interaction(Dbg *dbg) {
	FILE	*tty;

	tty = dbg_get_tty(dbg);
	if (!tty)
		return;
	dbg_display_enter(dbg);
	init_screen(dbg);
	draw_line(dbg, tty, DBG_COMMAND_LINE, dbg->screen.courant[DBG_COMMAND_LINE]);
	draw_line(dbg, tty, DBG_PROMPT_LINE, dbg->screen.courant[DBG_PROMPT_LINE]);
	draw_line(dbg, tty, DBG_STATE_LINE, dbg->screen.courant[DBG_STATE_LINE]);
	fflush(tty);
}

/**
 * @brief Affiche un message indiquant que le terminal est trop petit
 * @param dbg Le débogueur
 */
void	dbg_display_trop_petit(Dbg *dbg) {
	FILE	*tty;

	tty = dbg_get_tty(dbg);
	if (!tty)
		return;
	if (dbg->terminal.petit_affiche)
		return;
	dbg_display_leave(dbg);
	fprintf(tty, RST D_WRAP D_SHOW_CURSOR D_HOME E_SCREEN_ALL "%s\n",
		DBG_STATE_SMALL_TERM);
	dbg->terminal.petit_affiche = true;
	fflush(tty);
	dbg->screen.initialise = false;
}
