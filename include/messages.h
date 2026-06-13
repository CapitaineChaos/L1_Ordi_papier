/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\              Header des messages             /================

Auteur : Sylvain Maitre     24002886

Date de création :              03/10/2025
Date de dernière modification : 13/06/2026

Fichier     : mini_ordi.h
Description : Définitions des messages utilisés dans le programme

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


// ============================================================================
// Couleurs de fond et de textes pour l'unification de l'affichage du débogueur
# define MODE_CLAIR BGD_WH B_BK
# define FOND_CLAIR BGD_WH

# define MODE_FONCE BGD_BK B_WH
# define FOND_FONCE BGD_BK

# define MODE_ALERT BGD_RD B_WH
# define FOND_ALERT BGD_RD
// ============================================================================

# define MSG_TOUCHE	MODE_CLAIR  "         Appuyez sur une touche pour reprendre le cours du programme...         " RST


# define DBG_STATE_BOOTSTRAP_STDIN	MODE_CLAIR "Le bootstrap à lu une entrée depuis stdin" RST
# define DBG_STATE_PROGRAM_STDIN	MODE_CLAIR "Le programme à lu une entrée depuis stdin" RST
# define DBG_STATE_BOOTSTRAP_USER	MODE_CLAIR "Le bootstrap à lu une entrée utilisateur" RST
# define DBG_STATE_PROGRAM_USER		MODE_CLAIR "Le programme à lu une entrée utilisateur" RST
# define DBG_STATE_BOOTSTRAP_DEBUG	MODE_CLAIR "Buffer d'entrée vide : bascule en debug bootstrap" RST

# define DBG_STATE_MODE_INSTR		MODE_CLAIR "Bascule en mode Instruction (**)" RST
# define DBG_STATE_MODE_PHASE		MODE_CLAIR "Bascule en mode Phase (++)" RST
# define DBG_STATE_MODE_MICRO		MODE_CLAIR "Bascule en mode Microcode (--)" RST
# define DBG_MODE_INSTRUCTION		BGG_06 G_00 "Pico est en mode " BLD "Instruction" RST
# define DBG_MODE_PHASE				BGG_06 G_00 "   Pico est en mode " BLD "Phase   " RST
# define DBG_MODE_MICROCODE			BGG_06 G_00 " Pico est en mode " BLD "Microcode " RST
# define DBG_MODE_INCONNU			BGG_06 G_00 "  Pico est en mode " BLD "Inconnu  " RST

# define DBG_STATE_EXEC_MICRO		MODE_CLAIR "Execution du microcode %02d" RST
# define DBG_STATE_JUMP				MODE_CLAIR "PC placé à l'adresse " BLD "%02X" RST
# define DBG_STATE_UNKNOWN_CMD		MODE_CLAIR "Commande inconnue" RST
# define DBG_STATE_SMALL_TERM		"Affichage du debogueur indisponible :\n Agrandir pour atteindre le terminal minimum 80x27."
# define DBG_STATE_FOND				MODE_CLAIR "                                                                                " RST
# define DBG_HELP_TEXT				MODE_FONCE "%.*s" RST

# define MSG_ERR_TTY			"Erreur lors de la réinitialisation de l'entrée.\n"
# define MSG_ERR_OPTION_INVALID	"Option %s invalide : %s\n"
# define MSG_ERR_INPUT_BUFFER_BIG	"Erreur : buffer d'entrée trop grand (%s), maximum %d octets.\n"
# define MSG_ERR_INPUT_FILE_HEX	"Erreur : fichier d'entrée non .hex : %s\n"
# define MSG_ERR_PROG_RANGE_OPTIONS	"Erreur : -a/--adresse et -t/--taille doivent être fournies ensemble.\n"
# define MSG_OPT_PROG_ADRESSE	"Adresse de départ fournie : %02X\n"
# define MSG_OPT_PROG_TAILLE	"Taille du programme fournie : %02X\n"
# define MSG_OPT_ADRESSE		"-a/--adresse"
# define MSG_OPT_TAILLE			"-t/--taille"

