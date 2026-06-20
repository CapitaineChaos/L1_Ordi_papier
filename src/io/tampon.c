/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\              Buffer des entrées              /================

Auteur : Sylvain Maitre     24002886

Date de création :              12/06/2026
Date de dernière modification : 13/06/2026

Fichier     : io/tampon.c
Description : Chargement et lecture des flux d'entrée

==============================================================================*/

#include "interne.h"
#include "messages.h"
#include "pico.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * @brief Convertit une chaîne hexadécimale en octet
 * @param hex La chaîne hexadécimale (2 caractères)
 * @param val Pointeur vers l'octet résultant
 * @return true si la conversion a réussi, false sinon
 */
bool	io_parse_hex_byte(const char *hex, u8 *val) {
	char	tmp[3];

	if (!hex || !val || !isxdigit((unsigned char)hex[0])
		|| !isxdigit((unsigned char)hex[1]))
		return (false);
	tmp[0] = hex[0];
	tmp[1] = hex[1];
	tmp[2] = '\0';
	*val = (u8)strtoul(tmp, NULL, 16);
	return (true);
}

/**
 * @brief Convertit une chaîne décimale en octet
 * @param dec La chaîne décimale (1 à 3 chiffres)
 * @param val Pointeur vers l'octet résultant
 * @return true si la conversion a réussi et tient sur 8 bits, false sinon
 */
bool	io_parse_dec_byte(const char *dec, u8 *val) {
	unsigned long	nb;
	char			*fin;

	if (!dec || !val || !isdigit((unsigned char)dec[0]))
		return (false);
	nb = strtoul(dec, &fin, 10);
	if (*fin != '\0' || nb > 255)
		return (false);
	*val = (u8)nb;
	return (true);
}

/**
 * @brief Lit un octet décimal saisi au clavier (terminé par Entrée)
 * @param stream Le flux d'entrée
 * @param val Pointeur vers l'octet résultant
 * @return true si la lecture a réussi, false sinon
 * @note Seule la saisie clavier utilise le décimal ; le stdin reste hexa.
 */
bool	io_lire_octet_dec(FILE *stream, u8 *val) {
	char	dec[4];
	int		i;
	int		c;

	i = 0;
	while ((c = fgetc(stream)) != EOF && c != '\n') {
		if (isdigit(c) && i < 3)
			dec[i++] = (char)c;
	}
	dec[i] = '\0';
	if (c == EOF && i == 0)
		return (false);
	return (io_parse_dec_byte(dec, val));
}

/**
 * @brief Lit un octet hexadécimal à partir d'un flux
 * @param stream Le flux d'entrée
 * @param val Pointeur vers l'octet résultant
 * @return true si la lecture a réussi, false sinon
 */
bool	io_lire_octet_hex(FILE *stream, u8 *val, bool clavier) {
	bool	commentaire;
	char	hex[3];
	int		i;
	int		c;

	commentaire = false;
	i = 0;
	hex[0] = '\0';
	hex[1] = '\0';
	hex[2] = '\0';
	while (i < 2) {
		c = fgetc(stream);
		if (c == EOF)
			return (false);
		if (c == '\n')
		{
			if (clavier && i > 0) {
				hex[1] = hex[0];
				hex[0] = '0';
				return (io_parse_hex_byte(hex, val));
			}
			commentaire = false;
		}
		if (c == ';' || c == '#')
			commentaire = true;
		if (commentaire)
			continue;
		if (isxdigit(c)) {
			hex[i++] = c;
		}
	}
	return (io_parse_hex_byte(hex, val));
}

/**
 * @brief Affiche une erreur si le buffer d'entrée est trop grand
 * @param source La source de l'entrée
 */
static void	erreur_buffer_entree_trop_grand(const char *source) {
	fprintf(stderr, MSG_ERR_INPUT_BUFFER_BIG, source, PICO_INPUT_MAX_BYTES);
	exit(EXIT_FAILURE);
}

/**
 * @brief Ajoute un octet au buffer d'entrée
 * @param pico Le mini-ordinateur
 * @param val L'octet à ajouter
 * @param source La source de l'entrée
 */
