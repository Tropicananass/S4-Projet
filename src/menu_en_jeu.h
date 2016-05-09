/*
 * menu_en_jeu.h
 *
 *  Created on: 9 mai 2016
 *      Author: nathan
 */

#ifndef _MENU_EN_JEU_H_
#define _MENU_EN_JEU_H_

#include <stdbool.h>
#include <SDL/SDL.h>

#include "globals.h"

bool d_menu_mouse (plateau_t p, SDL_MouseMotionEvent motion);

int menu_en_jeu (plateau_t p);

#endif /* _MENU_EN_JEU_H_ */
