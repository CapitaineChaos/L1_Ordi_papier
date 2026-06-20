/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\         Parser des commandes debug           /================

Auteur : Sylvain Maitre     24002886

Date de création :              12/06/2026
Date de dernière modification : 20/06/2026

Fichier     : debogueur/analyseur.c
Description : Interprétation des commandes debug validées

==============================================================================*/

#include "debogueur/analyseur.h"
#include "debogueur/menu.h"
#include "debogueur/rendu.h"
#include "debogueur/libelles.h"
#include "outils.h"
#include <ctype.h>
#include <stdio.h>
#include <strings.h>

static bool	is_help_cmd(const char *cmd) {
	return (!strcasecmp(cmd, "help") || !strcasecmp(cmd, "h") || !strcasecmp(cmd, "?") || !strcasecmp(cmd, "aide"));
}

static bool	is_input_cmd(const char *cmd) {
	return (!strcasecmp(cmd, "i") || !strcasecmp(cmd, "in") || !strcasecmp(cmd, "input"));
}

static bool	is_output_cmd(const char *cmd) {
	return (!strcasecmp(cmd, "o") || !strcasecmp(cmd, "out") || !strcasecmp(cmd, "output"));
}

static bool	is_quit_cmd(const char *cmd) {
	return (!strcasecmp(cmd, "quit") || !strcasecmp(cmd, "q") || !strcasecmp(cmd, "exit"));
}

static bool	is_jump_cmd(const char *cmd) {
	return (!strcasecmp(cmd, "jump") || !strcasecmp(cmd, "j"));
}

static bool	is_microcode_cmd(const char *cmd) {
	return (!strcasecmp(cmd, "m") || !strcasecmp(cmd, "mc"));
}

static bool	is_stop_dbg_cmd(const char *cmd) {
	return (!strcasecmp(cmd, "s") || !strcasecmp(cmd, "stop"));
}

/**
 * @brief Parse une commande avec un argument de type u8 saisie par l'utilisateur dans le débogueur
 * @param str La chaîne de caractères contenant la commande
 * @param cmd La chaîne de caractères où sera stockée la commande parsée
 * @param cmd_size La taille de la chaîne cmd
 * @param val L'adresse où sera stockée la valeur parsée
 * @return true si la commande a été correctement parsée, false sinon
 */
static bool	parser_cmd_u8(const char *str, char *cmd, size_t cmd_size, u8 *val) {
	size_t	i;

	if (!str || !cmd || cmd_size == 0 || !val)
		return (false);
	while (isspace((unsigned char)*str))
		str++;
	i = 0;
	while (isalpha((unsigned char)str[i]) && i + 1 < cmd_size) {
		cmd[i] = str[i];
		i++;
	}
	cmd[i] = '\0';
	if (i == 0)
		return (false);
	str += i;
	while (isspace((unsigned char)*str))
		str++;
	return (pico_parse_u8(str, val));
}

/**
 * @brief Parse une commande de changement de mode par défaut saisie par l'utilisateur dans le débogueur
 * @param dbg Le débogueur
 * @param cmd La chaîne de caractères contenant la commande
 * @return true si la commande a été correctement parsée, false sinon
 */
static bool	is_def_change_cmd(Dbg *dbg, const char *cmd) {
	if (!strcasecmp(cmd, "**")) {
		dbg->exec.mode_defaut = DBG_DEF_INSTRUCTION;
		render_set_text(dbg->texte.lg_mode, sizeof(dbg->texte.lg_mode), DBG_MODE_INSTRUCTION);
		return (true);
	}
	if (!strcasecmp(cmd, "++")) {
		dbg->exec.mode_defaut = DBG_DEF_PHASE;
		render_set_text(dbg->texte.lg_mode, sizeof(dbg->texte.lg_mode), DBG_MODE_PHASE);
		return (true);
	}
	if (!strcasecmp(cmd, "--")) {
		dbg->exec.mode_defaut = DBG_DEF_MICROCODE;
		render_set_text(dbg->texte.lg_mode, sizeof(dbg->texte.lg_mode), DBG_MODE_MICROCODE);
		return (true);
	}
	return (false);
}

