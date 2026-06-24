/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\              Microcodes table                /================

Auteur : Sylvain Maitre     24002886

Date de création :              11/10/2025
Date de dernière modification : 22/06/2026

Fichier     : microcodes.c
Description : Définition des séquences de microcodes pour chaque instruction

==============================================================================*/

#include "pico.h"
#include <stddef.h>
#include <string.h>

bool	cond_A_negatif(Mini_ordi *pico) { return pico->A < 0; }
bool	cond_A_zero(Mini_ordi *pico) { return pico->A == 0; }

/**
 * @brief Charge les micro-séquences pour chaque instruction dans le mini-ordinateur.
 * 
 * @param pico Pointeur vers le mini-ordinateur.
 */
void charger_microsequences(Mini_ordi *pico) {
	pico->microsequences[OP_LOAD_IMM]  = (t_mcseq){ .cond = NULL,           .nb_microcodes = 3,  
		.sequence = {1, 13, 3}};
	pico->microsequences[OP_LOAD_ABS]  = (t_mcseq){ .cond = NULL,           .nb_microcodes = 6,  
		.sequence = {1, 13, 6, 7, 13, 3}};
	pico->microsequences[OP_LOAD_IND]  = (t_mcseq){ .cond = NULL,           .nb_microcodes = 9,  
		.sequence = {1, 13, 6, 7, 13, 6, 7, 13, 3}};
	pico->microsequences[OP_STORE_ABS] = (t_mcseq){ .cond = NULL,           .nb_microcodes = 6,  
		.sequence = {1, 13, 6, 7, 4, 14}};
	pico->microsequences[OP_STORE_IND] = (t_mcseq){ .cond = NULL,           .nb_microcodes = 9,  
		.sequence = {1, 13, 6, 7, 13, 6, 7, 4, 14}};
	pico->microsequences[OP_IN_ABS]    = (t_mcseq){ .cond = NULL,           .nb_microcodes = 7,  
		.sequence = {1, 13, 6, 7, 16, 8, 14}};
	pico->microsequences[OP_IN_IND]    = (t_mcseq){ .cond = NULL,           .nb_microcodes = 10, 
		.sequence = {1, 13, 6, 7, 13, 6, 7, 16, 8, 14}};
	pico->microsequences[OP_OUT_ABS]   = (t_mcseq){ .cond = NULL,           .nb_microcodes = 6,  
		.sequence = {1, 13, 6, 7, 13, 9}};
	pico->microsequences[OP_OUT_IND]   = (t_mcseq){ .cond = NULL,           .nb_microcodes = 9,  
		.sequence = {1, 13, 6, 7, 13, 6, 7, 13, 9}};
	pico->microsequences[OP_ADD_IMM]   = (t_mcseq){ .cond = NULL,           .nb_microcodes = 4,  
		.sequence = {10, 1, 13, 12}};
	pico->microsequences[OP_ADD_ABS]   = (t_mcseq){ .cond = NULL,           .nb_microcodes = 7,  
		.sequence = {10, 1, 13, 6, 7, 13, 12}};
	pico->microsequences[OP_ADD_IND]   = (t_mcseq){ .cond = NULL,           .nb_microcodes = 10, 
		.sequence = {10, 1, 13, 6, 7, 13, 6, 7, 13, 12}};
	pico->microsequences[OP_SUB_IMM]   = (t_mcseq){ .cond = NULL,           .nb_microcodes = 4,  
		.sequence = {11, 1, 13, 12}};
	pico->microsequences[OP_SUB_ABS]   = (t_mcseq){ .cond = NULL,           .nb_microcodes = 7,  
		.sequence = {11, 1, 13, 6, 7, 13, 12}};
	pico->microsequences[OP_SUB_IND]   = (t_mcseq){ .cond = NULL,           .nb_microcodes = 10, 
		.sequence = {11, 1, 13, 6, 7, 13, 6, 7, 13, 12}};
	pico->microsequences[OP_NAND_IMM]  = (t_mcseq){ .cond = NULL,           .nb_microcodes = 4,  
		.sequence = {17, 1, 13, 12}};
	pico->microsequences[OP_NAND_ABS]  = (t_mcseq){ .cond = NULL,           .nb_microcodes = 7,  
		.sequence = {17, 1, 13, 6, 7, 13, 12}};
	pico->microsequences[OP_NAND_IND]  = (t_mcseq){ .cond = NULL,           .nb_microcodes = 10, 
		.sequence = {17, 1, 13, 6, 7, 13, 6, 7, 13, 12}};
	pico->microsequences[OP_JUMP_ABS]  = (t_mcseq){ .cond = NULL,           .nb_microcodes = 3,  
		.sequence = {1, 13, 2}};
	pico->microsequences[OP_BRN_ABS]   = (t_mcseq){ .cond = cond_A_negatif, .nb_microcodes = 3,  
		.sequence = {1, 13, 2}};
	pico->microsequences[OP_BRZ_ABS]   = (t_mcseq){ .cond = cond_A_zero,    .nb_microcodes = 3,  
		.sequence = {1, 13, 2}};
}
