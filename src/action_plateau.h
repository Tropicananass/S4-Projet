#ifndef _ACTION_PLATEAU_H_
#define _ACTION_PLATEAU_H_

#include <SDL/SDL_events.h>

#include "globals.h"

void annuler (plateau_t p);

int selection (plateau_t p, curseur_t c);

void deplacement_mouse (plateau_t p, SDL_MouseMotionEvent motion, curseur_t* c);

void deplacement_key (plateau_t p, SDLKey key, curseur_t* c);

#endif /* _ACTION_PLATEAU_H_ */