# define MSG_ERR_MEMORY			FOND_ALERT BLK "  Erreur de mémoire : " BLD "%02X" RST FOND_ALERT BLK " - Arrêt du programme\n " RSTFG
# define MSG_ERR_INSTRUCTION	FOND_ALERT BLK "Instruction inconnue : " BLD "%02X" RST FOND_ALERT BLK " - Arrêt du programme\n" RSTFG
# define MSG_ERR_CORE			FOND_ALERT BLK "    Défaillance du cœur - Arrêt du programme\n    " RSTFG
# define MSG_ERR_INCONNUE		FOND_ALERT BLK " Erreur inconnue : code " BLD "%d" RST FOND_ALERT BLK " - Arrêt du programme\n " RSTFG


# define DBG_TOP_TITRE			MODE_FONCE BLD "                                   PICO v1.0                                    " RST
# define DBG_TOP_MENU(i, o)		"%s%s%s%s%s", FOND_FONCE "    Quitter " L_YL "[Q]" RSTFG "       Sortir " L_YL "[S]" RSTFG "       Aide " L_YL "[H]" RSTFG "       Buffers ", (i) ? L_YL : G_04, "[I]" RSTFG "/", (o) ? L_YL : G_04, "[O]" RST
# define DBG_UAL_NAND			"⊼"
# define DBG_UAL_ADD			"+"
# define DBG_UAL_SUB			"-"

// ligne de fond blanche
# define DBG_FOND_WH				MODE_CLAIR "                                                                                " RST
// ligne de fond noire
# define DBG_FOND_BK				MODE_FONCE "                                                                                " RST
// ligne de fond rouge
# define DBG_FOND_RD				MODE_ALERT "                                                                                " RST

# define LG_COLOR(bg,fg)			bg fg "                                                                                " RST

# define DBG_EMPTY_LINE				"                                                                                " RST
# define DBG_ENTETE_ADR				MODE_CLAIR  "Adr :" RST
# define DBG_ENTETE_MEM				MODE_CLAIR  "État de la mémoire :" RST
# define DBG_ENTETE_REG				MODE_CLAIR  "Registres :" RST

# define DBG_RG_HAUT				FOND_FONCE G_08 "┌──────────┐\n" RST
# define DBG_RG_MILIEU				FOND_FONCE G_08 "│          │\n" RST
# define DBG_RG_BAS					FOND_FONCE G_08 "└──────────┘\n" RST
# define DBG_RG_PC(M, PC)			MODE_FONCE "%s PC    %s%02X \n" RST, (M == 15 || M == 2) ? BGD_RD : (M == 1 ? BGD_GN : ""), BLD, PC
# define DBG_RG_RS(M, RS)			MODE_FONCE "%s RS    %s%02X \n" RST, (M == 1 || M == 7) ? BGD_RD : ((M == 13 || M == 14) ? BGD_CN : ""), BLD, RS
# define DBG_RG_RM(M, RM)			MODE_FONCE "%s RM    %s%02X \n" RST, (M == 4 || M == 8 || M == 13) ? BGD_RD : ((M == 2 || M == 3 || M == 5 || M == 6 || M == 9 || M == 12 || M == 14) ? BGD_GN : ""), BLD, RM
# define DBG_RG_AD(M, AD)			MODE_FONCE "%s AD    %s%02X \n" RST, M == 6 ? BGD_RD : (M == 7 ? BGD_GN : ""), BLD, AD
# define DBG_RG_OP(M, OP)			MODE_FONCE "%s OP    %s%02X \n" RST, M == 5 ? BGD_RD : "", BLD, OP
# define DBG_RG_UAL(M, UAL)			MODE_FONCE "%s UAL   %s%-2s \n" RST, (M == 10 || M == 11 || M == 17) ? BGD_RD : ( M == 12 ? BGD_CN : ""), BLD, UAL
# define DBG_RG_IN(M, IN)			MODE_FONCE "%s IN    %s%02X \n" RST, M == 16 ? BGD_RD : (M == 8 ? BGD_GN : ""), BLD, IN
# define DBG_RG_OUT(M, OUT)			MODE_FONCE "%s OUT   %s%02X \n" RST, M == 9 ? BGD_RD : "", BLD, OUT
# define DBG_RG_A(M, A)				MODE_FONCE "%s A     %s%02X \n" RST, (M == 3) ? BGD_RD : (M == 12 ? BG_MG : (M == 4 ? BGD_GN : "")), BLD, (u8)A
# define DBG_RG_Z(M, A)				MODE_FONCE "%s Z      %s%1d \n" RST, M == 19 ? (A == 0 ? BGD_GN : BGD_RD) : "", BLD, A == 0
# define DBG_RG_N(M, A)				MODE_FONCE "%s N      %s%1d \n" RST, M == 20 ? ((u8)A < 0 ? BGD_GN : BGD_RD) : "", BLD, A < 0

