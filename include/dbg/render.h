/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\              Rendu des buffers debug         /================

Auteur : Sylvain Maitre     24002886

Date de création :              11/06/2026
Date de dernière modification : 12/06/2026

Fichier     : dbg/render.h
Description : Déclarations du rendu des buffers du débogueur

==============================================================================*/

#ifndef DBG_RENDER_H
# define DBG_RENDER_H

# include "dbg.h"

void	dbg_render_clear(Dbg *dbg);
void	render_set_text(char *buffer, size_t size, const char *fmt, ...);
void	render_set_line(Dbg *dbg, int line, const char *fmt, ...);
void	render_top_menu(Mini_ordi *pico, Dbg *dbg);
void	render_fill_lines(Dbg *dbg, int from, int to, const char *fmt);
void	render_viewer_bottom(Dbg *dbg, int line);
void	render_append_to_line(Dbg *dbg, int line, const char *fmt, ...);
void	render_lg_col(Dbg *dbg, int line, size_t col, const char *fmt, ...);

#endif
