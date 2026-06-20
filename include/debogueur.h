/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\               Header du débogueur            /================

Auteur : Sylvain Maitre     24002886

Date de création :              06/06/2026
Date de dernière modification : 20/06/2026

Fichier     : debogueur.h
Description : Définitions du débogueur

==============================================================================*/

#ifndef DEBOGUEUR_H
# define DEBOGUEUR_H

#include "pico.h"
#include <stdio.h>
#include <termios.h>

# define DBG_SCREEN_LINES 27
# define DBG_LINE_SIZE 2048
# define DBG_UAL_SIZE 32

/* Lignes de l'écran du débogueur (indices 0-based dans le buffer) */
# define DBG_MENU_LINE 0
# define DBG_TITLE_LINE 1
# define DBG_HEADER_LINE 2
/* lignes 3..18 : mémoire (cf. MEM_LINE_FIRST dans viewer.c) */
# define DBG_PHASE_LINE 19
# define DBG_MICRO_LINE 20
# define DBG_LMICRO_LINE 21
# define DBG_INSTR_LINE 22
# define DBG_MODE_LINE 23
# define DBG_PROMPT_LINE (DBG_SCREEN_LINES - 3)
# define DBG_COMMAND_LINE (DBG_SCREEN_LINES - 2)
# define DBG_STATE_LINE (DBG_SCREEN_LINES - 1)

enum dbg_default_e {
	DBG_DEF_MICROCODE,
	DBG_DEF_PHASE,
	DBG_DEF_INSTRUCTION,
};


enum dbg_command_e {
	DBG_CMD_NONE,
	DBG_CMD_NEXT_MICROCODE,
	DBG_CMD_NEXT_PHASE,
	DBG_CMD_NEXT_INSTRUCTION,
	DBG_CMD_QUIT,
	DBG_CMD_STOP_DBG,
	DBG_CMD_HELP,
	DBG_CMD_AUTO,
	DBG_CMD_EXEC_MICROCODE,
	DBG_CMD_EXEC_INSTRUCTION,
	DBG_CMD_JUMP,
	DBG_CMD_SHOW_HELP,
	DBG_CMD_SHOW_INPUT,
	DBG_CMD_SHOW_OUTPUT,
	DBG_CMD_REFRESH,
	DBG_CMD_UNKNOWN,
};

typedef enum dbg_command_e dbg_cmd;
typedef enum dbg_default_e dbg_def;

typedef struct	s_dbg_exec {
	u8		microcode_precedent;
	u8		microcode_a_executer;
	dbg_def	mode_defaut;
	bool	next_instruction;
	bool	next_phase;
	bool	next_microcode;
	bool	restart_cycle;
}	Dbg_exec;

typedef struct	s_dbg_text {
	char	instruction[52];
	char	microcode[52];
	char	operation_ual[DBG_UAL_SIZE];
	char	lg_status[DBG_LINE_SIZE];
	char	lg_mode[DBG_LINE_SIZE];
	char	lg_cmd[DBG_LINE_SIZE];
}	Dbg_text;

typedef struct	s_dbg_terminal {
	FILE			*tty;
	bool			viewer_actif;
	bool			raw_actif;
	struct termios	orig;
	bool			petit_affiche;
}	Dbg_terminal;

typedef struct	s_dbg_screen {
	char	precedent[DBG_SCREEN_LINES][DBG_LINE_SIZE];
	char	courant[DBG_SCREEN_LINES][DBG_LINE_SIZE];
	bool	initialise;
}	Dbg_screen;

struct	s_dbg {
	Dbg_exec		exec;
	Dbg_text		texte;
	Dbg_terminal	terminal;
	Dbg_screen		screen;
	int				erreur_prog;
};


void	exec_debogueur(Mini_ordi *pico, Dbg *dbg, int phase, t_mcseq *mseq, int pos);

# endif