# define DBG_AFF_ADR(i)							MODE_FONCE "0x%02X " RST, (i)
# define DBG_AFF_MEM(i, val, PC, RS, M)			MODE_FONCE " %s%02X%s",  (M == 13 && i == RS) ? RST BGD_GN : ((M == 14 && i == RS) ? RST BGD_RD : (i == PC ? RST BG_CN B_BK : "")), val, RST

# define DBG_MICROCODE(fg, m)	MODE_FONCE " %s%d%s", (fg) == true ? BG_YL B_BK BLD : "" , m, RST
# define DBG_PHASE(f)			BGG_06 G_00 "Phase " BLD "%d" RST, (f)
# define DBG_MICROCODE_P(mp)	MODE_FONCE "Microcode précédent : " BG_MG BLD "%d" RST, (mp)
# define DBG_MICROCODE_S(mp)	MODE_FONCE "%s            │ suivants :" RST, (mp) < 10 ? " " : ""
# define DBG_LMICROCODE(m, s)	MODE_FONCE "Microcode à venir   : " BLD "%-2d" RSTFG "            │  %s\n" RST, m, s
# define DBG_LINSTRUCTION(i)	MODE_FONCE "Instruction à venir : " "%s" RST, INS
# define DBG_CMD_LIST			MODE_FONCE "Commandes : " L_BL "ENTER" RSTFG " " L_BL "*" RSTFG " " L_BL "**" RSTFG " " L_BL "+" RSTFG " " L_BL "++" RSTFG " " L_BL "-" RSTFG " " L_BL "--" RSTFG " " L_BL "m" RSTFG " " L_BL "j" RST
# define DBG_INVITE(err)		MODE_FONCE "%s$" RSTFG " ", (err) ? B_RD : ""


# define STR_MIC_1(m)			m, 51, "(RS) ← (PC)"
# define STR_MIC_2(m)			m, 51, "(PC) ← (RM)"
# define STR_MIC_3(m)			m, 51, "(A) ← (RM)"
# define STR_MIC_4(m)			m, 51, "(RM) ← (A)"
# define STR_MIC_5(m)			m, 51, "(OP) ← (RM)"
# define STR_MIC_6(m)			m, 51, "(AD) ← (RM)"
# define STR_MIC_7(m)			m, 51, "(RS) ← (AD)"
# define STR_MIC_8(m)			m, 51, "(RM) ← (Entrée)"
# define STR_MIC_9(m)			m, 51, "(Sortie) ← (RM)"
# define STR_MIC_10(m)			m, 51, "(UAL) ← Opération +"
# define STR_MIC_11(m)			m, 51, "(UAL) ← Opération -"
# define STR_MIC_17(m)			m, 51, "(UAL) ← Opération ⊼"
# define STR_MIC_12(m)			m, 51, "(A) ← (A) OP_UAL (RM)"
# define STR_MIC_13(m)			m, 51, "(RM) ← mem[(RS)]"
# define STR_MIC_14(m)			m, 51, "mem[(RS)] ← (RM)"
# define STR_MIC_15(m)			m, 51, "(PC) ← (PC) + 1"
# define STR_MIC_16(m)			m, 51, "Attente entrée"
# define STR_MIC_18(m)			m, 51, "Retour phase 1"
# define STR_MIC_19(m)			m, 51, "Continuer si (A) < 0"
# define STR_MIC_20(m)			m, 51, "Continuer si (A) == 0"
# define STR_MIC_INC(m)			m, 51, " - "

