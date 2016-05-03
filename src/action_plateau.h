#ifndef _ACTION_PLATEAU_H_
#define _ACTION_PLATEAU_H_

#include <SDL/SDL_events.h>

#include "globals.h"

int selection (plateau_t p, curseur_t c);

void deplacement (plateau_t p, SDL_Event* event, curseur_t* c);

#endif /* _ACTION_PLATEAU_H_ */
