#ifndef _AFFICHAGE_PLATEAU_H_
#define _AFFICHAGE_PLATEAU_H_

#include <SDL/SDL.h>

#define COLOR 32

typedef struct s_plateau* plateau_t;

void Affiche_hexagon (plateau_t p, int x, int y, int state);

plateau_t init_plateau (SDL_Surface* window);

plateau_t actu_plateau (plateau_t p);

void free_plateau (plateau_t p);

#endif /* _AFFICHAGE_PLATEAU_H_ */
