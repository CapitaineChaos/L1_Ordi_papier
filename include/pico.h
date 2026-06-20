/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\               Header principal               /================

Auteur : Sylvain Maitre     24002886

Date de création :              01/10/2025
Date de dernière modification : 20/06/2026

Fichier     : pico.h
Description : Définitions principales du mini-ordinateur

==============================================================================*/

#ifndef PICO_H
#define PICO_H

#include "pico_types.h"
#include <stdbool.h>
#include <stddef.h>

#define PICO_INPUT_BUFFER_SIZE (1024 * 1024)
#define PICO_INPUT_MAX_BYTES (255 - 32 + 2)

struct s_dbg;
typedef int					(*t_microfonction)(Mini_ordi *pico);

enum opcodes_e {
	OP_ADD_IMM = 0x20,
	OP_ADD_ABS = 0x60,
	OP_ADD_IND = 0xE0,
	OP_SUB_IMM = 0x21,
	OP_SUB_ABS = 0x61,
	OP_SUB_IND = 0xE1,
	OP_NAND_IMM = 0x22,
	OP_NAND_ABS = 0x62,
	OP_NAND_IND = 0xE2,
	OP_LOAD_IMM = 0x00,
	OP_LOAD_ABS = 0x40,
	OP_LOAD_IND = 0xC0,
	OP_STORE_ABS = 0x48,
	OP_STORE_IND = 0xC8,
	OP_IN_ABS = 0x49,
	OP_IN_IND = 0xC9,
	OP_OUT_ABS = 0x41,
	OP_OUT_IND = 0xC1,
	OP_JUMP_ABS = 0x10,
	OP_BRN_ABS = 0x11,
	OP_BRZ_ABS = 0x12
};

typedef struct	s_io {
	u8		in;
	u8		out;
	char	buffer[PICO_INPUT_BUFFER_SIZE];
	size_t	buffer_len;
	size_t	buffer_pos;
	char	output[PICO_INPUT_BUFFER_SIZE];
	size_t	output_len;
	const char	*input_file;
	bool	stdin_is_tty;
}	io;

#define IO_STDIN_EMPTY (pico->IO.buffer_pos >= pico->IO.buffer_len)
#define IO_STDIN_AVAILABLE (!IO_STDIN_EMPTY)
#define IO_OUTPUT_AVAILABLE (pico->IO.output_len > 0)

/** Modes d'exécution */
struct	s_modes {
	/** Mode debug */
	bool	debogage;
	/** Ajouter les messages bootstrap */
	bool	bootstrap;
	/** Mode verbeux */
	bool	verbeux;
	/** Représentation hexadécimale de la saisie clavier et de la sortie
	 *  non-debug (sinon décimal par défaut) */
	bool	mode_hexa;
};

typedef bool	(*t_condition)(Mini_ordi *pico);

typedef struct s_microsequence {
	t_condition			cond;
	int					nb_microcodes;
	int					sequence[10];
}	t_mcseq;

extern t_microfonction	g_microfonctions[18];

/**
 * @brief Structure représentant le mini-ordinateur et ses composants et états.
 */
struct	s_mini_ordi {
	/** Mémoire de 256 octets */
	u8		mem[256];
	/** Registre A */
	s8		A;
	/** Compteur ordinal */
	u8		PC;
	/** Registre de sélection */
	u8		RS;
	/** Registre mot */
	u8		RM;
	/** Registre adresse */
	u8		AD;
	/** Registre code operation */
	u8		OP;
	/** Microsequences pour chaque instruction */
	t_mcseq	 microsequences[256];
	/** fonction UAL */
	u8		(*UAL)(u8, u8);
	/** Entrée/Sortie */
	io		IO;
	/** Modes d'exécution */
	Modes	modes;
	/** Adresse de début du programme */
	u8		prog_debut;
	/** Taille du programme */
	u8		prog_taille;
	/** Paramètres de programme fournis par option */
	bool	prog_params;
	/** Lien vers le débogueur */
	struct s_dbg	*dbg;
};

#endif
