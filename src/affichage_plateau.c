#include "affichage_plateau.h"

#include <math.h>
#include <stdlib.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_rotozoom.h>

#include "draw.h"

#define DBORD 4

/* Internes */

void Define_rayon (plateau_t p)
{
	p->marge_hori = p->window->w / 96;
	p->marge_vert = p->window->h / 12;
	int width = p->window->w - 2 * p->marge_hori;
	int height = p->window->h - 2 * p->marge_vert;
	int r1 = height / (.5 + NBSIDE * 1.5);
	int r2 = width / ((NBSIDE + NBSIDE / 2.) * RAC3);
	if (r1 < r2)
	{
		p->r = r1 - r1%2;
		int l = p->r * RAC3;
		p->marge_hori = (p->window->w - NBSIDE * l - NBSIDE * l / 2) / 2;
		p->marge_vert = (p->window->h - 1.5 * (NBSIDE) * p->r - p->r / 2) / 2 - p->window->h / 24;
	}
	else
	{
		p->r = r2 - r2%2;
		int l = p->r * RAC3;
		p->marge_hori = (p->window->w - NBSIDE * l - NBSIDE * l / 2) / 2;
		p->marge_vert = (p->window->h - 1.5 * (NBSIDE) * p->r - p->r / 2) / 2;
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

	Menu_button(p, 0);

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

	Menu_button(p, 0);

	SDL_Flip (p->window);
}

/*Externes*/

void Menu_button (plateau_t p, bool pointe)
{
	SDL_Rect pos = {p->marge_hori + (p->window->w - 2 * p->marge_hori) / 48, p->marge_vert + p->r * 1.5 * (NBSIDE - 2) + p->r / 2, .h = (p->window->h - 2 * p->marge_vert) / 9};

	SDL_Surface* tmp;
	SDL_Color c;
	if (pointe)
		if (PLAYER(p->player) == J1)
			c = param->rgb_j1;
		else
			c = param->rgb_j2;
	else
		c = param->rgb_ex;
	tmp = TTF_RenderUTF8_Blended (param->font, "Menu", c);
	SDL_Surface* menu = rotozoomSurface(tmp, .0, (pos.h - 8) / (float)tmp->h, 1);
	SDL_FreeSurface (tmp);
	pos. w = menu->w + 16;
	SDL_FillRect (p->window, &pos, SDL_MapRGB (p->window->format, c.r, c.g, c. b));
	pos.w -= 8;
	pos.h -= 8;
	pos.x += 4;
	pos.y += 4;
	SDL_FillRect (p->window, &pos, param->in);
	p->menu.x = pos.x;
	p->menu.y = pos.y;
	p->menu.w = pos.w;
	p->menu.h = pos.h;
	SDL_Rect postxt = {pos.x, pos.y};
	postxt.x = pos.x + (pos.w - menu->w) / 2;
	SDL_BlitSurface (menu, NULL, p->window, &postxt);
	SDL_Flip(p->window);
}

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
	p->annule = false;
	Quadrille (p);
	return p;
}

plateau_t load_plateau (SDL_Surface* window, int* hist)
{
	plateau_t p = malloc (sizeof (struct s_plateau));
	p->grid = malloc (sizeof (int) * NBSIDE * NBSIDE);
	p->hist = hist;
	p->player = false;
	p->annule = false;
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
