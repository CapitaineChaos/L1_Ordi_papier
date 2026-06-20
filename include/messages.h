/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\              Header des messages             /================

Auteur : Sylvain Maitre     24002886

Date de création :              03/10/2025
Date de dernière modification : 20/06/2026

Fichier     : messages.h
Description : Messages généraux du programme (CLI, erreurs, signaux)

Note : l'interface du débogueur (couleurs, gabarits) vit dans debogueur/libelles.h ;
la sémantique des microcodes/instructions dans debogueur/semantique.h.

==============================================================================*/

#ifndef MESSAGES_H
# define MESSAGES_H

# include "ansi.h"
# include "pico_types.h"

# include <stdbool.h>
# include <stddef.h>

void	msg_print_signal(int sig);
void	msg_print_usage(const char *nom);
void	msg_print_error(Mini_ordi *pico, int code);
void	msg_print_debug_help(void);
void	msg_print_input_prompt(Modes *modes);
void	msg_perror_tty(void);
void	msg_print_hex(int val);


# define MSG_CMDS "\n" \
	"  Commandes disponibles :\n\n" \
	"     Entrée       : Pas suivant  (step)\n" \
	"     *            : Passer à l'instruction suivante\n" \
	"     **           : step = instruction\n" \
	"     +            : Passer à la phase suivante\n" \
	"     ++           : step = phase\n" \
	"     -            : Passer au microcode suivant\n" \
	"     --           : step = microcode\n" \
	"     m NN         : (m, mc) exécuter le microcode NN\n" \
	"     j NN         : (j, jump) placer PC à l'adresse NN  / 0xNN\n\n" \
	"  Menus :\n\n" \
	"    i / in       : afficher le buffer input\n" \
	"    o / out      : afficher le buffer output\n" \
	"    q            : quitter le programme\n" \
	"    s            : stopper le débogueur\n" \
	"    ?            : afficher cette aide\n"


# define MSG_USAGE \
	"Usage : %s [options]\n" \
	"\n" \
	"Options :\n" \
	"  -h, --help        Afficher ce message d'aide\n" \
	"  -v, --verbose     Afficher des messages détaillés sur l'exécution\n" \
	"  -b, --bootstrap   Déboguer avec le bootstrap\n" \
	"  -d, --debug       Mode débogage\n" \
	"  -f, --file HEX    Ajouter un fichier .hex au buffer d'entrée\n" \
	"  -a, --adresse N   Adresse de début du programme (-t requis)\n" \
	"  -t, --taille N    Taille du programme (-a requis)\n" \
	"\n" \
	"Exemples :\n" \
	"  %s < programme.hex         (mode classique)\n" \
	"  cat programme.hex | %s -b  (mode pas à pas avec le bootstrap)\n\n"


# define MSG_INPUT_BOOTSTRAP		"Le bootstrap demande la saisie de 2 symboles hexa :"
# define MSG_INPUT_PROGRAM			"Le programme demande la saisie de 2 symboles hexa :"

// Glyphes de l'opération UAL (écrits par le cœur, affichés par le débogueur)
# define DBG_UAL_NAND			"⊼"
# define DBG_UAL_ADD			"+"
# define DBG_UAL_SUB			"-"

# define MSG_ERR_TTY				"Erreur lors de la réinitialisation de l'entrée.\n"
# define MSG_ERR_OPTION_INVALID		"Option %s invalide : %s\n"
# define MSG_ERR_INPUT_BUFFER_BIG	"Erreur : buffer d'entrée trop grand (%s), maximum %d octets.\n"
# define MSG_ERR_INPUT_FILE_HEX		"Erreur : fichier d'entrée non .hex : %s\n"
# define MSG_ERR_PROG_RANGE_OPTIONS	"Erreur : -a/--adresse et -t/--taille doivent être fournies ensemble.\n"
# define MSG_OPT_PROG_ADRESSE		"Adresse de départ fournie : %02X\n"
# define MSG_OPT_PROG_TAILLE		"Taille du programme fournie : %02X\n"
# define MSG_OPT_ADRESSE			"-a/--adresse"
# define MSG_OPT_TAILLE				"-t/--taille"

# define MSG_ERR_MEMORY			"Erreur de mémoire : %02X - Arrêt du programme\n "
# define MSG_ERR_INSTRUCTION	"Instruction inconnue : %02X - Arrêt du programme\n"
# define MSG_ERR_CORE			"Défaillance du cœur - Arrêt du programme\n    "
# define MSG_ERR_INCONNUE		"Erreur inconnue : code %d - Arrêt du programme\n "

# define MSG_SIG_INT					"Programme interrompu par CTRL+C.\n"
# define MSG_SIG_TERM					"Programme interrompu par SIGTERM.\n"
# define MSG_SIG_QUIT					"Programme interrompu par CTRL+\\.\n"
# define MSG_SIG_HUP					"Programme interrompu par fermeture du terminal.\n"
# define MSG_SIG_STP					"Programme interrompu par CTRL+Z.\n"
# define MSG_SIG_AUTRE					"Programme interrompu par un signal.\n"

#endif
