/*
 * sub_menu.h
 *
 *  Created on: 9 mai 2016
 *      Author: nathan
 */

#ifndef _SUB_MENU_H_
#define _SUB_MENU_H_

#include <SDL/SDL.h>
#include "globals.h"
#include "scrolling.h"

int confirmation (SDL_Surface* window, scrolling_t scroll, char* ask1, char* ask2);

int menu_music (SDL_Surface* window, scrolling_t scroll);

int menu_son (SDL_Surface* window, scrolling_t scroll);

#endif /* _SUB_MENU_H_ */
