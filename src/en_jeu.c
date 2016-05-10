/*
 * en_jeu.c
 *
 *  Created on: 6 avr. 2016
 *      Author: nathan
 */

#include "en_jeu.h"

#include <SDL/SDL_rotozoom.h>
#include <stdlib.h>

#include "globals.h"
#include "action_plateau.h"
#include "affichage_plateau.h"
#include "affichage_menu.h"
#include "window.h"
#include "param.h"
#include "sauvegarde.h"
#include "scrolling.h"
#include "menu_en_jeu.h"
#include "menu.h"
#include "testJeu.h"

bool fake_IA (plateau_t p, bool* end)
{
	if (!*end)
	{
		int casejouee;
		do
		{
			casejouee = rand() % (NBSIDE * NBSIDE);
		} while (p->grid [casejouee] != 0);
		p->grid [casejouee] = PLAYER(p->player);
		p->hist [p->nb_coups++] = casejouee;
		p->player = !p->player;
		Affiche_hexagon (p, casejouee / NBSIDE, casejouee % NBSIDE, NORMAL);
		//Affiche_hexagon (p, p->hist[p->nb_coups - 2] / NBSIDE, p->hist[p->nb_coups - 2] % NBSIDE, PLAYER(p->player));
		*end = testGagne (p->grid, PLAYER(!p->player));
		return true;
	}
	return false;
}

void en_jeu (SDL_Surface* window, int* hist)
{
	Reset_window(window);
	Mix_PlayMusic(param->music, -1);
	plateau_t plateau;
	if (hist == NULL)
		plateau = init_plateau (window);
	else
		plateau = load_plateau (window, hist);
	vec2 c = {0, 0};
	bool end = false;
	bool gagne = false;
	d_scrolling_t d = init_dynamic_scroll (window, plateau);
	bool button = false;
	while (!end)
	{
		SDL_Event event = dynamic_scroll (window, d, plateau);
		switch (event.type)
		{
			case SDL_VIDEORESIZE:
				resize_window(window, &event);
				plateau = actu_plateau(plateau);
				d = resize_dynamic_scroll (window, d, plateau);
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
					end = true;
				else if (event.key.keysym.sym == SDLK_f)
				{
					window = fullscreen_window(window);
					plateau = actu_plateau(plateau);
					d = resize_dynamic_scroll (window, d, plateau);
				}
				else if (event.key.keysym.sym == SDLK_RETURN)
					if (button)
					{
						if (menu_en_jeu(plateau) == M_DOWN)
							end = true;
						Reset_window(window);
						plateau = actu_plateau(plateau);
						d = resize_dynamic_scroll (window, d, plateau);
					}
					else
						gagne = selection (plateau, c);
				else if (SDLK_UP <= event.key.keysym.sym && event.key.keysym.sym <= SDLK_LEFT)
					deplacement_key(plateau, event.key.keysym.sym, &c);
				else if (event.key.keysym.sym == SDLK_a || event.key.keysym.sym == SDLK_b || event.key.keysym.sym == SDLK_n)
					east1 (window, event.key.keysym.sym);
				break;
			case SDL_MOUSEBUTTONUP:
				if (button)
				{
					if (menu_en_jeu(plateau) == M_DOWN)
						end = true;
					Reset_window(window);
					plateau = actu_plateau(plateau);
					d = resize_dynamic_scroll (window, d, plateau);
				}
				else
					gagne = selection (plateau, c);
				break;
			case SDL_MOUSEMOTION:
			{
				bool tmp = d_menu_mouse(plateau, event.motion);
				if (button && !tmp)
					Menu_button (plateau, 0);
					button = tmp;
				if (!button)
					deplacement_mouse(plateau, event.motion, &c);
				break;
			}
			case SDL_QUIT:
				exit (0);
			default:
			{
				Uint8 *keyboard = SDL_GetKeyState(NULL);
				if (keyboard [SDLK_UP])
					deplacement_key (plateau, SDLK_UP, &c);
				if (keyboard [SDLK_DOWN])
					deplacement_key (plateau, SDLK_DOWN, &c);
				if (keyboard [SDLK_LEFT])
					deplacement_key (plateau, SDLK_LEFT, &c);
				if (keyboard [SDLK_RIGHT])
					deplacement_key (plateau, SDLK_RIGHT, &c);
			}
			
		}
		if (gagne)
		{
			while (!end)
			{
				char new [50];
				sprintf (new, "Joueur %d Gagne !!", PLAYER(!plateau->player));
				SDL_Surface* txt;
				if (PLAYER(!plateau->player) == J1)
					txt = TTF_RenderUTF8_Blended (param->font, new, param->rgb_j1);
				else
					txt = TTF_RenderUTF8_Blended (param->font, new, param->rgb_j2);
				SDL_Surface* final = rotozoomSurface (txt, .0, window->w / txt->w, 1);
				SDL_FreeSurface (txt);
				SDL_BlitSurface (final, NULL , window, NULL);
				SDL_FreeSurface (final);
				SDL_Event e;
				SDL_WaitEvent (&e);
				if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
					end = true;
			}
		}
	}
	free_plateau (plateau);
	Mix_HaltMusic();
	Mix_RewindMusic();
}
