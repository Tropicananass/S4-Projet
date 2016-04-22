#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include <stdbool.h>

#define DWIDTH 800
#define DHEIGHT 600

#define NBSIDE 11

#define PI 3.14159265
#define RAC3 1.73205080757

#define PLAYER(x) x + 1
#define J1 1
#define J2 2
#define NORMAL 0
#define POINTE 1

#define M_JOUER 0
#define M_CHARGER 1
#define M_OPTIONS 2
#define M_QUITTER 3
#define M_HEX 5

typedef struct s_vec3{
	int x;
	int y;
	int z;
} vec3;

typedef struct s_vec2{
	int x;
	int y;
} vec2;

typedef struct s_vec2 curseur_t;

typedef struct s_menu {
	int l;
	int r;
	vec2 pos;
	vec2 cur;
	SDL_Surface* window;
}* menu_t;

typedef struct s_plateau {
	int l;
	int r;
	int marge_vert;
	int marge_hori;
	SDL_Surface* window;
	int *grid;
	bool player;
}* plateau_t;

typedef struct s_param {
	Uint32 in, ex, j1, j2, background;
	SDL_Color rgb_in, rgb_ex, rgb_j1, rgb_j2, rgb_background;
	TTF_Font* font;
	Mix_Music* click;
}* param_t;

#endif /* _GLOBALS_H_ */
