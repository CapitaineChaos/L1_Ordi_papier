/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\            Types de retour de Pico           /================

Auteur : Sylvain Maitre     24002886

Date de création :              13/06/2026
Date de dernière modification : 14/07/2026

Fichier     : pico_types.h
Description : Déclarations anticipées des types principaux

==============================================================================*/

#ifndef PICO_TYPES_H
# define PICO_TYPES_H

# include <stdint.h>

typedef uint8_t					u8;
typedef int8_t					s8;
typedef struct s_mini_ordi		Mini_ordi;
typedef struct s_modes			Modes;
typedef struct s_dbg			Dbg;

/* Codes renvoyés par le cœur et affichés par le programme */
enum	pico_status {
	/** Tout va bien */
	PICO_OK					= 0,
	/** Aller en Phase 1 */
	PICO_PHASE_1			= 1,
	/** Phase terminée */
	PICO_FIN_PHASE			= 2,
	/** Accès mémoire interdit */
	PICO_ERR_MEMOIRE		= 4,
	/** Instruction inconnue */
	PICO_ERR_INSTRUCTION	= 8,
	/** Défaillance processeur */
	PICO_ERR_COEUR			= 16,
	/** Extinction de Pico */
	PICO_STOP				= 32
};

typedef enum pico_status	pstatus;

#endif
