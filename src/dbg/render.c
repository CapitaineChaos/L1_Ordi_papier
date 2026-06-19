/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\           Rendu des buffers debug            /================

Auteur : Sylvain Maitre     24002886

Date de création :              11/06/2026
Date de dernière modification : 20/06/2026

Fichier     : dbg/render.c
Description : Rendu des buffers du débogueur

Alternative : La prochaine fois ce sera avec ncurses, trop de problèmes à gérer
Remarque    : Le rendu différentiel est par ligne, et non par caractère
              À cause du formattage ANSI et des couleurs, trop de complexité.

==============================================================================*/

#include "dbg/render.h"
#include "messages.h"
#include <stdarg.h>
#include <string.h>

void	dbg_render_clear(Dbg *dbg) {
	memset(dbg->screen.courant, 0, sizeof(dbg->screen.courant));
}

/**
 * @brief Formatte une chaîne dans un buffer
 * @param buffer Le buffer de destination
 * @param fmt La chaîne de formatage
 * @param ... Les arguments de formatage
 * @note La chaîne est tronquée à la première occurrence d'un saut de ligne
 * @param size La taille du buffer de destination
 * @note S'emploie surtout avec dbg->texte.quelquechose
 */
void	render_set_text(char *buffer, size_t size, const char *fmt, ...) {
	va_list	args;

	if (!buffer || size == 0)
		return;
	va_start(args, fmt);
	vsnprintf(buffer, size, fmt, args);
	va_end(args);
	buffer[strcspn(buffer, "\n")] = '\0';
}

/**
 * @brief Formatte une ligne dans le buffer de l'écran
 * @param dbg Le débogueur
 * @param line Le numéro de la ligne à formater
 * @param fmt La chaîne de formatage
 * @param ... Les arguments de formatage
 * @note La chaîne est tronquée à la première occurrence d'un saut de ligne
 */
void	render_set_line(Dbg *dbg, int line, const char *fmt, ...) {
	va_list	args;

	if (line < 0 || line >= DBG_SCREEN_LINES)
		return;
	va_start(args, fmt);
	vsnprintf(dbg->screen.courant[line], DBG_LINE_SIZE, fmt, args);
	va_end(args);
	dbg->screen.courant[line][strcspn(dbg->screen.courant[line], "\n")] = '\0';
}

/**
 * @brief Remplit un intervalle de lignes de l'écran du débogueur avec une chaîne de formatage
 * @param dbg Le débogueur
 * @param from Le numéro de la ligne de départ
 * @param to Le numéro de la ligne de fin (exclus)
 * @param fmt La chaîne de formatage
 */
void	render_fill_lines(Dbg *dbg, int from, int to, const char *fmt) {
	while (from < to) {
		render_set_line(dbg, from, fmt);
		from++;
	}
}

/**
 * @brief Formatte une ligne dans le buffer de l'écran du débogueur avec des arguments variadiques
 * @param dbg Le débogueur
 * @param line Le numéro de la ligne à formater
 * @param col La colonne de départ pour l'affichage (-1 pour append)
 * @param fmt La chaîne de formatage
 * @param args Les arguments de formatage
 * @note La chaîne est tronquée à la première occurrence d'un saut de ligne
 */
static void	add_line_args(Dbg *dbg, int line, ssize_t col, const char *fmt, va_list args) {
	size_t	len;
	char	tmp[DBG_LINE_SIZE];

	if (line < 0 || line >= DBG_SCREEN_LINES)
		return;
	vsnprintf(tmp, DBG_LINE_SIZE, fmt, args);
	tmp[strcspn(tmp, "\n")] = '\0';
	len = strlen(dbg->screen.courant[line]);
	if (col >= 0)
		snprintf(dbg->screen.courant[line] + len, DBG_LINE_SIZE - len,
			D_COL_FMT "%s", (size_t)col, tmp);
	else
		snprintf(dbg->screen.courant[line] + len, DBG_LINE_SIZE - len, "%s", tmp);
}

/**
 * @brief Ajoute du texte à une ligne existante dans le buffer de l'écran du débogueur
 * @param dbg Le débogueur
 * @param line Le numéro de la ligne à modifier
 * @param fmt La chaîne de formatage
 * @param ... Les arguments de formatage
 * @note La chaîne est tronquée à la première occurrence d'un saut de ligne
 * par héritage de add_line_args
 */
void	render_append_to_line(Dbg *dbg, int line, const char *fmt, ...) {
	va_list	args;

	va_start(args, fmt);
	add_line_args(dbg, line, -1, fmt, args);
	va_end(args);
}

/**
 * @brief Formatte une ligne dans le buffer de l'écran à une colonne spécifique
 * @param dbg Le débogueur
 * @param line Le numéro de la ligne à formater
 * @param col La colonne de départ pour l'affichage (-1 pour append)
 * @param fmt La chaîne de formatage
 * @param ... Les arguments de formatage
 * @note La chaîne est tronquée à la première occurrence d'un saut de ligne
 * par héritage de add_line_args
 */
void	render_lg_col(Dbg *dbg, int line, size_t col, const char *fmt, ...) {
	va_list	args;

	va_start(args, fmt);
	add_line_args(dbg, line, (ssize_t)col, fmt, args);
	va_end(args);
}