/**
 * @brief Parse une commande de microcode saisie par l'utilisateur dans le débogueur
 * @param dbg Le débogueur
 * @param str La chaîne de caractères contenant la commande
 * @return true si la commande a été correctement parsée, false sinon
 */
static bool	parser_microcode_cmd(Dbg *dbg, const char *str) {
	char	cmd[10];
	u8		microcode;

	if (!parser_cmd_u8(str, cmd, sizeof(cmd), &microcode))
		return (false);
	if (!is_microcode_cmd(cmd) || microcode <= 0 || microcode >= 18)
		return (false);
	dbg->exec.microcode_a_executer = microcode;
	return (true);
}

/**
 * @brief Parse une commande de saut (jump) saisie par l'utilisateur dans le débogueur
 * @param pico Le mini-ordinateur
 * @param dbg Le débogueur
 * @param str La chaîne de caractères contenant la commande
 * @return true si la commande a été correctement parsée, false sinon
 */
static bool	parser_jump_cmd(Mini_ordi *pico, Dbg *dbg, const char *str) {
	char	cmd[10];
	u8		adresse;

	if (!parser_cmd_u8(str, cmd, sizeof(cmd), &adresse))
		return (false);
	if (!is_jump_cmd(cmd))
		return (false);
	pico->PC = adresse;
	pico->RS = adresse;
	pico->RM = pico->mem[adresse];
	if (adresse < 32)
		pico->modes.bootstrap = true;
	dbg->exec.microcode_precedent = 0;
	dbg->exec.microcode_a_executer = 0;
	dbg->exec.next_microcode = false;
	dbg->exec.next_phase = false;
	dbg->exec.next_instruction = false;
	dbg->exec.restart_cycle = true;
	render_set_text(dbg->texte.lg_status, sizeof(dbg->texte.lg_status),
		DBG_STATE_JUMP, pico->PC);
	return (true);
}

/**
 * @brief Parse une commande saisie par l'utilisateur dans le débogueur
 * @param pico Le mini-ordinateur
 * @param dbg Le débogueur
 * @param str La chaîne de caractères contenant la commande
 * @return Le code de commande correspondant
 */
dbg_cmd	parser_commande(Mini_ordi *pico, Dbg *dbg, const char *str) {
	char	cmd[10] = {0};

	if (sscanf(str, "%9s", cmd) < 1)
		return (DBG_CMD_UNKNOWN);
	if (is_quit_cmd(cmd))
		return (DBG_CMD_QUIT);
	if (parser_microcode_cmd(dbg, str))
		return (DBG_CMD_EXEC_MICROCODE);
	if (parser_jump_cmd(pico, dbg, str))
		return (DBG_CMD_JUMP);
	if (!strcasecmp(cmd, "*"))
		return (DBG_CMD_NEXT_INSTRUCTION);
	if (!strcasecmp(cmd, "+"))
		return (DBG_CMD_NEXT_PHASE);
	if (!strcasecmp(cmd, "-"))
		return (DBG_CMD_NEXT_MICROCODE);
	if (is_stop_dbg_cmd(cmd))
		return (DBG_CMD_STOP_DBG);
	if (is_def_change_cmd(dbg, cmd))
		return (DBG_CMD_NONE);
	if (is_help_cmd(cmd))
		return (attendre_menu(pico, dbg, DBG_CMD_SHOW_HELP));
	if (is_input_cmd(cmd) && IO_STDIN_AVAILABLE)
		return (attendre_menu(pico, dbg, DBG_CMD_SHOW_INPUT));
	if (is_output_cmd(cmd) && IO_OUTPUT_AVAILABLE)
		return (attendre_menu(pico, dbg, DBG_CMD_SHOW_OUTPUT));
	return (DBG_CMD_UNKNOWN);
}
