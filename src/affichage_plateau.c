#include "affichage_plateau.h"

#include <math.h>
#include <stdlib.h>

#include "param.h"
#include "draw.h"

#define DBORD 4

/* Internes */

void Quadrille (plateau_t p)
{
	SDL_Surface* hex = SDL_CreateRGBSurface (SDL_HWSURFACE, 2 * p->r, 2 * p->r, 32, 0, 0, 0, 0);
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
	SDL_Flip (p->window);
}

void Quadrille_bis (plateau_t p)
{
	SDL_Surface* hex = SDL_CreateRGBSurface (SDL_HWSURFACE, 2 * p->r, 2 * p->r, 32, 0, 0, 0, 0);
	SDL_Surface* hex1 = SDL_CreateRGBSurface (SDL_HWSURFACE, 2 * p->r, 2 * p->r, 32, 0, 0, 0, 0);
	SDL_Surface* hex2 = SDL_CreateRGBSurface (SDL_HWSURFACE, 2 * p->r, 2 * p->r, 32, 0, 0, 0, 0);
	Hexagon (hex, p->r, param->ex, param->in, DBORD, &(p->l));
	Hexagon (hex1, p->r, param->ex, param->in, DBORD, &(p->l));
	Circle (hex1, p->l, param->j2);
	Hexagon (hex2, p->r, param->ex, param->in, DBORD, &(p->l));
	Circle (hex2, p->l, param->j1);
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
	SDL_Flip (p->window);
}

void define_rayon (plateau_t p)
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

/*Externes*/

void Affiche_hexagon (plateau_t p, int x, int y, int state)
{
	SDL_Rect position = {p->marge_hori + x * (p->l + 1) + y * (p->l + 1) / 2, p->marge_vert + y * (1.5 * p->r)};
	SDL_Surface* hex = SDL_CreateRGBSurface (SDL_HWSURFACE, 2 * p->r, 2 * p->r, 32, 0, 0, 0, 0);
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
	Hexagon (hex, p->r, c, param->in, DBORD, &(p->l));
	switch (p->grid[x * NBSIDE + y])
	{
	case J1:
		Circle (hex, p->l, param->j1);
		break;
	case J2:
		Circle (hex, p->l, param->j2);
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
	for (int x = 0; x < NBSIDE; ++x)
		for (int y = 0; y < NBSIDE; ++y)
			p->grid [x * NBSIDE + y] = 0;
	p->window = window;
	p->player = false;
	define_rayon(p);
	Quadrille (p);
	return p;
}

plateau_t actu_plateau (plateau_t p)
{
	define_rayon(p);
	Quadrille_bis (p);
	return p;
}

void free_plateau (plateau_t p)
{
	free (p->grid);
	free (p);
}
