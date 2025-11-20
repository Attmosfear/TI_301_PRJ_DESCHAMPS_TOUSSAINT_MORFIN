# Makefile pour le projet Graphes de Markov
# TI301 - Algorithmique et Structures de données 2

# Compilateur et options
CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -g
LDFLAGS = -lm

# Fichiers
SOURCES = main.c utils.c hasse.c matrix.c
HEADERS = utils.h hasse.h matrix.h
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = markov

# Règle par défaut
all: $(EXECUTABLE)

# Création de l'exécutable
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "✓ Compilation terminée : $(EXECUTABLE)"

# Compilation des fichiers objet
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers générés
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
	rm -f *.mmd
	@echo "✓ Nettoyage effectué"

# Recompilation complète
rebuild: clean all

# Exécution du programme
run: $(EXECUTABLE)
	./$(EXECUTABLE)

# Aide
help:
	@echo "Commandes disponibles :"
	@echo "  make         - Compile le projet"
	@echo "  make clean   - Supprime les fichiers générés"
	@echo "  make rebuild - Recompile tout"
	@echo "  make run     - Compile et exécute"
	@echo "  make help    - Affiche cette aide"

.PHONY: all clean rebuild run help