# define STR_ADD_IMM(i, a)			i, 51, "ADD    # %02X   │  A ← A + %02X", a, a
# define STR_ADD_ABS(i, a)			i, 51, "ADD    α %02X   │  A ← A + mem[%02X]", a, a
# define STR_ADD_IND(i, a, b)		i, 51, "ADD   *α %02X   │  A ← A + mem[%02X]", a, b
# define STR_SUB_IMM(i, a)			i, 51, "SUB    # %02X   │  A ← A - %02X", a, a
# define STR_SUB_ABS(i, a)			i, 51, "SUB    α %02X   │  A ← A - mem[%02X]", a, a
# define STR_SUB_IND(i, a, b)		i, 51, "SUB   *α %02X   │  A ← A - mem[%02X]", a, b
# define STR_NAND_IMM(i, a)			i, 51, "NAND   # %02X   │  A ← ~(A & %02X)", a, a
# define STR_NAND_ABS(i, a)			i, 51, "NAND   α %02X   │  A ← ~(A & mem[%02X])", a, a
# define STR_NAND_IND(i, a, b)		i, 51, "NAND  *α %02X   │  A ← ~(A & mem[%02X])", a, b
# define STR_LOAD_IMM(i, a)			i, 51, "LOAD   # %02X   │  A ← %02X", a, a
# define STR_LOAD_ABS(i, a)			i, 51, "LOAD   α %02X   │  A ← mem[%02X]", a, a
# define STR_LOAD_IND(i, a, b)		i, 51, "LOAD  *α %02X   │  A ← mem[%02X]", a, b
# define STR_STORE_ABS(i, a)		i, 51, "STORE  α %02X   │  mem[%02X] ← A", a, a
# define STR_STORE_IND(i, a, b)		i, 51, "STORE *α %02X   │  mem[%02X] ← A", a, b
# define STR_IN_ABS(i, a)			i, 51, "IN     α %02X   │  mem[%02X] ← Saisie utilisateur", a, a
# define STR_IN_IND(i, a, b)		i, 51, "IN    *α %02X   │  mem[%02X] ← Saisie utilisateur", a, b
# define STR_OUT_ABS(i, a)			i, 51, "OUT    α %02X   │  Sortie ← mem[%02X]", a, a
# define STR_OUT_IND(i, a, b)		i, 51, "OUT   *α %02X   │  Sortie ← mem[%02X]", a, b
# define STR_JUMP_ABS(i, a)			i, 51, "JUMP   α %02X   │  Sauter à l'adresse %02X", a, a
# define STR_BRN_ABS(i, a)			i, 51, "BRN    α %02X   │  Si N, sauter à %02X", a, a
# define STR_BRZ_ABS(i, a)			i, 51, "BRZ    α %02X   │  Si Z, sauter à %02X", a, a
# define STR_INCONNUE(i)			i, 51, " - "

# define MSG_STDIN_RESTANT(len, pos)	MODE_CLAIR "Buffer stdin restant : %zu octets" RST, (len) - (pos)
# define MSG_STDIN_ASC 					MODE_CLAIR "ASCII" RST
# define MSG_STDIN_IGNORE(len, pos)		MODE_CLAIR "... %zu octets non affichés" RST, (len) - (pos)
# define MSG_OUTPUT_TOTAL(len)			MODE_CLAIR "Buffer output : %zu octets" RST, (len)
# define MSG_OUTPUT_IGNORE(len, pos)	MODE_CLAIR "... %zu octets non affichés" RST, (len) - (pos)
# define STDIN_ADR(pos)					MODE_FONCE "%04zX:" RST, pos
# define STDIN_OCTET(c)					MODE_FONCE "%02X" RST, c
# define STDIN_ASC(c)					MODE_FONCE "%c" RST, c
# define STDIN_NON_PRINT(c)				MODE_FONCE "." RST

# define MSG_SIG_INT					"Programme interrompu par CTRL+C.\n"
# define MSG_SIG_TERM					"Programme interrompu par SIGTERM.\n"
# define MSG_SIG_QUIT					"Programme interrompu par CTRL+\\.\n"
# define MSG_SIG_HUP					"Programme interrompu par fermeture du terminal.\n"
# define MSG_SIG_STP					"Programme interrompu par CTRL+Z.\n"
# define MSG_SIG_AUTRE					"Programme interrompu par un signal.\n"

#endif
