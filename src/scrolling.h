/*
 * scrolling.h
 *
 *  Created on: 8 mai 2016
 *      Author: nathan
 */

#ifndef _SCROLLING_H_
#define _SCROLLING_H_

#include <SDL/SDL.h>
#include "globals.h"

typedef struct s_scrolling* scrolling_t;

struct s_scrolling{
	char** msgstr;
	SDL_Color* c;
	SDL_Surface** msg;
	int nb_surf;
	SDL_Rect* pos;
	SDL_Rect* dim;
	int first_msg, nb_msg, nb_msg_max;
};

typedef struct s_d_scrolling{
	scrolling_t s;
	SDL_Surface* precedent;
	bool id;
}* d_scrolling_t;

scrolling_t init_scroll (SDL_Surface* window, char** msg, SDL_Color* c, int nb_mess);

void free_scroll (scrolling_t s);

scrolling_t resize_scroll (SDL_Surface* w, scrolling_t s);

SDL_Event scroll_msg (SDL_Surface* window, scrolling_t s);

d_scrolling_t init_dynamic_scroll (SDL_Surface* window, plateau_t p);

SDL_Event dynamic_scroll (SDL_Surface* window, d_scrolling_t d, plateau_t p);

d_scrolling_t resize_dynamic_scroll (SDL_Surface* window, d_scrolling_t d, plateau_t p);

#endif /* _SCROLLING_H_ */
