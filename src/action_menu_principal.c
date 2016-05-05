/*
 * action_menu_principal.c
 *
 *  Created on: 21 avr. 2016
 *      Author: nathan
 */

#include "action_menu_principal.h"

#include <math.h>
#include <SDL/SDL_image.h>
#include "globals.h"
#include "affichage_menu_principal.h"
#include "sound.h"
#include "SDL/SDL_mixer.h"

void deplacement_menu_mouse (menu_t m, const SDL_Event* event)
{
	char* entries [3][3] =  {{"", "Charger", ""}, {"Jouer", "HEX", "Quitter"}, {"", "Options", ""}};

	vec2 pos = {event->motion.x - m->pos.x + m->l/2 - m->r, event->motion.y - m->pos.y};
	int ligne =  round(pos.y / (1.5 * m->r) - .5);
	int colone = round(pos.x / (m->l + 1.) - .5);
	vec2 relative = {pos.x % (m->l + 1), pos.y % (int)(1.5 * m->r)};
	vec2 proj = {relative.y + relative.x / RAC3, relative.y - relative.x / RAC3};
	vec2 hex;

	if (ligne % 2)
	{
		if (proj.y < 0 && proj.x < m->r)
		{
			/* haut */
			hex.y = ligne - 1;
			hex.x = colone - ligne / 2;
		}
		else if (relative.x > m->l / 2)
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
		if (-proj.y > m->r/2)
		{
			/* haut droite */
			hex.y = ligne - 1;
			hex.x = colone - ligne / 2 + 1;
		}
		else if (proj.x < m->r/2)
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

	if (0 > hex.x || hex.x >= 3 || 0 > hex.y || hex.y >= 3  || (hex.x != 1 && hex.y != 1))
	{
		hex.x = 1;
		hex.y = 1;
	}
	if (hex.x != m->cur.x || hex.y != m->cur.y)
	{
		Affiche_entry(m, entries [m->cur.x][m->cur.y], NORMAL);
		m->cur.x = hex.x;
		m->cur.y = hex.y;
		Affiche_entry(m, entries [hex.x][hex.y], POINTE);
		play_clik();
	}
}

void deplacement_menu_key (menu_t m, SDLKey key)
{

	char* entries [3][3] =  {{"", "Charger", ""}, {"Jouer", "HEX", "Quitter"}, {"", "Options", ""}};

	Affiche_entry(m, entries [m->cur.x][m->cur.y], NORMAL);
	switch (key)
	{
	case SDLK_RIGHT:
		++(m->cur.x);
		if (m->cur.y != 1)
			m->cur.y = 1;
		break;
	case SDLK_LEFT:
		--(m->cur.x);
		if (m->cur.y != 1)
			m->cur.y = 1;
		break;
	case SDLK_UP:
		--(m->cur.y);
		if (m->cur.x != 1)
			m->cur.x = 1;
		break;
	case SDLK_DOWN:
		++(m->cur.y);
		if (m->cur.x != 1)
			m->cur.x = 1;
		break;
	default:
		break;
	}
	if (m->cur.x >= 3)
		m->cur.x = 0;
	if (m->cur.x < 0)
		m->cur.x = 2;
	if (m->cur.y >= 3)
		m->cur.y = 0;
	if (m->cur.y < 0)
		m->cur.y = 2;
	Affiche_entry(m, entries [m->cur.x][m->cur.y], POINTE);
	play_clik();
}

void boom (SDL_Surface* window)
{
	SDL_Surface* tmp = IMG_Load ("ressources/.east/Boom.png");
	SDL_Surface* boom = rotozoomSurface(tmp, .0, fmin(window->h / (float)tmp->h, window->w / (float)tmp->w), 1);
	SDL_Rect p = {(window->w - boom->w) / 2, (window->h - boom->h) / 2};
	SDL_Surface* lum = SDL_CreateRGBSurface (SDL_HWSURFACE, window->w, window->h, window->format->BitsPerPixel, 0, 0, 0, 0);

	SDL_FreeSurface(tmp);
	tmp = SDL_CreateRGBSurface (SDL_HWSURFACE, window->w, window->h, window->format->BitsPerPixel, 0, 0, 0, 0);
	SDL_BlitSurface (window, NULL, tmp, NULL);

	Mix_Chunk* boomS = Mix_LoadWAV("ressources/.east/Boom.wav");
	Mix_VolumeChunk (boomS, .1 * MIX_MAX_VOLUME);
	if (boomS == NULL)
		fprintf (stderr, "ressources/.east/Boom.mp3 : %s\n", SDL_GetError());
	Mix_PlayChannel(0, boomS, 0);

	bool end = false;

	int i = 255;
	while (i >= 0)
	{
		if (!(SDL_GetTicks() % 40))
		{
			SDL_FillRect (window, NULL, SDL_MapRGB (window->format, 0, 0, 0));

			SDL_BlitSurface (boom, NULL, window, &p);

			Uint32 white = SDL_MapRGB (window->format, 255, 255, 255);
			SDL_FillRect (lum, NULL, white);
			SDL_SetAlpha(lum, SDL_SRCALPHA, i);
			SDL_BlitSurface (lum, NULL, window, NULL);
			SDL_Flip (window);
			i -= 4;
		}
	}

	Uint32 black = SDL_MapRGB (window->format, 0, 0, 0);
	SDL_FillRect (lum, NULL, black);
	SDL_SetAlpha(lum, SDL_SRCALPHA, 1);

	while (i < 256)
	{
		if (!(SDL_GetTicks() % 10))
		{
			SDL_BlitSurface (lum, NULL, window, NULL);
			SDL_Flip (window);
			i += 1;
		}
	}

	while (!end)
	{
		SDL_Event e;
		SDL_PollEvent (&e);
		if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
			end = true;
	}

	SDL_BlitSurface (tmp, NULL, window, NULL);

	SDL_FreeSurface(tmp);
	SDL_FreeSurface(boom);
	SDL_FreeSurface(lum);
	Mix_FreeChunk (boomS);
}

bool selection_menu (menu_t m, int* r)
{
	switch (10 * m->cur.x + m->cur.y)
	{
	case 01:
		*r = M_CHARGER;
		return false;
	case 10:
		*r = M_JOUER;
		play_gong();
		return true;
	case 11:
		*r = M_HEX;
		if (m->c == 15)
		{
			boom (m->window);
			m->c = 0;
		}
		else
			++m->c;
		return false;
	case 12:
		*r = M_QUITTER;
		return true;
	case 21:
		*r = M_OPTIONS;
		return false;
	default:
		return false;
	}
}
