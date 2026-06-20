/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\              Décodage affichage              /================

Auteur : Sylvain Maitre     24002886

Date de création :              12/06/2026
Date de dernière modification : 20/06/2026

Fichier     : debogueur/decodage.c
Description : Décodage textuel des microcodes et instructions

==============================================================================*/

#include "debogueur/decodage.h"
#include "debogueur/semantique.h"
#include <stdio.h>

#define INS	dbg->texte.instruction
#define MIC	dbg->texte.microcode
#define OPV	pico->mem[pico->PC + 1]
#define OPD	pico->mem[OPV]

/**
 * @brief Décode un microcode du mini-ordinateur
 * @param microcode Le microcode à décoder
 * @param dbg Le débogueur
 */
void	decode_microcode(int microcode, Dbg *dbg) {
	switch (microcode) {
		case 1:		snprintf(STR_MIC_1(MIC));	break;
		case 2:		snprintf(STR_MIC_2(MIC));	break;
		case 3:		snprintf(STR_MIC_3(MIC));	break;
		case 4:		snprintf(STR_MIC_4(MIC));	break;
		case 5:		snprintf(STR_MIC_5(MIC));	break;
		case 6:		snprintf(STR_MIC_6(MIC));	break;
		case 7:		snprintf(STR_MIC_7(MIC));	break;
		case 8:		snprintf(STR_MIC_8(MIC));	break;
		case 9:		snprintf(STR_MIC_9(MIC));	break;
		case 10:	snprintf(STR_MIC_10(MIC));	break;
		case 11:	snprintf(STR_MIC_11(MIC));	break;
		case 12:	snprintf(STR_MIC_12(MIC));	break;
		case 13:	snprintf(STR_MIC_13(MIC));	break;
		case 14:	snprintf(STR_MIC_14(MIC));	break;
		case 15:	snprintf(STR_MIC_15(MIC));	break;
		case 16:	snprintf(STR_MIC_16(MIC));	break;
		case 17:	snprintf(STR_MIC_17(MIC));	break;
		default:	snprintf(STR_MIC_INC(MIC));	break;
	}
}

/**
 * @brief Décode une instruction du mini-ordinateur
 * @param pico Le mini-ordinateur
 * @param dbg Le débogueur
 */
void	decode_instruction(Mini_ordi *pico, Dbg *dbg) {
	switch (pico->OP) {
		case OP_ADD_IMM:	snprintf(STR_ADD_IMM(INS, OPV));		break;
		case OP_ADD_ABS:	snprintf(STR_ADD_ABS(INS, OPV));		break;
		case OP_ADD_IND:	snprintf(STR_ADD_IND(INS, OPV, OPD));	break;
		case OP_SUB_IMM:	snprintf(STR_SUB_IMM(INS, OPV));		break;
		case OP_SUB_ABS:	snprintf(STR_SUB_ABS(INS, OPV));		break;
		case OP_SUB_IND:	snprintf(STR_SUB_IND(INS, OPV, OPD));	break;
		case OP_NAND_IMM:	snprintf(STR_NAND_IMM(INS, OPV));		break;
		case OP_NAND_ABS:	snprintf(STR_NAND_ABS(INS, OPV));		break;
		case OP_NAND_IND:	snprintf(STR_NAND_IND(INS, OPV, OPD));	break;
		case OP_LOAD_IMM:	snprintf(STR_LOAD_IMM(INS, OPV));		break;
		case OP_LOAD_ABS:	snprintf(STR_LOAD_ABS(INS, OPV));		break;
		case OP_LOAD_IND:	snprintf(STR_LOAD_IND(INS, OPV, OPD));	break;
		case OP_STORE_ABS:	snprintf(STR_STORE_ABS(INS, OPV));		break;
		case OP_STORE_IND:	snprintf(STR_STORE_IND(INS, OPV, OPD));	break;
		case OP_IN_ABS:		snprintf(STR_IN_ABS(INS, OPV));			break;
		case OP_IN_IND:		snprintf(STR_IN_IND(INS, OPV, OPD));	break;
		case OP_OUT_ABS:	snprintf(STR_OUT_ABS(INS, OPV));		break;
		case OP_OUT_IND:	snprintf(STR_OUT_IND(INS, OPV, OPD));	break;
		case OP_JUMP_ABS:	snprintf(STR_JUMP_ABS(INS, OPV));		break;
		case OP_BRN_ABS:	snprintf(STR_BRN_ABS(INS, OPV));		break;
		case OP_BRZ_ABS:	snprintf(STR_BRZ_ABS(INS, OPV));		break;
		default:			snprintf(STR_INCONNUE(INS));			break;
	}
}
