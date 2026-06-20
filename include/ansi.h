/*==============/                    cx25.1                    \================
=~~~~~~~~~~~~~=|              Ordinateur en papier              |=~~~~~~~~~~~~~=
================\              Header des couleurs             /================

Auteur : Sylvain Maitre     24002886

Date de création :              14/10/2025
Date de dernière modification : 20/06/2026

Fichier     : ansi.h
Description : Définitions des code ansi utilisées par les messages

==============================================================================*/

#ifndef ANSI_H
# define ANSI_H

# define ANSI_ESC '\033'

/** Contrôle du curseur */

# define D_HOME "\033[H"			    // Déplacer curseur en haut à gauche (HOME)
# define D_UP(n) "\033[" #n "A"		    // Déplacer curseur n lignes vers le haut
# define D_DOWN(n) "\033[" #n "B"	    // Déplacer curseur n lignes vers le bas
# define D_RIGHT(n) "\033[" #n "C"	    // Déplacer curseur n colonnes à droite
# define D_LEFT(n) "\033[" #n "D"	    // Déplacer curseur n colonnes à gauche
# define D_COL_FMT "\033[%zuG"		    // Déplacer curseur à la colonne %zu
# define D_LINE_COL_FMT "\033[%d;%zuH"	// Déplacer curseur ligne %d colonne %zu
# define D_LINE_START_FMT "\033[%d;1H"	// Déplacer curseur ligne %d colonne 1
# define D_SAVE "\033[s"			    // Sauvegarder position du curseur
# define D_RESTORE "\033[u"			    // Restaurer position du curseur
# define D_SHOW_CURSOR "\033[?25h"	    // Afficher le curseur
# define D_HIDE_CURSOR "\033[?25l"	    // Cacher le curseur
# define D_ALT_SCREEN "\033[?1049h"	    // Activer l'écran alternatif
# define D_MAIN_SCREEN "\033[?1049l"	// Restaurer l'écran principal
# define D_NO_WRAP "\033[?7l"		    // Désactiver le retour automatique à la ligne
# define D_WRAP "\033[?7h"			    // Activer le retour automatique à la ligne

/** Effacement */

# define E_LINE "\033[0K"             // Effacer du curseur à la fin de la ligne
# define E_LINE_START "\033[1K"       // Effacer du début de la ligne au curseur
# define E_LINE_ALL "\033[2K"         // Effacer toute la ligne
# define E_SCREEN "\033[0J"           // Effacer du curseur à la fin de l'écran
# define E_SCREEN_START "\033[1J"     // Effacer du début de l'écran au curseur
# define E_SCREEN_ALL "\033[2J"       // Effacer tout l'écran

/** Formattage */

# define BLD "\033[1m" // Gras (Bold)
# define DIM "\033[2m" // Dim/Faint
# define ITA "\033[3m" // Italique
# define UND "\033[4m" // Souligné
# define BLK "\033[5m" // Clignotant
# define INV "\033[7m" // Inversé (Reverse video)
# define HID "\033[8m" // Caché/Invisible
# define STR "\033[9m" // Barré (Strikethrough)

# define NOBLD "\033[21m" // Pas gras
# define NODIM "\033[22m" // Normal (ni gras ni dim)
# define NOITA "\033[23m" // Pas italique
# define NOUND "\033[24m" // Pas souligné
# define NOBLK "\033[25m" // Pas clignotant
# define NOINV "\033[27m" // Pas inversé
# define NOHID "\033[28m" // Pas caché
# define NOSTR "\033[29m" // Pas barré

# define BLINK    "\033[5m"
# define NOBLINK  "\033[25m"
# define RST      "\033[0m"  // Reseter tous les attributs
# define RSTBG    "\033[49m" // Reset arrière-plan seulement
# define RSTFG    "\033[39m" // Reset premier plan seulement

/** Couleurs d'arrière-plan "BRIGHT" */

# define BG_BK "\033[48;5;16m"
# define BG_RD "\033[48;5;196m"
# define BG_GN "\033[48;5;46m"
# define BG_YL "\033[48;5;226m"
# define BG_BL "\033[48;5;21m"
# define BG_CN "\033[48;5;51m"
# define BG_MG "\033[48;5;90m"
# define BG_WH "\033[48;5;15m"

/** Couleurs d'arrière-plan "DARK" */

# define BGD_BK "\033[48;5;235m"
# define BGD_RD "\033[48;5;124m"
# define BGD_GN "\033[48;5;34m"
# define BGD_YL "\033[48;5;136m"
# define BGD_BL "\033[48;5;19m"
# define BGD_CN "\033[48;5;23m"
# define BGD_MG "\033[48;5;96m"
# define BGD_WH "\033[48;5;250m"

/** Couleurs de premier plan "BRIGHT" */

# define B_BK "\033[38;5;16m"
# define B_RD "\033[38;5;196m"
# define B_GN "\033[38;5;46m"
# define B_YL "\033[38;5;226m"
# define B_BL "\033[38;5;21m"
# define B_MG "\033[38;5;201m"
# define B_CN "\033[38;5;51m"
# define B_WH "\033[38;5;15m"

/** Couleurs de premier plan "LIGHT" */
# define L_BK "\033[38;5;240m"
# define L_RD "\033[38;5;203m"
# define L_GN "\033[38;5;120m"
# define L_YL "\033[38;5;229m"
# define L_BL "\033[38;5;39m"
# define L_MG "\033[38;5;213m"
# define L_CN "\033[38;5;87m"
# define L_WH "\033[38;5;255m"

/** Couleurs de premier plan "GREY" */
# define G_00 "\033[38;5;232m"  // Sombre
# define G_01 "\033[38;5;234m"
# define G_02 "\033[38;5;236m"
# define G_03 "\033[38;5;238m"
# define G_04 "\033[38;5;240m"
# define G_05 "\033[38;5;242m"
# define G_06 "\033[38;5;244m"
# define G_07 "\033[38;5;246m"
# define G_08 "\033[38;5;248m"
# define G_09 "\033[38;5;250m"
# define G_10 "\033[38;5;252m"
# define G_11 "\033[38;5;254m"  // Clair

/** Couleurs d'arrière-plan "GREY" */
# define BGG_00 "\033[48;5;232m"  // Sombre
# define BGG_01 "\033[48;5;234m"
# define BGG_02 "\033[48;5;236m"
# define BGG_03 "\033[48;5;238m"
# define BGG_04 "\033[48;5;240m"
# define BGG_05 "\033[48;5;242m"
# define BGG_06 "\033[48;5;244m"
# define BGG_07 "\033[48;5;246m"
# define BGG_08 "\033[48;5;248m"
# define BGG_09 "\033[48;5;250m"
# define BGG_10 "\033[48;5;252m"
# define BGG_11 "\033[48;5;254m"  // Clair

/** Raccourcis  */

# define CLS D_HOME E_SCREEN

#endif
