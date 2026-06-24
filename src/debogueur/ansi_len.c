/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\              Helpers ANSI debug              /================

Auteur : Sylvain Maitre     24002886

Date de création :              11/06/2026
Date de dernière modification : 22/06/2026

Fichier     : debogueur/ansi_len.c
Description : Mesures de chaînes contenant des séquences ANSI

Problème    : strlen() compte aussi les caractères invisibles des couleurs.


==============================================================================*/

#include "debogueur/ansi_len.h"
#include "ansi.h"
#include <string.h>

/**
 * @brief Calcule la longueur d'une séquence ANSI dans une chaîne de caractères
 * @param str La chaîne de caractères contenant la séquence ANSI
 * @return La longueur de la séquence ANSI
 */
size_t	ansi_sequence_len(const char *str) {
	size_t	len;

	if (str[0] != ANSI_ESC || str[1] != '[')
		return (0);
	len = 2 + strspn(str + 2, "0123456789;:?");
	if (str[len] != '\0')
		len++;
	return (len);
}

/**
 * @brief Ignore les séquences ANSI dans une chaîne de caractères à partir d'un index donné
 * @param str La chaîne de caractères contenant des séquences ANSI
 * @param i L'index à partir duquel commencer à ignorer les séquences ANSI
 * @return L'index après avoir ignoré les séquences ANSI
 */
static size_t	skip_ansi_sequences(const char *str, size_t i) {
	size_t	ansi_len;

	ansi_len = ansi_sequence_len(str + i);
	while (ansi_len > 0) {
		i += ansi_len;
		ansi_len = ansi_sequence_len(str + i);
	}
	return (i);
}

/**
 * @brief Calcule la longueur visible d'une chaîne de caractères en ignorant les séquences ANSI
 * @param str La chaîne de caractères contenant des séquences ANSI
 * @return La longueur visible de la chaîne
 */
size_t	ansi_visible_len(const char *str) {
	size_t	len;
	size_t	i;
	size_t	next;

	len = 0;
	i = 0;
	while (str[i]) {
		next = skip_ansi_sequences(str, i);
		if (next != i) {
			i = next;
			continue;
		}
		len++;
		i++;
	}
	return (len);
}
