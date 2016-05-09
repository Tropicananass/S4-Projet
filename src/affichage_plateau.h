#ifndef _AFFICHAGE_PLATEAU_H_
#define _AFFICHAGE_PLATEAU_H_

#include <SDL/SDL.h>

#include "globals.h"

#define COLOR 32

void Affiche_hexagon (plateau_t p, int x, int y, int state);

void Menu_button (plateau_t p, bool pointe);

plateau_t init_plateau (SDL_Surface* window);

plateau_t load_plateau (SDL_Surface* window, int* hist);

plateau_t actu_plateau (plateau_t p);

void free_plateau (plateau_t p);

#endif /* _AFFICHAGE_PLATEAU_H_ */
