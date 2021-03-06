#include "action_plateau.h"

#include <math.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#include "affichage_plateau.h"
#include "globals.h"
#include "sound.h"
#include "testJeu.h"

void annuler (plateau_t p)
{
	if (p->annule)
	{
		p->grid [p->hist [--p->nb_coups]] = 0;
		//Affiche_hexagon(p, p->hist [p->nb_coups] / NBSIDE, p->hist [p->nb_coups] % NBSIDE, NORMAL);
		p->hist [p->nb_coups] = -1;
		p->player = !p->player;
		p->annule = false;
	}
}

int selection (plateau_t p, curseur_t c)
{
	if (0 <= c.x && c.x < NBSIDE && 0 <= c.y && c.y < NBSIDE && p->grid [c.x * NBSIDE + c.y] == 0)
	{
		p->grid [c.x * NBSIDE + c.y] = PLAYER(p->player);
		p->hist [p->nb_coups++] = c.x * NBSIDE + c.y;
		p->player = !p->player;
		p->annule = true;
		Affiche_hexagon(p, c.x, c.y, PLAYER(p->player));
		return testGagne (p->grid, PLAYER(!p->player));
	}/*
	for (int x = 0; x < NBSIDE; ++x)
	{
			for (int y = 0; y < NBSIDE; ++y)
				printf ("%d ", p->hist [y * NBSIDE + x]);
			printf ("\n");
	}
	printf ("\n");*/

	return 0;
}

void deplacement_mouse (plateau_t p, SDL_MouseMotionEvent motion, curseur_t* c)
{
	vec2 pos = {motion.x - (p->marge_hori + p->r - p->l/2 - 2), motion.y - (p->marge_vert  + 1)};
	int ligne =  round(pos.y / (1.5 * p->r) - .5);
	int colone = round(pos.x / (p->l + 1.) - .5);
	vec2 relative = {pos.x % (p->l + 1), pos.y % (int)(1.5 * p->r)};
	vec2 proj = {relative.y + relative.x / RAC3, relative.y - relative.x / RAC3};
	vec2 hex;
	if (ligne % 2)
	{
		if (proj.y < 0 && proj.x < p->r)
		{
			/* haut */
			hex.y = ligne - 1;
			hex.x = colone - ligne / 2;
		}
		else if (relative.x > p->l / 2)
		{
			/* bas droite */
			hex.y = ligne;
			hex.x = colone - ligne / 2;
		}
		else
		{
			/* bas gauche */
			hex.y = ligne;
			hex.x = colone - ligne / 2 - 1;
		}
	}
	else
	{
		if (-proj.y > p->r/2)
		{
			/* haut droite */
			hex.y = ligne - 1;
			hex.x = colone - ligne / 2 + 1;
		}
		else if (proj.x < p->r/2)
		{
			/* haut gauche */
			hex.y = ligne - 1;
			hex.x = colone - ligne / 2;
		}
		else
		{
			/* bas */
			hex.y = ligne;
			hex.x = colone - ligne / 2;
		}
	}

	if (hex.x != c->x || hex.y != c->y)
	{

		if (0 <= c->x && c->x < NBSIDE && 0 <= c->y && c->y < NBSIDE)
			Affiche_hexagon(p, c->x, c->y, NORMAL);
		c->x = hex.x;
		c->y = hex.y;
		if (0 <= hex.x && hex.x < NBSIDE && 0 <= hex.y && hex.y < NBSIDE)
		{
			Affiche_hexagon(p, c->x, c->y, PLAYER(p->player));
			play_clik();
		}

	}
}

void deplacement_key (plateau_t p, SDLKey key, curseur_t* c)
{
	if (0 <= c->x && c->x < NBSIDE && 0 <= c->y && c->y < NBSIDE)
		Affiche_hexagon(p, c->x, c->y, NORMAL);
	switch (key)
	{
	case SDLK_RIGHT:
		++(c->x);
		break;
	case SDLK_LEFT:
		--(c->x);
		break;
	case SDLK_UP:
		--(c->y);
		break;
	case SDLK_DOWN:
		++(c->y);
		break;
	default:
		break;
	}
	if (c->x >= NBSIDE)
		c->x = 0;
	if (c->x < 0)
		c->x = NBSIDE - 1;
	if (c->y >= NBSIDE)
		c->y = 0;
	if (c->y < 0)
		c->y = NBSIDE - 1;
	Affiche_hexagon(p, c->x, c->y, PLAYER(p->player));
	play_clik();
}
