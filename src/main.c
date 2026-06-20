/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\                Point d'entrée                /================

Auteur : Sylvain Maitre     24002886

Date de création :              01/10/2025
Date de dernière modification : 19/06/2026

Description : Point d'entrée du programme
Compilation : make
Execution   : ./cx25.1 [-dbhv]

Remarque : Ne fonctionnera pas sur Windows
==============================================================================*/

#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include "signaux.h"
#include "pico.h"
#include "messages.h"
#include "coeur.h"
#include "io.h"
#include "debogueur.h"
#include "debogueur/affichage.h"
#include "outils.h"

void	options(int ac, char** av, Mini_ordi *pico);
void	afficher_utilisation(char *nom, int exit_code);
void	charger_microsequences(Mini_ordi *pico);


/**
 * @brief Initialise la structure Mini_ordi.
 * @param pico Pointeur vers la structure Mini_ordi.
 * @note Le bootstrap est chargé par défaut dans la mémoire.
 * @todo Donner la possibilité de charger un autre bootstrap.
 */
void	init_pico(Mini_ordi *pico) {
	*pico = (Mini_ordi){
		.mem = {
			0x49, 0x20, 0x49, 0x22, 0x40, 0x20, 0x48, 0x21,
			0xC9, 0x21, 0x40, 0x22, 0x21, 0x01, 0x12, 0x1F,
			0x48, 0x22, 0x40, 0x21, 0x20, 0x01, 0x10, 0x06,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10
		},
		.PC = 0x00,
		.A = 0x00,
		.microsequences = {{0}},
		.IO = {
			.in = 0,
			.out = 0,
			.buffer = {0},
			.buffer_len = 0,
			.buffer_pos = 0,
			.output = {0},
			.output_len = 0,
			.input_file = NULL,
			.stdin_is_tty = true,
		},
		.modes = {
			.bootstrap = false,
			.debogage = false,
			.verbeux = false,
		},
		.prog_debut = 0,
		.prog_taille = 0,
		.prog_params = false,
		.UAL = NULL,
		.dbg = NULL
	};
}

/**
 * @brief Initialise la structure de débogage.
 * @param dbg Pointeur vers la structure Dbg.
 */
void	init_dbg(Dbg *dbg)
{
	*dbg = (Dbg){
		.exec = {
			.microcode_precedent = 0,
			.microcode_a_executer = 0,
			.mode_defaut = DBG_DEF_MICROCODE,
			.next_microcode = false,
			.next_phase = false,
			.next_instruction = false,
			.restart_cycle = false,
		},
		.texte = {
			.instruction = "",
			.microcode = "",
			.operation_ual = "",
			.lg_status = "",
			.lg_mode = "",
			.lg_cmd = "",
		},
		.terminal = {
			.tty = NULL,
			.viewer_actif = false,
			.raw_actif = false,
			.orig = {0},
			.petit_affiche = false,
		},
		.screen = {
			.precedent = {{0}},
			.courant = {{0}},
			.initialise = false,
		},
		.erreur_prog = 0,
	};
}

/**
 * @brief Point d'entrée du programme.
 * @param ac Nombre d'arguments.
 * @param av Tableau des arguments.
 */
int	main(int ac, char** av) {
	Mini_ordi 	pico;
	Dbg			dbg;
	int			ret = 0;

	init_sig();
	init_pico(&pico);

	options(ac, av, &pico);
	charger_buffer_entree(&pico);
	
	init_dbg(&dbg);
	pico.dbg = &dbg;

	charger_microsequences(&pico);
	while ((ret = coeur(&pico, &dbg))) {
		if (ret < 3 || !pico.modes.debogage)
			break;
		dbg.erreur_prog = ret;
	}
	dbg_display_leave(&dbg);
	msg_print_error(&pico, ret);
	printf(RST D_WRAP D_SHOW_CURSOR);
	return (ret);
}

/**
 * @brief Affiche l'utilisation du programme et quitte.
 * @param nom Nom du programme.
 * @param exit_code Code de sortie.
 */
void	afficher_utilisation(char *nom, int exit_code) {
	msg_print_usage(nom);
	printf(RST D_WRAP D_SHOW_CURSOR);
	exit (exit_code);
}


/**
 * @brief Convertit une chaîne de caractères en un entier non signé 8 bits.
 * @param nom Nom de l'option.
 * @param str Chaîne de caractères à convertir.
 * @return Valeur convertie.
 */
static u8	option_u8(const char *nom, const char *str) {
	u8	val;

	if (!pico_parse_u8(str, &val)) {
		fprintf(stderr, MSG_ERR_OPTION_INVALID, nom, str);
		exit(EXIT_FAILURE);
	}
	return (val);
}

/**
 * @brief Analyse les options de la ligne de commande.
 * @param ac Nombre d'arguments.
 * @param av Tableau des arguments.
 * @param pico Pointeur vers la structure Mini_ordi.
 */
void	options(int ac, char** av, Mini_ordi *pico) {
	static struct option	long_options[] = {
		{"adresse", required_argument, NULL, 'a'},
		{"debug", no_argument, NULL, 'd'},
		{"bootstrap", no_argument, NULL, 'b'},
		{"file", required_argument, NULL, 'f'},
		{"help", no_argument, NULL, 'h'},
		{"taille", required_argument, NULL, 't'},
		{"verbose", no_argument, NULL, 'v'},
		{0, 0, 0, 0},
	};
	int						opt;
	bool					adresse_fournie = false;
	bool					taille_fournie = false;

	while ((opt = getopt_long(ac, av, "a:dbhf:t:v", long_options, NULL)) != -1) {
		switch (opt) {
			case 'h':	afficher_utilisation(av[0], EXIT_SUCCESS); break;
			case 'a':	pico->prog_debut = option_u8(MSG_OPT_ADRESSE, optarg); adresse_fournie = true; break;
			case 'd':	pico->modes.debogage = true; break;
			case 'b':	pico->modes.bootstrap = true; break;
			case 'f':	pico->IO.input_file = optarg; break;
			case 't':	pico->prog_taille = option_u8(MSG_OPT_TAILLE, optarg); taille_fournie = true; break;
			case 'v':	pico->modes.verbeux = true; break;
			default: exit (EXIT_FAILURE);
		}
	}
	if (adresse_fournie != taille_fournie) {
		fprintf(stderr, MSG_ERR_PROG_RANGE_OPTIONS);
		exit(EXIT_FAILURE);
	}
	pico->prog_params = adresse_fournie;
	if (ac - optind > 0)
		afficher_utilisation(av[0], EXIT_FAILURE);
}
