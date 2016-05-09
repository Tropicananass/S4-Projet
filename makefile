CC=gcc
CFLAGS=-Wall -g -std=c99 -c 
#-lm `sdl-config --cflags --libs` -lSDL_ttf
LDFLAGS=-std=c99 -lm `sdl-config --cflags --libs` -lSDL_ttf -lSDL_gfx -lSDL_mixer -lSDL_image
EXEC=bin/hex
SRCDIR=src
OBJDIR=obj
SRC=hex.c affichage_plateau.c action_plateau.c affichage_menu.c window.c param.c en_jeu.c menu_principal.c draw.c action_menu.c sound.c testJeu.c sauvegarde.c scrolling.c menu.c menu_en_jeu.c sub_menu.c
#$(wildcard src/*.c)
OBJ=$(SRC:.c=.o)

all: $(EXEC)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	if [ ! -d "obj" ]; then mkdir "obj"; fi
	$(CC) -o $@ $< $(CFLAGS)
	
$(EXEC): $(addprefix $(OBJDIR)/, $(OBJ))
	$(CC) -o $@ $^ $(LDFLAGS)
	
clean:
	rm -rf src/*.o obj/*.o $(EXEC)