/*
 * action_menu_principal.h
 *
 *  Created on: 21 avr. 2016
 *      Author: nathan
 */

#ifndef _ACTION_MENU_PRINCIPAL_H_
#define _ACTION_MENU_PRINCIPAL_H_

#include <SDL/SDL.h>
#include <stdbool.h>
#include "globals.h"

void deplacement_menu_mouse (menu_t m, const SDL_Event* event);

void deplacement_menu_key (menu_t m, SDLKey key);

bool selection_menu (menu_t m, int* r);

#endif /* _ACTION_MENU_PRINCIPAL_H_ */
