/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\              Viewer du débogueur             /================

Auteur : Sylvain Maitre     24002886

Date de création :              11/06/2026
Date de dernière modification : 20/06/2026

Version du viewer : 4.5

Fichier     : debogueur/vue.c
Description : Construction de l'écran du débogueur

==============================================================================*/

#include "debogueur/vue.h"
#include "debogueur/compositeur.h"
#include "debogueur/decodage.h"
#include "debogueur/rendu.h"
#include "debogueur/libelles.h"

// Rappel, les colonnes ANSI sont basées sur 1 et non 0

#define INS	dbg->texte.instruction

#define MEM_LINE_FIRST	3
#define ADDR_COL		2
#define MEM_COL			14
#define MEM_BYTE_WIDTH	3
#define REG_COL			68

/**
 * @brief Affiche le titre de l'écran du débogueur
 * @param dbg Le débogueur
 */
static void	compositeur_titre(Dbg *dbg) {
	render_set_line(dbg, DBG_HEADER_LINE, DBG_FOND_WH);
	render_lg_col(dbg, DBG_HEADER_LINE, ADDR_COL, DBG_ENTETE_ADR);
	render_lg_col(dbg, DBG_HEADER_LINE, MEM_COL + 1, DBG_ENTETE_MEM);
	render_lg_col(dbg, DBG_HEADER_LINE, REG_COL + 1, DBG_ENTETE_REG);
}

/**
 * @brief Affiche les registres du mini-ordinateur
 * @param pico Le mini-ordinateur
 * @param dbg Le débogueur
 * @param lg Le numéro de la ligne de départ pour l'affichage des registres
 * @note Affiche les registres du mini-ordinateur à partir de la ligne lg
 * @note Affiche les registres PC, RS, RM, AD, OP, A, Z, N, UAL, IN et OUT
 * @note Met en évidence les registres modifiés ou lus
 */
static void	compositeur_registre(Mini_ordi *pico, Dbg *dbg, int lg) {
	int	microcode;
	int	ecran_lg;
	int	i = 0;

	microcode = dbg->exec.microcode_precedent;
	ecran_lg = MEM_LINE_FIRST;

	/** Création du cadre des registres */

	render_lg_col(dbg, ecran_lg + lg++, REG_COL, DBG_RG_HAUT);
	while (i < 14) {
		render_lg_col(dbg, ecran_lg + lg + i++, REG_COL, DBG_RG_MILIEU);
	}
	render_lg_col(dbg, ecran_lg + lg + i, REG_COL, DBG_RG_BAS);

	/** Affichage des valeurs des registres */

	render_lg_col(dbg, ecran_lg + lg++, REG_COL + 1, DBG_RG_PC(microcode, pico->PC));
	render_lg_col(dbg, ecran_lg + lg++, REG_COL + 1, DBG_RG_RS(microcode, pico->RS));
	render_lg_col(dbg, ecran_lg + lg++, REG_COL + 1, DBG_RG_RM(microcode, pico->RM));
	render_lg_col(dbg, ecran_lg + lg++, REG_COL + 1, DBG_RG_AD(microcode, pico->AD));
	render_lg_col(dbg, ecran_lg + lg++, REG_COL + 1, DBG_RG_OP(microcode, pico->OP));
	lg++;
	render_lg_col(dbg, ecran_lg + lg++, REG_COL + 1, DBG_RG_A(microcode, pico->A));
	render_lg_col(dbg, ecran_lg + lg++, REG_COL + 1, DBG_RG_Z(microcode, pico->A));
	render_lg_col(dbg, ecran_lg + lg++, REG_COL + 1, DBG_RG_N(microcode, pico->A));
	lg++;
	render_lg_col(dbg, ecran_lg + lg++, REG_COL + 1, DBG_RG_UAL(microcode, dbg->texte.operation_ual));
	lg++;
	render_lg_col(dbg, ecran_lg + lg++, REG_COL + 1, DBG_RG_IN(microcode, pico->IO.in));
	render_lg_col(dbg, ecran_lg + lg++, REG_COL + 1, DBG_RG_OUT(microcode, pico->IO.out));
}

/**
 * @brief Affiche une ligne de la mémoire du mini-ordinateur
 * @param pico Le mini-ordinateur
 * @param dbg Le débogueur
 * @param lg Le numéro de la ligne à afficher (0 à 15)
 * @note Affiche les 16 octets de la ligne, ainsi que leur adresse
 * @note Met en évidence l'octet pointé par le PC et le RS
 * ainsi que les octets modifiés par les microcodes 13 et 14
 */
static void	afficher_ligne_memoire(Mini_ordi *pico, Dbg *dbg, int lg) {
	int	addr;
	int	col;
	int	d_lg, d_col;
	int	val, PC, RS, M;

	addr = lg * 16;
	// Ligne complete fond noir
	render_set_line(dbg, MEM_LINE_FIRST + lg, DBG_FOND_BK);
	// Affichage de l'adresse de la ligne
	render_lg_col(dbg, MEM_LINE_FIRST + lg, ADDR_COL, DBG_AFF_ADR(addr));
	col = 0;
	while (col < 16) {
		addr = lg * 16 + col;
		val = pico->mem[addr];
		PC = pico->PC;
		RS = pico->RS;
		M = dbg->exec.microcode_precedent;
		d_lg = MEM_LINE_FIRST + lg;
		d_col = MEM_COL + col * MEM_BYTE_WIDTH;
		// Affichage en cellule de l'octet avec mise en évidence
		render_lg_col(dbg, d_lg, d_col, DBG_AFF_MEM(addr, val, PC, RS, M));
		col++;
	}
}

