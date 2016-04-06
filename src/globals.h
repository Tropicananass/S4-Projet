#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#define DWIDTH 800
#define DHEIGHT 600
#define NBSIDE 11
#define PI 3.14159265

#define J1 1
#define J2 2
#define NORMAL 0

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

struct s_plateau {
	int l;
	int r;
	int marge_vert;
	int marge_hori;
	SDL_Surface* window;
};

void sleep_ms(int milliseconds);

#endif /* _GLOBALS_H_ */
