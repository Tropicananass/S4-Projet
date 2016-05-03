#include "action_plateau.h"

#include <math.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#include "affichage_plateau.h"
#include "globals.h"
#include "sound.h"
#include "testJeu.h"

int selection (plateau_t p, curseur_t c)
{
	if (0 <= c.x && c.x < NBSIDE && 0 <= c.y && c.y < NBSIDE && p->grid [c.x * NBSIDE + c.y] == 0)
	{
		p->grid [c.x * NBSIDE + c.y] = PLAYER(p->player);
		p->player = !p->player;
		Affiche_hexagon(p, c.x, c.y, PLAYER(p->player));
		return testGagne (p->grid, PLAYER(!p->player));
	}
	return 0;
	/*for (int x = 0; x < NBSIDE; ++x)
	{
			for (int y = 0; y < NBSIDE; ++y)
				printf ("%d ", p->grid [y * NBSIDE + x]);
			printf ("\n");
	}
	printf ("\n");*/
}

void deplacement (plateau_t p, SDL_Event* event, curseur_t* c)
{
	switch (event->type)
	{
	case SDL_MOUSEMOTION:
	{
		vec2 pos = {event->motion.x - (p->marge_hori + p->r - p->l/2 - 2), event->motion.y - (p->marge_vert  + 1)};
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



		break;
	}
	case SDL_KEYDOWN:
		while (!SDL_PollEvent(event))
		{
			if (0 <= c->x && c->x < NBSIDE && 0 <= c->y && c->y < NBSIDE)
				Affiche_hexagon(p, c->x, c->y, NORMAL);
			switch (event->key.keysym.sym)
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
			SDL_Delay (100);
		}
		break;
	}

}
