/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\             Messages du programme            /================

Auteur : Sylvain Maitre     24002886

Date de création :              07/06/2026
Date de dernière modification : 22/06/2026

Fichier     : messages.c
Description : Affichage des messages utilisés dans le programme

==============================================================================*/

#include "messages.h"
#include "pico.h" // IWYU pragma: keep
#include "pico_types.h"
#include <signal.h>
#include <stdio.h>

#define MSG_BUFFER_SIZE 4096

void	msg_print_signal(int sig) {
	switch (sig) {
		case SIGINT:
			printf(MSG_SIG_INT);
			break;
		case SIGTERM:
			printf(MSG_SIG_TERM);
			break;
		case SIGQUIT:
			printf(MSG_SIG_QUIT);
			break;
		case SIGHUP:
			printf(MSG_SIG_HUP);
			break;
		case SIGTSTP:
			printf(MSG_SIG_STP);
			break;
		default:
			printf(MSG_SIG_AUTRE);
			break;
	}
}

void	msg_print_usage(const char *nom) {
	printf(MSG_USAGE, nom, nom, nom);
}

void	msg_print_error(Mini_ordi *pico, pstatus status) {
	if (!status || status == PICO_STOP)
		return ;
	if (status == PICO_ERR_MEMOIRE)
		fprintf(stderr, MSG_ERR_MEMORY, pico->RS);
	else if (status == PICO_ERR_INSTRUCTION)
		fprintf(stderr, MSG_ERR_INSTRUCTION, pico->OP);
	else if (status == PICO_ERR_COEUR)
		fprintf(stderr, MSG_ERR_CORE);
	else
		fprintf(stderr, MSG_ERR_INCONNUE, status);
}

void	msg_print_debug_help(void) {
	printf(D_HOME E_SCREEN_ALL);
	printf(MSG_CMDS);
}

void	msg_print_input_prompt(Modes *modes) {
	if (!modes->verbeux)
		return;
	if (modes->bootstrap)
		printf(modes->mode_hexa ? MSG_INPUT_BOOTSTRAP : MSG_INPUT_BOOTSTRAP_DEC);
	else
		printf(modes->mode_hexa ? MSG_INPUT_PROGRAM : MSG_INPUT_PROGRAM_DEC);
	printf(RST " $ ");
}

void	msg_perror_tty(void) {
	fprintf(stderr, MSG_ERR_TTY);
}

