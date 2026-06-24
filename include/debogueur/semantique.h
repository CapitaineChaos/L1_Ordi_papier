/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\      Sémantique microcodes/instructions      /================

Auteur : Sylvain Maitre     24002886

Date de création :              20/06/2026
Date de dernière modification : 22/06/2026

Fichier     : debogueur/semantique.h
Description : Chaînes décrivant la sémantique des microcodes et des instructions

Ces macros sont des arguments prêts pour snprintf : (buffer, taille, format[, args]).
Texte brut, sans formatage ANSI. Utilisées par debogueur/decodage.c.

==============================================================================*/

#ifndef DBG_SEMANTIQUE_H
# define DBG_SEMANTIQUE_H

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

#endif
