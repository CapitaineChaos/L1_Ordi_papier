#===============/                  Makefile.mk                  \===============
#=~~~~~~~~~~~~=|         Fichier de configuration commune        |=~~~~~~~~~~~~=
#===============\             Configuration Makefile            /===============
#
# Auteur : Sylvain Maitre 24002886
#
# Date de création 				: 20/06/2026
# Date de dernière modification : 20/06/2026
#
# Description : Configuration de tests
#
#=============================================================================*/

# .SHELLFLAGS avec -e préserve l'arrêt-sur-erreur ligne par ligne dans toutes les
# recettes, y compris les règles de compilation (sinon .ONESHELL peut masquer une
# erreur intermédiaire).
.ONESHELL:
.SHELLFLAGS := -ec

.PHONY: t1 t2 t3 t4 t5 t6 t7 t8 t9

# Tests
t1: $(TARGET)
	@printf "${BOLD}%s${RESET} : ${B_GN}%s${RESET}\n" "Exécution du test 1 / standard input"
	@$(TARGET) < exemples/simple.hex  | xxd -r -ps; echo

t2: $(TARGET)
	@printf "${BOLD}%s${RESET} : ${B_GN}%s${RESET}\n" "Exécution du test 2, option -b / standard input"
	@$(TARGET) -b < exemples/simple.hex

t3: $(TARGET)
	@printf "${BOLD}%s${RESET} : ${B_GN}%s${RESET}\n" "Exécution du test 3, option -d / standard input"
	@$(TARGET) -d < exemples/simple.hex

t4: $(TARGET)
	@printf "${BOLD}%s${RESET} : ${B_GN}%s${RESET}\n" "Exécution du test 4, option -db / standard input"
	@$(TARGET) -db < exemples/simple.hex

t5: $(TARGET)
	@printf "${BOLD}%s${RESET} : ${B_GN}%s${RESET}\n" "Exécution du test 5, option -d / here document"
	@$(TARGET) -d <<-'EOF'
	80 ; adresse de départ
	1F ; longueur
	00 48
	48 70
	41 70
	00 65
	48 70
	41 70
	00 6C
	48 70
	41 70
	00 6C
	48 70
	41 70
	00 6F
	48 70
	41 70
	FF
	EOF

t6: $(TARGET)
	@printf "${BOLD}%s${RESET} : ${B_GN}%s${RESET}\n" "Exécution du test 6, option -d -a 0x80 -t 0x1F / here document"
	@$(TARGET) -d -a 0x80 -t 0x1F <<-'EOF'
	00 48
	48 70
	41 70
	00 65
	48 70
	41 70
	00 6C
	48 70
	41 70
	00 6C
	48 70
	41 70
	00 6F
	48 70
	41 70
	FF
	EOF

t7: $(TARGET)
	@printf "${BOLD}%s${RESET} : ${B_GN}%s${RESET}\n" "Exécution du test 7, option -d -a 128 -t 31 / here document"
	@$(TARGET) -d -a 0x80 -t 0x1F <<-'EOF'
	00 48
	48 70
	41 70
	00 65
	48 70
	41 70
	00 6C
	48 70
	41 70
	00 6C
	48 70
	41 70
	00 6F
	48 70
	41 70
	FF
	EOF

t8: $(TARGET)
	@printf "${BOLD}%s${RESET} : ${B_GN}%s${RESET}\n" "Exécution du test 8, option -df / fichier d'entrée"
	@$(TARGET) -df exemples/simple.hex

t9: $(TARGET)
	@printf "${BOLD}%s${RESET} : ${B_GN}%s${RESET}\n" "Exécution du test 9, option -d / pipe d'entrée"
	@cat exemples/simple.hex | $(TARGET) -d

