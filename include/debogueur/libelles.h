/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\         Libellés et style du débogueur       /================

Auteur : Sylvain Maitre     24002886

Date de création :              20/06/2026
Date de dernière modification : 20/06/2026

Fichier     : debogueur/libelles.h
Description : Chaînes, couleurs et gabarits de l'interface du débogueur

==============================================================================*/

#ifndef DBG_LIBELLES_H
# define DBG_LIBELLES_H

# include "ansi.h"

// ============================================================================
// Couleurs de l'interface
// ============================================================================
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

# define MSG_ERR_MEMORY_D		FOND_ALERT BLK "  Erreur de mémoire : " BLD "%02X" RST FOND_ALERT BLK " - Arrêt du programme\n " RSTFG
# define MSG_ERR_INSTRUCTION_D	FOND_ALERT BLK "Instruction inconnue : " BLD "%02X" RST FOND_ALERT BLK " - Arrêt du programme\n" RSTFG
# define MSG_ERR_CORE_D			FOND_ALERT BLK "    Défaillance du cœur - Arrêt du programme\n    " RSTFG
# define MSG_ERR_INCONNUE_D		FOND_ALERT BLK " Erreur inconnue : code " BLD "%d" RST FOND_ALERT BLK " - Arrêt du programme\n " RSTFG

# define DBG_TOP_TITRE			MODE_FONCE BLD "                                   PICO v1.0                                    " RST
# define DBG_TOP_MENU(i, o)		"%s%s%s%s%s", FOND_FONCE "    Quitter " L_YL "[Q]" RSTFG "       Sortir " L_YL "[S]" RSTFG "       Aide " L_YL "[H]" RSTFG "       Buffers ", (i) ? L_YL : G_04, "[I]" RSTFG "/", (o) ? L_YL : G_04, "[O]" RST

# define DBG_FOND_WH				MODE_CLAIR "                                                                                " RST
# define DBG_FOND_BK				MODE_FONCE "                                                                                " RST
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

# define MSG_STDIN_RESTANT(len, pos)	MODE_CLAIR "Buffer stdin restant : %zu octets" RST, (len) - (pos)
# define MSG_STDIN_ASC 					MODE_CLAIR "ASCII" RST
# define MSG_STDIN_IGNORE(len, pos)		MODE_CLAIR "... %zu octets non affichés" RST, (len) - (pos)
# define MSG_OUTPUT_TOTAL(len)			MODE_CLAIR "Buffer output : %zu octets" RST, (len)
# define MSG_OUTPUT_IGNORE(len, pos)	MODE_CLAIR "... %zu octets non affichés" RST, (len) - (pos)
# define STDIN_ADR(pos)					MODE_FONCE "%04zX:" RST, pos
# define STDIN_OCTET(c)					MODE_FONCE "%02X" RST, c
# define STDIN_ASC(c)					MODE_FONCE "%c" RST, c
# define STDIN_NON_PRINT(c)				MODE_FONCE "." RST

#endif
