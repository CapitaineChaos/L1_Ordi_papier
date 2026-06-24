/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\                    Coeur                     /================

Auteur : Sylvain Maitre     24002886

Date de création :              01/10/2025
Date de dernière modification : 22/06/2026

Version du coeur : 1.3

Fichier     : coeur.c
Description : Exécution des instructions

==============================================================================*/

#include "coeur.h"
#include "debogueur.h"
#include "io.h"
#include "messages.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief Microcode 1
 * @note RS <- PC
 */
int	m_01(Mini_ordi *pico) {
	pico->RS = pico->PC;
	return (0);
}

/**
 * @brief Microcode 2
 * @note PC <- RM
 */
int	m_02(Mini_ordi *pico) {
	pico->PC = pico->RM;
	return (1);
}

/**
 * @brief Microcode 3
 * @note A <- RM
 */
int	m_03(Mini_ordi *pico) {
	pico->A = pico->RM;
	return (0);
}

/**
 * @brief Microcode 4
 * @note RM <- A
 */
int	m_04(Mini_ordi *pico) {
	pico->RM = pico->A;
	return (0);
}

/**
 * @brief Microcode 5
 * @note OP <- RM
 */
int	m_05(Mini_ordi *pico) {
	pico->OP = pico->RM;
	return (0);
}

/**
 * @brief Microcode 6
 * @note AD <- RM
 */
int	m_06(Mini_ordi *pico) {
	pico->AD = pico->RM;
	return (0);
}

/**
 * @brief Microcode 7
 * @note RS <- AD
 */
int	m_07(Mini_ordi *pico) {
	pico->RS = pico->AD;
	return (0);
}

/**
 * @brief Microcode 8
 * @note RM <- entrée
 */
int	m_08(Mini_ordi *pico) {
	pico->RM = pico->IO.in;
	return (0);
}

/**
 * @brief Microcode 9
 * @note sortie <- RM
 */
int	m_09(Mini_ordi *pico) {
	pico->IO.out = pico->RM;
	afficher_sortie(pico, pico->IO.out);
	return (0);
}

u8	UAL_add(u8 a, u8 b) { return (a + b); }

u8	UAL_sub(u8 a, u8 b) { return (a - b); }

u8	UAL_nand(u8 a, u8 b) { return (~(a & b)); }

/**
 * @brief Microcode 10
 * @note Mettre la fonction UAL en addition
 */
int	m_10(Mini_ordi *pico) {
	pico->UAL = UAL_add;
	snprintf(pico->dbg->texte.operation_ual, DBG_UAL_SIZE, "%s", DBG_UAL_ADD);
	return (0);
}

/**
 * @brief Microcode 11
 * @note Mettre la fonction UAL en soustraction
 */
int	m_11(Mini_ordi *pico) {
	pico->UAL = UAL_sub;
	snprintf(pico->dbg->texte.operation_ual, DBG_UAL_SIZE, "%s", DBG_UAL_SUB);
	return (0);
}

/**
 * @brief Microcode 17
 * @note Mettre la fonction UAL en NAND
 */
int	m_17(Mini_ordi *pico) {
	pico->UAL = UAL_nand;
	snprintf(pico->dbg->texte.operation_ual, DBG_UAL_SIZE, "%s", DBG_UAL_NAND);
	return (0);
}


/**
 * @brief Microcode 12
 * @note A <- UAL(A, RM)
 */
int	m_12(Mini_ordi *pico) {
	pico->A = pico->UAL(pico->A, pico->RM);
	return (0);
}

/**
 * @brief Microcode 13
 * @note RM = mem[RS]
 */
int	m_13(Mini_ordi *pico) {
	pico->RM = pico->mem[pico->RS];
	return (0);
}

/**
 * @brief Microcode 14
 * @note mem[RS] <- RM
 * @note Retourner 3 pour indiquer une erreur de mémoire
 */
int	m_14(Mini_ordi *pico) {
	if (pico->RS < 32)
		return (3);
	pico->mem[pico->RS] = pico->RM;
	return (0);
}

/**
 * @brief Microcode 15
 * @note Incrémenter PC
 */
int	m_15(Mini_ordi *pico) {
	pico->PC++;
	return (0);
}

