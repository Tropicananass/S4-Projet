#include "affichage_plateau.h"

#include <math.h>
#include <stdlib.h>
#include <SDL/SDL_gfxPrimitives.h>

#include "draw.h"

#define DBORD 4

/* Internes */

void Define_rayon (plateau_t p)
{
	int decal_droite = 40;
	p->marge_vert = p->window->h / 6;
	int width = p->window->w - 2 * decal_droite;
	int height = p->window->h - 2 * p->marge_vert;
	int r1 = (height - (NBSIDE - 1)) / (.5 + NBSIDE * 1.5);
	int r2 = width / ((3 * NBSIDE - 1) * cos (PI/6));
	if (r1 < r2)
	{
		p->r = r1 - r1%2;
		p->marge_hori = p->window->w - decal_droite - 1.5 * (NBSIDE) * 2 * p->r * cos(PI/6);
	}
	else
	{
		p->r = r2 - r2%2;
		p->marge_hori = p->window->w - decal_droite - 1.5 * (NBSIDE) * 2 * p->r * cos(PI/6);
	}
}

void Bordures (plateau_t p)
{
	Sint16 x [NBSIDE * 4 + 2];
	Sint16 y [NBSIDE * 4 + 2];

	int dx = p->marge_hori + p->r - p->l / 2 - 2;
	x [0] = dx - 2 * RAC3;
	x [NBSIDE * 4 - 1] = dx;
	y [0] = p->marge_vert + p->r / 2 - 6;
	y [1] = p->marge_vert - 6;
	y [NBSIDE * 2] = y [1] + 4;
	y [NBSIDE * 2 + 1] = y [0] + 4;
	for (int i = 1; i < NBSIDE * 2; ++i)
	{
		x [i] = dx + i * (p->l + 1) / 2;
		x [NBSIDE * 4 - i - 1] = x [i];
		y [i] = y [i%2];
		y [NBSIDE * 2 + i] = y [NBSIDE * 2 + i%2];
	}
	y [0] += 2;

	filledPolygonRGBA(p->window, x, y, NBSIDE * 4, param->rgb_j2.r, param->rgb_j2.g, param->rgb_j2.b, 255);

	dx = p->marge_hori + p->r - p->l / 2 - 2 + (NBSIDE - 1) * (p->l + 1) / 2;
	x [0] = dx;
	x [NBSIDE * 4 + 1] = dx - 2 * RAC3;
	y [0] = p->marge_vert + NBSIDE * 1.5 *  p->r + 1;
	y [1] = p->marge_vert + NBSIDE * 1.5 *  p->r + p->r / 2 + 1;
	y [NBSIDE * 2 + 1] = y [0] + 4;
	y [NBSIDE * 2 + 2] = y [1] + 4;
	for (int i = 1; i < NBSIDE * 2 + 1; ++i)
	{
		x [i] = dx + i * (p->l + 1) / 2;
		x [NBSIDE * 4 + 2 - i - 1] = x [i];
		y [i] = y [i%2];
		y [NBSIDE * 2 + 1 + i] = y [NBSIDE * 2 + 1 + i%2];
	}
	y [NBSIDE * 4 + 1] -= 2;
	x [NBSIDE * 2 + 1] += 2 * RAC3;
	y [NBSIDE * 2 + 1] -= 2;

	filledPolygonRGBA(p->window, x, y, NBSIDE * 4 + 2, param->rgb_j2.r, param->rgb_j2.g, param->rgb_j2.b, 255);

	dx = p->marge_hori + p->r - p->l / 2 - 7;
	int dy = p->marge_vert + p->r / 2 + 2;
	for (int i = 0; i < NBSIDE * 2; ++i)
	{
		x [i] = dx + (int)(i / 2) * ((p->l + 1) / 2);
		x [NBSIDE * 4 - i - 1] = x[i] + 2 * RAC3;
		y [i] = dy + i * p->r / 2 + (int)((i + 1) / 2) * (p->r / 2);
		y [NBSIDE * 2 + i] = dy + (NBSIDE * 2 - 1 - i) * p->r / 2 + (int)((NBSIDE * 2 - i) / 2) * (p->r / 2) - 2;//y [i];
	}
	y [0] -= 2 * RAC3 + 1;
	y [NBSIDE * 4 - 1] -= 1;

	for (int i = 0; i < NBSIDE * 2; ++i)
	{
	printf ("%d - %d  -  %d - %d\n", x [i], x [NBSIDE * 2 + i], y [i], y [NBSIDE * 2 + i]);
	}

	filledPolygonRGBA(p->window, x, y, NBSIDE * 4, param->rgb_j1.r, param->rgb_j1.g, param->rgb_j1.b, 255);

	dx = p->marge_hori + p->r - p->l / 2 + NBSIDE * (p->l + 1) - p->l / 2 - 1;
	dy = p->marge_vert - 1;
	for (int i = 0; i < NBSIDE * 2 + 1; ++i)
	{
		x [i] = dx + (int)((i + 1) / 2) * ((p->l + 1) / 2);
		x [NBSIDE * 4 + 1 - i] = x[i] + 2 * RAC3;
		y [i] = dy + i * p->r / 2 + (int)(i / 2) * (p->r / 2);
		y [NBSIDE * 2 + 1 + i] = dy + (NBSIDE * 2 - i) * p->r / 2 + (int)((NBSIDE * 2 - i) / 2) * (p->r / 2) - 2;//y [i];
	}
	x [NBSIDE * 4 + 1] -= 2 * RAC3 - 1;
	y [NBSIDE * 4 + 1] -= 3;
	y [0] -= 1;
	y [NBSIDE * 2 + 1] += 2 * RAC3 + 2;
	y [NBSIDE * 2] += 1;

	filledPolygonRGBA(p->window, x, y, NBSIDE * 4 + 2, param->rgb_j1.r, param->rgb_j1.g, param->rgb_j1.b, 255);


	/*for (int i = 0; i < NBSIDE * 2 + 1; ++i)
	{
	printf ("%d - %d  -  %d - %d\n", x [i], x [NBSIDE * 2 + 1 + i], y [i], y [NBSIDE * 2 + 1 + i]);
	}*/

	/*// (horizontales) player 2
	int l = (cos (PI / 6) - cos (5 * PI / 6)) * p->r;
	int dx = p->marge_hori+ p->r - l / 2;
	Sint16 x1 [5] = {dx,
					dx - l / 2,
					dx,
					dx + NBSIDE * (l + 1) - .25 * l,
					dx + NBSIDE * (l + 1)};
	Sint16 y1 [5] = {p->marge_vert - p->r / 2,
					p->marge_vert,
					p->marge_vert + p->r / 2,
					p->marge_vert + p->r / 2,
					p->marge_vert - p->r / 2};
	filledPolygonRGBA(p->window, x1, y1, 5, param->rgb_j2.r, param->rgb_j2.g, param->rgb_j2.b, 255);
	dx += (NBSIDE - 1) * (l + 1) / 2;
	Sint16 x2 [5] = {dx + .25 * l,
					dx + NBSIDE * (l + 1),
					dx + NBSIDE * (l + 1) + l / 2,
					dx + NBSIDE * (l + 1),
					dx};
	int dy = p->marge_vert + 1.5 * p->r * NBSIDE + .5 * p->r;
	Sint16 y2 [5] = {dy - p->r / 2,
					dy - p->r / 2,
					dy,
					dy + p->r / 2,
					dy + p->r / 2};
	filledPolygonRGBA(p->window, x2, y2, 5, param->rgb_j2.r, param->rgb_j2.g, param->rgb_j2.b, 255);
	Sint16 x3 [5] = {x1 [1],
					x1 [1] - l / 2,
					x2 [4],
					x2 [0],
					x1 [2]};
	Sint16 y3 [5] = {y1 [1],
					y1 [2],
					y2 [4],
					y2 [0],
					y1 [2]};
	filledPolygonRGBA(p->window, x3, y3, 5, param->rgb_j1.r, param->rgb_j1.g, param->rgb_j1.b, 255);*/

	/*x [0] = ;
	x [1] = ;
	x [2] = ;
	x [3] = ;
	x [4] = ;
	y [0] = ;
	y [1] = ;
	y [2] = ;
	y [3] = ;
	y [4] = ;
	pos.x += (NBSIDE - 1) * (l + 1) / 2.;
	pos.y += 1.5 * p->r * NBSIDE + .5 * p->r;
	SDL_FillRect (p->window, &pos, param->j2);

	// (Verticales) player 1
	filledPolygonRGBA(p->window, x, y, 4, 255, 0, 255, 255);*/

	SDL_Flip (p->window);
}

