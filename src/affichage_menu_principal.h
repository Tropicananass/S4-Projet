#ifndef _AFFICHAGE_MENU_PRICIPAL_
#define _AFFICHAGE_MENU_PRICIPAL_ 

#include <SDL/SDL.h>
#include "globals.h"

void Affiche_menu_principal (menu_t m);

void Affiche_menu_options (SDL_Surface* window, int rayon, int l);

void Affiche_entry (menu_t m, char* entry, bool pointe);

void east1 (SDL_Surface* window, SDLKey key);

#endif /* _AFFICHAGE_MENU_PRICIPAL_ */