static void	ajouter_octet_buffer_entree(Mini_ordi *pico, u8 val,
	const char *source) {
	if (pico->IO.buffer_len >= PICO_INPUT_MAX_BYTES)
		erreur_buffer_entree_trop_grand(source);
	pico->IO.buffer[pico->IO.buffer_len++] = (char)val;
}

/**
 * @brief Ajoute les octets d'un flux au buffer d'entrée
 * @param pico Le mini-ordinateur
 * @param stream Le flux d'entrée
 * @param source La source de l'entrée
 */
static void	ajouter_flux_buffer_entree(Mini_ordi *pico, FILE *stream,
	const char *source) {
	u8	val;

	while (io_lire_octet_hex(stream, &val, false)) {
		ajouter_octet_buffer_entree(pico, val, source);
	}
	if (ferror(stream)) {
		perror(source);
		exit(EXIT_FAILURE);
	}
}


/**
 * @brief Vérifie si l'extension d'un fichier est .hex
 * @param path Le chemin du fichier
 * @return true si l'extension est .hex, false sinon
 */
static bool	extension_hex_valide(const char *path) {
	size_t	len;

	if (!path)
		return (false);
	len = strlen(path);
	return (len > 4 && strcmp(path + len - 4, ".hex") == 0);
}


/**
 * @brief Ajoute un fichier au buffer d'entrée
 * @param pico Le mini-ordinateur
 */
static void	ajouter_fichier_buffer_entree(Mini_ordi *pico) {
	FILE	*file;

	if (!pico->IO.input_file)
		return;
	if (!extension_hex_valide(pico->IO.input_file)) {
		fprintf(stderr, MSG_ERR_INPUT_FILE_HEX, pico->IO.input_file);
		exit(EXIT_FAILURE);
	}
	file = fopen(pico->IO.input_file, "r");
	if (!file) {
		perror(pico->IO.input_file);
		exit(EXIT_FAILURE);
	}
	ajouter_flux_buffer_entree(pico, file, pico->IO.input_file);
	fclose(file);
}


/**
 * @brief Ajoute les options du programme au buffer d'entrée
 * @param pico Le mini-ordinateur
 */
static void	ajouter_options_prog_buffer_entree(Mini_ordi *pico) {
	if (!pico->prog_params)
		return;
	ajouter_octet_buffer_entree(pico, pico->prog_debut, MSG_OPT_ADRESSE);
	ajouter_octet_buffer_entree(pico, pico->prog_taille, MSG_OPT_TAILLE);
	if (pico->modes.verbeux) {
		printf(MSG_OPT_PROG_ADRESSE, pico->prog_debut);
		printf(MSG_OPT_PROG_TAILLE, pico->prog_taille);
	}
}

/**
 * @brief Charge le buffer d'entrée avec stdin et le fichier -f
 * @param pico Le mini-ordinateur
 * @note Si stdin est un terminal, il sera lu en mode interactif.
 */
void	charger_buffer_entree(Mini_ordi *pico) {
	pico->IO.stdin_is_tty = isatty(STDIN_FILENO);
	ajouter_options_prog_buffer_entree(pico);
	if (!pico->IO.stdin_is_tty)
		ajouter_flux_buffer_entree(pico, stdin, "stdin");
	ajouter_fichier_buffer_entree(pico);
}

/**
 * @brief Ouvre le terminal pour l'entrée utilisateur
 * @return Le flux d'entrée du terminal
 */
static FILE	*ouvrir_tty(void) {
	FILE	*tty = fopen("/dev/tty", "r");

	if (!tty) {
		msg_perror_tty();
		exit(EXIT_FAILURE);
	}
	return (tty);
}


/**
 * @brief Obtient le flux d'entrée de l'utilisateur
 * @param pico Le mini-ordinateur
 * @return Le flux d'entrée de l'utilisateur
 * @note Si l'entrée standard est un terminal, retourne stdin. Sinon, ouvre /dev/tty.
 */
FILE	*io_flux_entree_utilisateur(Mini_ordi *pico) {
	static FILE	*tty = NULL;

	if (pico->IO.stdin_is_tty) {
		return (stdin);
	}
	if (!tty)
		tty = ouvrir_tty();
	return (tty);
}