void Quadrille (plateau_t p)
{
	Define_rayon(p);

	SDL_Surface* hex = SDL_CreateRGBSurface (SDL_HWSURFACE, 2 * p->r, 2 * p->r, p->window->format->BitsPerPixel, 0, 0, 0, 0);
	Hexagon (hex, p->r, param->ex, param->in, DBORD, &(p->l));
	for (int i = 0; i < NBSIDE; ++i)
	{
		int dx = p->marge_hori + i * (p->l + 1);
		for (int j = 0; j < NBSIDE; ++j)
		{
			SDL_Rect position = {dx + j * (p->l + 1) / 2, p->marge_vert + j * (1.5 * p->r)};
			SDL_BlitSurface (hex, NULL, p->window, &position);
		}
	}
	SDL_FreeSurface(hex);

	Bordures (p);

	SDL_Flip (p->window);
}

void Quadrille_bis (plateau_t p)
{
	Define_rayon(p);

	SDL_Surface* hex = SDL_CreateRGBSurface (SDL_HWSURFACE, 2 * p->r, 2 * p->r, p->window->format->BitsPerPixel, 0, 0, 0, 0);
	SDL_Surface* hex1 = SDL_CreateRGBSurface (SDL_HWSURFACE, 2 * p->r, 2 * p->r, p->window->format->BitsPerPixel, 0, 0, 0, 0);
	SDL_Surface* hex2 = SDL_CreateRGBSurface (SDL_HWSURFACE, 2 * p->r, 2 * p->r, p->window->format->BitsPerPixel, 0, 0, 0, 0);
	Hexagon (hex, p->r, param->ex, param->in, DBORD, &(p->l));
	Hexagon (hex1, p->r, param->ex, param->j1, DBORD, &(p->l));
	//Circle (hex1, p->l, param->j2);
	Hexagon (hex2, p->r, param->ex, param->j2, DBORD, &(p->l));
	//Circle (hex2, p->l, param->j1);
	for (int i = 0; i < NBSIDE; ++i)
	{
		int dx = p->marge_hori + i * (p->l + 1);
		for (int j = 0; j < NBSIDE; ++j)
		{
			SDL_Rect position = {dx + j * (p->l + 1) / 2, p->marge_vert + j * (1.5 * p->r)};
			switch (p->grid[i * NBSIDE + j])
			{
			case 0:
				SDL_BlitSurface (hex, NULL, p->window, &position);
				break;
			case 1:
				SDL_BlitSurface (hex1, NULL, p->window, &position);
				break;
			case 2:
				SDL_BlitSurface (hex2, NULL, p->window, &position);
				break;
			}
		}
	}
	SDL_FreeSurface(hex);
	SDL_FreeSurface(hex1);
	SDL_FreeSurface(hex2);

	Bordures (p);

	SDL_Flip (p->window);
}

