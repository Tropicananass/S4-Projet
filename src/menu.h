/*
 * menu.h
 *
 *  Created on: 9 mai 2016
 *      Author: nathan
 */

#ifndef _SRC_MENU_H_
#define _SRC_MENU_H_

#include <stdbool.h>
#include <SDL/SDL.h>

#include "globals.h"

menu_t init_menu (SDL_Surface* window, char** entries);

void free_menu (menu_t m);

int evenement_menu (SDL_Surface* window, menu_t m, SDL_Event event, bool persist_select);

#endif /* _SRC_MENU_H_ */
