#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include <stdbool.h>

#define OK 0

#define DWIDTH 800
#define DHEIGHT 600

#define NBSIDE param->size
#define MAXSIZE 20

#define PI 3.14159265
#define RAC3 1.73205080757

#define PLAYER(x) x + 1
#define J1 1
#define J2 2
#define NORMAL 0
#define POINTE 1

#define M_UP 0
#define M_LEFT 1
#define M_RIGHT 2
#define M_DOWN 3
#define M_MID 5
#define M7_DOWN 6
#define M7_UP 7
#define M_NOT 8
#define M_QUIT 10

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
	char** entries;
	int nb_entries;
	int l;
	int r;
	vec2 pos;
	vec2 cur;
	SDL_Surface* window;
	int c;
}* menu_t;

typedef struct s_plateau {
	int l;
	int r;
	int marge_vert;
	int marge_hori;
	SDL_Surface* window;
	int *grid;
	int *hist;
	int nb_coups;
	bool player;
	bool annule;
	int ia;
	SDL_Rect menu;
}* plateau_t;

typedef struct s_param {
	Uint32 in, ex, j1, j2, background;
	SDL_Color rgb_in, rgb_ex, rgb_j1, rgb_j2, rgb_background;
	TTF_Font* font;
	Mix_Music* music;
	unsigned int music_vol;
	Mix_Chunk* click,* gong;
	unsigned int chunk_vol;
	int size;
}* param_t;

param_t param;

#endif /* _GLOBALS_H_ */
