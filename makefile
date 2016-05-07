CC=gcc
CFLAGS=-Wall -g -std=c99 -c 
#-lm `sdl-config --cflags --libs` -lSDL_ttf
LDFLAGS=-std=c99 -lm `sdl-config --cflags --libs` -lSDL_ttf -lSDL_gfx -lSDL_mixer -lSDL_image
EXEC=bin/hex
SRC=src/hex.c src/affichage_plateau.c src/action_plateau.c src/affichage_menu_principal.c src/window.c src/param.c src/en_jeu.c src/menu_principal.c src/draw.c src/action_menu_principal.c src/sound.c src/testJeu.c src/sauvegarde.c
#$(wildcard src/*.c)
OBJ=$(SRC:.c=.o)

all: $(EXEC)

src/%.o: src/%.c
	$(CC) -o $@ $< $(CFLAGS)
	
$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)
	
	mv src/*.o obj/
	
clean:
	rm -rf src/*.o $(EXEC)