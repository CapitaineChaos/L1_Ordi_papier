/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\              Entrées / Sorties               /================

Auteur : Sylvain Maitre     24002886

Date de création :              01/10/2025
Date de dernière modification : 14/07/2026

Fichier     : io/io.c
Description : Fonctions d'entrées/sorties du mini-ordinateur

==============================================================================*/

#include "interne.h"
#include "io.h"
#include "messages.h"
#include "pico.h"
#include <ctype.h>
#include <stdio.h>

/**
 * @brief Indique si l'affichage verbeux des entrées/sorties doit être effectué
 * @note Les E/S du bootstrap sont masquées sauf si option -b active
 * @param pico Le mini-ordinateur
 * @param PC Le compteur de programme au moment de l'E/S
 * @return true si le message verbeux doit être affiché
 */
static bool	io_verbeux_actif(Mini_ordi *pico, u8 PC) {
	return (pico->modes.verbeux && (pico->modes.bootstrap || PC >= 32));
}

/**
 * @brief Affiche un octet en sortie
 * @param pico Le mini-ordinateur
 * @param val L'octet à afficher
 * @todo Ajouter un avertissement de dépassement du buffer de sortie dans chaque mode
 */
void	afficher_sortie(Mini_ordi *pico, u8 val) {
	// Ajouter l'octet au buffer de sortie si possible
	// Pas d'avertissement si c'est full, on laisse tomber
	if (pico->IO.output_len < PICO_INPUT_BUFFER_SIZE)
		pico->IO.output[pico->IO.output_len++] = (char)val;
	if (pico->modes.debogage)
		return;
	// Mode ASCII (-p) : on n'affiche que les caractères imprimables, le reste est ignoré
	if (pico->modes.mode_ascii && !isprint(val))
		return;
	// Préciser l'affichage pour le mode verbeux si activé
	if (io_verbeux_actif(pico, pico->PC))
		printf("\nSortie : ");
	// L'ASCII (-p) prime ; sinon hexadécimal si -x, décimal par défaut
	if (pico->modes.mode_ascii)
		printf("%c", val);
	else if (pico->modes.mode_hexa)
		printf("%02X", val);
	else
		printf("%d", val);
	// Saut de ligne entre les sorties, sauf en mode -n (un seul à la fin)
	if (!pico->modes.sans_saut_ligne)
		printf("\n");
}

/**
 * @brief Lit un octet en entrée de manière classique (Hors du débogueur)
 * @param pico Le mini-ordinateur
 * @param PC Le compteur de programme
 * @return L'octet lu
 */
static u8	lire_entree_classique(Mini_ordi *pico, u8 PC) {
	u8		val = 0;
	FILE	*stream;

	if (io_verbeux_actif(pico, PC))
		printf("\nEntrée (PC=%02X) : ", PC);
	if (IO_STDIN_AVAILABLE) {
		val = (unsigned char)pico->IO.buffer[pico->IO.buffer_pos++];
		if (io_verbeux_actif(pico, PC))
			printf("buffer stdin -> %02X", val);
		return (val);
	}
	msg_print_input_prompt(&pico->modes, PC);
	stream = io_flux_entree_utilisateur(pico);
	// Ici l'entrée se fait au clavier : décimal par défaut, hexa si -x
	{
		unsigned long	nb = 0;

		if (pico->modes.mode_hexa) {
			while (!io_lire_hexa_clavier(stream, &nb))
				continue;
			// Trop grand : modulo 256
			if (nb > 255 && io_verbeux_actif(pico, PC))
				printf(MSG_HEX_MODULO, nb, (unsigned)(nb % 256));
		}
		else {
			while (!io_lire_decimal_clavier(stream, &nb))
				continue;
			if (nb > 255 && io_verbeux_actif(pico, PC))
				printf(MSG_DEC_MODULO, nb, (unsigned)(nb % 256));
		}
		val = (u8)(nb % 256);
	}
	return (val);
}

/**
 * @brief Lit un octet en entrée
 * @param pico Le mini-ordinateur
 * @param PC Le compteur de programme
 * @return L'octet lu
 */
u8	lire_entree(Mini_ordi *pico, u8 PC) {
	if (pico->modes.debogage)
		return (io_lire_entree_debug(pico));
	return (lire_entree_classique(pico, PC));
}
