#===============/                    cx25.1                    \================
#=~~~~~~~~~~~~=|               Ordinateur en papier             |=~~~~~~~~~~~~~=
#===============\              Makefile standard L1            /================
#
# Auteur : Sylvain Maitre     24002886
#
# Date de création :              01/10/2025
# Date de dernière modification : 20/06/2026
#
# Description : Makefile standard pour le projet cx25.1
# Utilisation : make [cible]
# Exemple     : make all
# Aide        : make help
#
#===============================================================================

NAME = pico

include Makefile.config.mk

# Fichiers
TARGET  = $(BINDIR)/$(NAME)
SOURCES = $(shell find $(SRCDIR) -name '*.c')
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)
DEPENDS = $(OBJECTS:.o=.d)

.PHONY: all $(NAME) clean distclean help re usage help


# Règle par défaut
all: $(TARGET)

# Exécutable principal (alias)
$(NAME): all

# Créer le ou les dossiers nécessaires
$(BUILDDIR) $(BINDIR):
	@mkdir -p $@
	@printf "${BOLD}%s${RESET} : ${B_BL}%s${RESET}\n" "Création du dossier" "$(notdir $@)"

# Exécutable principal (linkage)
# Relink si des fichiers objets ont changé
# Dépendance simple sans relink si dossier bin manquant
$(TARGET): $(OBJECTS) | $(BINDIR)
	@printf "${BOLD}%s${RESET} : ${BPINK}%s${RESET}\n" "Édition des liens pour l'exécutable" "$(notdir $@)"
	@$(CC) -o $@ $(LDFLAGS) $(OBJECTS) $(LDLIBS)

# Fichiers objets du code source (compilation)
# Recompilation si un fichier source a changé
# Dépendance simple sans recompilation si dossier build manquant
$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	@printf "${BOLD}%s${RESET} : ${BCYAN}%s${RESET}\n" "Compilation" "$(notdir $<)"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INC) -c -o $@ $<

# Nettoyage des fichiers générés
# Pas de suppression des dossiers build et bin
clean:
	@printf "${BOLD}%s${RESET} : ${B_YL}%s${RESET}\n" "Nettoyage en cours" "fichiers objets et dépendances..."
	@rm -rf $(BUILDDIR)/*

# Nettoyage complet (distclean)
# Suppression des dossiers build et bin
distclean: clean
	@printf "${BOLD}%s${RESET} : ${B_RD}%s${RESET}\n" "Nettoyage en cours" "environnement..."
	@rm -rf $(BINDIR)
	@rm -rf $(BUILDDIR)

# Recompilation complète
# Nettoyage complet puis compilation complète
re: distclean all

# Inclusion des dépendances automatiques
# Rend la compilation incrémentale possible et plus fiable et efficace
-include $(DEPENDS)



usage:
	@echo ""
	@echo "${BPINK}Exemples d'utilisation :${RESET}"
	@echo "          ./$(NAME) < fichier.hex"
	@echo "          echo \"8007000548324132FF\" | ./$(NAME)"



# Aide
help:
	@echo ""
	@echo "${BPINK}Cibles disponibles :${RESET}"
	@echo "  ${BCYAN}${NAME}${RESET}      - Compile le programme"
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

include Makefile.tests.mk