/*Externes*/

void Affiche_hexagon (plateau_t p, int x, int y, int state)
{
	SDL_Rect position = {p->marge_hori + x * (p->l + 1) + y * (p->l + 1) / 2, p->marge_vert + y * (1.5 * p->r)};
	SDL_Surface* hex = SDL_CreateRGBSurface (SDL_HWSURFACE, 2 * p->r, 2 * p->r, p->window->format->BitsPerPixel, 0, 0, 0, 0);
	Uint32 c;
	switch (state)
	{
	case J1 :
		c = param->j1;
		break;
	case J2 :
		c = param->j2;
		break;
	case NORMAL :
		c = param->ex;
		break;
	}
	switch (p->grid[x * NBSIDE + y])
	{
	case NORMAL :
		Hexagon (hex, p->r, c, param->in, DBORD, &(p->l));
		break;
	case J1:
		//Circle (hex, p->l, param->j1);
		Hexagon (hex, p->r, c, param->j1, DBORD, &(p->l));
		break;
	case J2:
		//Circle (hex, p->l, param->j2);
		Hexagon (hex, p->r, c, param->j2, DBORD, &(p->l));
		break;
	}
	SDL_BlitSurface (hex, NULL, p->window, &position);
	SDL_FreeSurface (hex);
	SDL_Flip (p->window);
}

plateau_t init_plateau (SDL_Surface* window)
{
	plateau_t p = malloc (sizeof (struct s_plateau));
	p->grid = malloc (sizeof (int) * NBSIDE * NBSIDE);
	p->hist = malloc (sizeof (int) * NBSIDE * NBSIDE);
	for (int x = 0; x < NBSIDE * NBSIDE; ++x)
	{
		p->grid [x] = 0;
		p->hist [x] = -1;
	}
	p->nb_coups = 0;
	p->window = window;
	p->player = false;
	Quadrille (p);
	return p;
}

plateau_t load_plateau (SDL_Surface* window, int* hist)
{
	plateau_t p = malloc (sizeof (struct s_plateau));
	p->grid = malloc (sizeof (int) * NBSIDE * NBSIDE);
	p->hist = hist;
	p->player = false;
	for (int i = 0; i < NBSIDE * NBSIDE; ++i)
		p->grid [i] = 0;
	for (p->nb_coups = 0; p->nb_coups < NBSIDE * NBSIDE && hist [p->nb_coups] != -1; ++p->nb_coups)
	{
		p->grid [hist [p->nb_coups]] = PLAYER(p->player);
		p->player = !p->player;
	}
	p->window = window;
	Quadrille_bis (p);
	return p;
}

plateau_t actu_plateau (plateau_t p)
{
	Quadrille_bis (p);
	return p;
}

void free_plateau (plateau_t p)
{
	free (p->grid);
	free (p);
}
