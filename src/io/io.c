/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\              Entrées / Sorties               /================

Auteur : Sylvain Maitre     24002886

Date de création :              01/10/2025
Date de dernière modification : 20/06/2026

Fichier     : io/io.c
Description : Fonctions d'entrées/sorties du mini-ordinateur

==============================================================================*/

#include "interne.h"
#include "io.h"
#include "messages.h"
#include "pico.h"
#include <stdio.h>

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
	// Préciser l'affichage pour le mode verbeux si activé
	if (pico->modes.verbeux)
		printf("\nSortie : ");
	msg_print_hex(val);
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

	if (pico->modes.verbeux)
		printf("\nEntrée (PC=%02X) : ", PC);
	if (IO_STDIN_AVAILABLE) {
		val = (unsigned char)pico->IO.buffer[pico->IO.buffer_pos++];
		if (pico->modes.verbeux)
			printf("buffer stdin -> %02X\n", val);
		return (val);
	}
	msg_print_input_prompt(&pico->modes);
	stream = io_flux_entree_utilisateur(pico);
	// Ici l'entrée devrait se fait au clavier
	while (!io_lire_octet_hex(stream, &val, true))
		continue;
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
