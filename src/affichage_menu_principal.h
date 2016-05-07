#ifndef _AFFICHAGE_MENU_PRICIPAL_
#define _AFFICHAGE_MENU_PRICIPAL_ 

#include <SDL/SDL.h>
#include "globals.h"

#define NBMESS 4

typedef struct s_scrolling {
	SDL_Surface* msg [NBMESS];
	SDL_Rect* pos;
	SDL_Rect* dim;
	int first_msg, nb_msg, nb_msg_max;
}* scrolling_t;

void Affiche_menu_principal (menu_t m);

void Affiche_menu_options (SDL_Surface* window, int rayon, int l);

void Affiche_entry (menu_t m, char* entry, bool pointe);

scrolling_t init_scroll (SDL_Surface* window);

void free_scroll (scrolling_t s);

SDL_Event scroll_msg (SDL_Surface* window, scrolling_t s);

void east1 (SDL_Surface* window, SDLKey key);

#endif /* _AFFICHAGE_MENU_PRICIPAL_ */