/**
 * @brief Affiche la mémoire totale du mini-ordinateur en 16x16 octets
 * @param pico Le mini-ordinateur
 * @param dbg Le débogueur
 */
static void	compositeur_memoire(Mini_ordi *pico, Dbg *dbg) {
	int	ligne;

	ligne = 0;
	while (ligne < 16) {
		afficher_ligne_memoire(pico, dbg, ligne);
		ligne++;
	}
}


/**
 * @brief Affiche la phase en cours
 * @param dbg Le débogueur
 * @param phase La phase en cours
 * @note Affiche la phase en cours sur la ligne 19
 */
static void compositeur_phase(Dbg *dbg, int phase) {
	render_set_line(dbg, DBG_PHASE_LINE, BGG_06 DBG_EMPTY_LINE);
	render_lg_col(dbg, DBG_PHASE_LINE, 36, DBG_PHASE(phase));
}


/**
 * @brief Affiche les microcodes de la phase en cours sur 2 lignes
 * @param dbg Le débogueur
 * @param mseq La microsequence de la phase en cours
 * @param pos La position du microcode en cours dans la microsequence
 * @note Ligne 20 : microcode précédent et suivants
 * @note Ligne 21 : microcode à venir et sa sémantique
 */
static void	compositeur_microcodes(Dbg *dbg, t_mcseq *mseq, int pos) {
	int	i = 0;
	int	microcode = 0;
	int microcode_p = dbg->exec.microcode_precedent;

	/**  première ligne */

	// Ligne complète fond noir
	render_set_line(dbg, DBG_MICRO_LINE, DBG_FOND_BK);
	// Microcode précédent
	render_lg_col(dbg, DBG_MICRO_LINE, 3, DBG_MICROCODE_P(microcode_p));
	// Séparateur
	render_append_to_line(dbg, DBG_MICRO_LINE, DBG_MICROCODE_S(microcode_p));
	// liste des microcodes suivants
	while (i < mseq->nb_microcodes) {
		microcode = mseq->sequence[i];
		render_append_to_line(dbg, DBG_MICRO_LINE, DBG_MICROCODE(i == pos, microcode));
		i++;
	}

	/** Deuxième ligne */

	decode_microcode(microcode, dbg);
	render_set_line(dbg, DBG_LMICRO_LINE, DBG_FOND_BK);
	render_lg_col(dbg, DBG_LMICRO_LINE, 3, DBG_LMICROCODE(mseq->sequence[pos], dbg->texte.microcode));
}

/**
 * @brief Affiche l'instruction à venir et sa sémantique
 * @param pico Le mini-ordinateur
 * @param dbg Le débogueur
 * @param phase La phase en cours
 * @param pos La position du microcode en cours dans la microsequence
 * @note Affiche l'instruction à venir
 */
static void	compositeur_instruction(Mini_ordi *pico, Dbg *dbg, int phase, int pos) {
	// L'instruction n'est connue qu'à partir du microcode n°5 de la phase 1
	// Soit en position 3
	if (phase == 1 && pos == 3)
		decode_instruction(pico, dbg);
	render_set_line(dbg, DBG_INSTR_LINE, DBG_FOND_BK);
	render_lg_col(dbg, DBG_INSTR_LINE, 3, DBG_LINSTRUCTION(dbg->texte.instruction));
}


/**
 * @brief Affiche le mode d'exécution actuel du débogueur
 * @param dbg Le débogueur
 * @note Affiche le mode d'exécution actuel du débogueur sur la ligne 23
 * @note par défaut le mode d'exécution est le mode microcode
 */
static void	compositeur_mode(Dbg *dbg) {
	if (dbg->texte.lg_mode[0] == '\0')
		render_set_text(dbg->texte.lg_mode, sizeof(dbg->texte.lg_mode), DBG_MODE_MICROCODE);
	render_set_line(dbg, DBG_MODE_LINE, BGG_06 DBG_EMPTY_LINE);
	render_lg_col(dbg, DBG_MODE_LINE, 26, dbg->texte.lg_mode);
}


/**
 * @brief Compose l'écran principal du débogueur dans le buffer
 * @param pico Le mini-ordinateur
 * @param dbg Le débogueur
 * @param phase La phase en cours
 * @param mseq La microsequence de la phase en cours
 * @param pos La position du microcode en cours dans la microsequence
 * @return true si l'écran a été composé, false si le terminal n'est pas affichable
 * @note Compose la mémoire, les registres, la phase en cours, les microcodes,
 * l'instruction à venir et le mode d'exécution. Ne dessine pas : l'affichage
 * est déclenché par l'appelant via dbg_display_draw().
 */
bool	compositeur_ecran(Mini_ordi *pico, Dbg *dbg, int phase, t_mcseq *mseq, int pos) {
	if (!compositeur_debut(dbg))
		return (false);
	compositeur_top_menu(pico, dbg);
	compositeur_titre(dbg);
	compositeur_memoire(pico, dbg);
	compositeur_registre(pico, dbg, 0);
	compositeur_phase(dbg, phase);
	compositeur_mode(dbg);
	compositeur_microcodes(dbg, mseq, pos);
	compositeur_instruction(pico, dbg, phase, pos);
	return (true);
}
