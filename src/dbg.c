/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\                   Débogueur                  /================

Auteur : Sylvain Maitre     24002886

Date de création :              01/10/2025
Date de dernière modification : 20/06/2026

Version du débogueur : 3.1

Fichier     : dbg.c
Description : Fonctions de débogage

==============================================================================*/

#include "dbg.h"
#include "dbg/cmd.h"
#include "dbg/display.h"
#include "dbg/render.h"
#include "dbg/viewer.h"
#include "coeur.h"
#include "messages.h"

/**
 * @brief Exécute le débogueur.
 * @note C'est le coeur du débogueur.
 * @param pico Pointeur vers la structure Mini_ordi.
 * @param dbg Pointeur vers la structure Dbg.
 * @param phase Phase d'exécution actuelle.
 * @param mseq Micro-séquence en cours d'exécution.
 * @param pos Position dans la micro-séquence.
 */
void	exec_debogueur(Mini_ordi *pico, Dbg *dbg, int phase, t_mcseq *mseq, int pos) {
	dbg_cmd	cmd = DBG_CMD_NONE;

	// Pas de debogueur actif on affiche rien
	if (!pico->modes.debogage)
		return;

	// Si on demande pas explicitiement à deboguer le bootstrap on l'ignore
	if (!pico->modes.bootstrap) {
		// Si on est dans le bootstrap
		// Et que le stdin est vide
		// Alors on estime que le bootstrap pourrait manquer de données
		// Mais on en est certain que si une demande d'entrée est faite (mc16)
		if (pico->PC < 32 && IO_STDIN_EMPTY && mseq->sequence[pos] == 16) {
			pico->modes.bootstrap = true;
			vider_etat_temporaire(dbg, false);
			render_set_text(dbg->texte.lg_status,sizeof(dbg->texte.lg_status), DBG_STATE_BOOTSTRAP_DEBUG);
		}
		// Faire en sorte de positionner PC sur le début du programme
		else if (pico->PC < 32 || phase != 1)
			return;
		else {
			pico->modes.bootstrap = true;
			vider_etat_temporaire(dbg, false);
		}
	}

	if (dbg->exec.next_microcode) {
		dbg->exec.next_microcode = false;
	}

	if (dbg->exec.next_phase) {
		if (pos == 0)
			dbg->exec.next_phase = false;
		else
			return;
	}

	if (dbg->exec.next_instruction) {
		// On triche un petit peu car on ne connaitra réellement l'instruction
		// qu'à partir du microcode 5 :
		// L'instruction est pointées par PC à la phase suivante.
		if (phase == 1 && pos == 0)
			dbg->exec.next_instruction = false;
		else
			return;
	}

	// Eviter le rafraichissement lié aux mouvements des sélections
	if (!dbg->exec.next_instruction && !dbg->exec.next_phase) {
		if (compositeur_ecran(pico, dbg, phase, mseq, pos))
			dbg_display_draw(dbg);
	}

	while (cmd == DBG_CMD_NONE || cmd == DBG_CMD_UNKNOWN || cmd == DBG_CMD_HELP || cmd == DBG_CMD_REFRESH)
	{
		cmd = attente_commande(pico, dbg, cmd == DBG_CMD_UNKNOWN);

		if (cmd == DBG_CMD_STOP_DBG) {
			dbg_display_leave(dbg);
			pico->modes.debogage = false;
			return;
		}

		if (cmd == DBG_CMD_EXEC_MICROCODE) {
			int num_microcode = dbg->exec.microcode_a_executer;
			render_set_line(dbg, DBG_STATE_LINE, DBG_STATE_FOND);
			render_lg_col(dbg, DBG_STATE_LINE, 1, DBG_STATE_EXEC_MICRO,
				num_microcode);
			exec_microcode(pico, dbg, num_microcode);
			cmd = DBG_CMD_NONE;
		}

		/** En cas de redimensionnement */
		if (cmd == DBG_CMD_REFRESH) {
			if (compositeur_ecran(pico, dbg, phase, mseq, pos))
				dbg_display_draw(dbg);
			cmd = DBG_CMD_NONE;
			return;
		}

		if (cmd == DBG_CMD_JUMP)
			return;

		if (cmd == DBG_CMD_NEXT_MICROCODE || (cmd == DBG_CMD_AUTO && dbg->exec.mode_defaut == DBG_DEF_MICROCODE)) {
			dbg->exec.next_microcode = true;
			dbg->exec.next_phase = false;
			dbg->exec.next_instruction = false;
			return;
		}
		if (cmd == DBG_CMD_NEXT_PHASE || (cmd == DBG_CMD_AUTO && dbg->exec.mode_defaut == DBG_DEF_PHASE)) {
			dbg->exec.next_microcode = false;
			dbg->exec.next_phase = true;
			dbg->exec.next_instruction = false;
			return;
		}
		if (cmd == DBG_CMD_NEXT_INSTRUCTION || (cmd == DBG_CMD_AUTO && dbg->exec.mode_defaut == DBG_DEF_INSTRUCTION)) {
			dbg->exec.next_microcode = false;
			dbg->exec.next_phase = false;
			dbg->exec.next_instruction = true;
			return;
		}

		if (compositeur_ecran(pico, dbg, phase, mseq, pos))
			dbg_display_draw(dbg);
	}
}
