#===============/                    cx25.1                    \================
#=~~~~~~~~~~~~=|               Ordinateur en papier             |=~~~~~~~~~~~~~=
#===============\              Makefile standard L1            /================
#
# Auteur : Sylvain Maitre     24002886
#
# Date de création :              01/10/2025
# Date de dernière modification : 05/10/2025
#
# Description : Makefile standard pour le projet cx25.1
# Utilisation : make [cible]
# Exemple     : make all
# Aide        : make help
#
#===============================================================================

NAME = pico

include Makefile.config

# Fichiers
TARGET = $(BINDIR)/$(NAME)
SOURCES = $(shell find $(SRCDIR) -name '*.c')
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)
DEPENDS = $(OBJECTS:.o=.d)

.PHONY: all $(NAME) clean distclean help re usage

# Règle par défaut
all: $(TARGET)

# Règle pour l'exécutable principal (alias)
$(NAME): all

# Créer le ou les dossiers nécessaires
$(BUILDDIR) $(BINDIR):
	@mkdir -p $@
	@printf "${BOLD}%s${RESET} : ${B_BL}%s${RESET}\n" "Création du dossier" "$(notdir $@)"

# Règle pour l'exécutable principal (linkage)
# Relink si des fichiers objets ont changé
# Dépendance simple sans relink si dossier bin manquant
$(TARGET): $(OBJECTS) | $(BINDIR)
	@printf "${BOLD}%s${RESET} : ${BPINK}%s${RESET}\n" "Édition des liens pour l'exécutable" "$(notdir $@)"
	@$(CC) -o $@ $(LDFLAGS) $(OBJECTS) $(LDLIBS)

# Règle pour les fichiers objets du code source (compilation)
# Recompilation si un fichier source a changé
# Dépendance simple sans recompilation si dossier build manquant
$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	@printf "${BOLD}%s${RESET} : ${BCYAN}%s${RESET}\n" "Compilation" "$(notdir $<)"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INC) -c -o $@ $<

# Règle pour le nettoyage des fichiers générés
# Pas de suppression des dossiers build et bin
clean:
	@printf "${BOLD}%s${RESET} : ${B_YL}%s${RESET}\n" "Nettoyage en cours" "fichiers objets et dépendances..."
	@rm -rf $(BUILDDIR)/*

# Règle pour un nettoyage complet (distclean)
# Suppression des dossiers build et bin
distclean: clean
	@printf "${BOLD}%s${RESET} : ${B_RD}%s${RESET}\n" "Nettoyage en cours" "environnement..."
	@rm -rf $(BINDIR)
	@rm -rf $(BUILDDIR)

# Règle pour une recompilation complète
# Nettoyage complet puis compilation complète
re: distclean all

# Inclusion des dépendances automatiques
# Rend la compilation incrémentale possible et plus fiable et efficace
-include $(DEPENDS)

# Règles de tests
test1: $(TARGET)
	@printf "${BOLD}%s${RESET} : ${B_GN}%s${RESET}\n" "Exécution du test 1"
	@$(TARGET) < exemples/prog.hex

test2: $(TARGET)
	@printf "${BOLD}%s${RESET} : ${B_GN}%s${RESET}\n" "Exécution du test 2, option -b"
	@$(TARGET) -b < exemples/prog.hex

test3: $(TARGET)
	@printf "${BOLD}%s${RESET} : ${B_GN}%s${RESET}\n" "Exécution du test 3, option -d"
	@$(TARGET) -d < exemples/prog.hex

test4: $(TARGET)
	@printf "${BOLD}%s${RESET} : ${B_GN}%s${RESET}\n" "Exécution du test 4, option -db"
	@$(TARGET) -db < exemples/prog.hex

usage:
	@echo ""
	@echo "${BPINK}Exemples d'utilisation :${RESET}"
	@echo "          ./$(NAME) < fichier.hex"
	@echo "          echo \"8007000548324132FF\" | ./$(NAME)"



# Aide
help:
	@echo ""
	@echo "${BPINK}Cibles disponibles :${RESET}"
	@echo "  ${BCYAN}${NAME}${RESET}   - Compile le programme"
	@echo "  ${BCYAN}all${RESET}       - Compile le programme"
	@echo "  ${BCYAN}clean${RESET}     - Supprime les fichiers objets et dépendances ($(BUILDDIR)/)"
	@echo "  ${BCYAN}distclean${RESET} - Supprime tous les fichiers générés ($(BUILDDIR)/ et $(BINDIR)/)"
	@echo "  ${BCYAN}re${RESET}        - Recompilation complète (distclean + all)"
	@echo "  ${BCYAN}help${RESET}      - Affiche cette aide"
	@echo "  ${BCYAN}usage${RESET}     - Affiche l'utilisation du programme"
	@echo ""
	@echo "${BPINK}Structure du projet :${RESET}"
	@echo "  ${BCYAN}$(SRCDIR)/${RESET}     - Code source (.c, .h)"
	@echo "  ${BCYAN}$(BUILDDIR)/${RESET}   - Fichiers objets (.o) et dépendances (.d)"
	@echo "  ${BCYAN}$(BINDIR)/${RESET}     - Exécutables"
	@echo ""

