#ifndef _AFFICHAGE_MENU_PRICIPAL_
#define _AFFICHAGE_MENU_PRICIPAL_ 

#include <SDL/SDL.h>
#include "globals.h"

void Affiche_menu (menu_t m);

void Affiche_menu_t (menu_t m, bool seven);

void Affiche_entry (menu_t m, bool pointe);

void east1 (SDL_Surface* window, SDLKey key);

#endif /* _AFFICHAGE_MENU_PRICIPAL_ */