/**
 * @brief Microcode 16
 * @note Lire une entrée
 */
int	m_16(Mini_ordi *pico) {
	pico->IO.in = lire_entree(pico, pico->PC);
	return (0);
}


t_microfonction	g_microfonctions[18] = { 
	NULL, m_01, m_02, m_03, m_04, m_05, m_06, m_07, m_08, m_09, m_10, m_11, m_12, m_13, m_14, m_15, m_16, m_17
};

/**
 * @brief Exécute un microcode donné
 * @param pico Le mini-ordinateur
 * @param dbg Le débogueur
 * @param microcode Le numéro du microcode à exécuter (1 à 17)
 * @return Code de retour d'execution ou de problème
 */
pstatus	exec_microcode(Mini_ordi *pico, Dbg *dbg, int microcode) {
	t_microfonction	p_microfct = NULL;

	if (microcode <= 0 || microcode >= 18)
		return (PICO_ERR_COEUR);

	p_microfct = g_microfonctions[microcode];
	if (!p_microfct)
		return (PICO_ERR_COEUR);

	if (dbg)
		dbg->exec.microcode_precedent = microcode;

	return ((pstatus)(p_microfct(pico)));
}

/**
 * @brief Phase : exécution du microcode
 * @return Code de retour :
 * @note 0 : OK
 * @note 1 : Retour à la phase 1
 * @note 2 : Fin de la boucle microcode
 * @note 3 : Erreur mémoire
 * @note 4 : Instruction inconnue
 * @note 5 : Défaillance processeur
 * @note 6 : Extinction de Pico
 */
pstatus	phase(Mini_ordi *pico, Dbg *dbg, t_mcseq *mseq, int phase) {
	int		i = 0;
	pstatus	status = PICO_OK;

	if (!mseq || mseq->nb_microcodes == 0) {
		if (pico->OP == 0xFF)
			return (PICO_STOP);
		return (PICO_ERR_INSTRUCTION);
	}
	if (mseq->cond && !mseq->cond(pico))
		return (PICO_OK);
	
	while (i < mseq->nb_microcodes)
	{
		exec_debogueur(pico, dbg, phase, mseq, i);
		if (dbg && dbg->exec.restart_cycle) {
			dbg->exec.restart_cycle = false;
			return (PICO_JUMP_PHASE_1);
		}
		status = exec_microcode(pico, dbg, mseq->sequence[i]);
		if (status) return (status);
		i++;
	}

	return (PICO_FIN_PHASE);
}


/**
 * @brief Boucle d'exécution des phases - Moteur principal du mini-ordinateur
 * @param pico Le mini-ordinateur
 * @param dbg Le débogueur
 * @return Code de retour :
 * @note 0 : OK
 * @note 1 : Retour à la phase 1
 * @note 2 : Fin de la boucle microcode
 * @note 3 : Erreur mémoire
 * @note 4 : Instruction inconnue
 * @note 5 : Défaillance processeur
 * @note 6 : Extinction de Pico
 */
pstatus	coeur(Mini_ordi *pico, Dbg *dbg) {
	pstatus	status = PICO_OK;
	t_mcseq	phase_1 = {
		.cond = NULL,
		.nb_microcodes = 4,
		.sequence = {1, 13, 5, 15}
	};
	t_mcseq	phase_3 = {
		.cond = NULL,
		.nb_microcodes = 1,
		.sequence = {15}
	};

	while (status < PICO_ERR_MEMOIRE)
	{
		if (pico->modes.verbeux && !pico->modes.debogage)
			printf(".");
		status = phase(pico, dbg, &phase_1, 1);
		if (status == PICO_JUMP_PHASE_1) continue;
		if (status > PICO_FIN_PHASE) return (status);
		status = phase(pico, dbg, &pico->microsequences[pico->OP], 2);
		if (status == PICO_JUMP_PHASE_1) continue;
		if (status > PICO_FIN_PHASE) return (status);
		status = phase(pico, dbg, &phase_3, 3);
		if (status == PICO_JUMP_PHASE_1) continue;
		if (status > PICO_FIN_PHASE) return (status);
	}
	return (PICO_OK);
}
