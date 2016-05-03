/*
 * en_jeu.c
 *
 *  Created on: 6 avr. 2016
 *      Author: nathan
 */

#include "en_jeu.h"

#include "globals.h"
#include "action_plateau.h"
#include "affichage_plateau.h"
#include "window.h"
#include "param.h"

void en_jeu (SDL_Surface* window)
{
	Mix_PlayMusic(param->music, -1);
	plateau_t plateau = init_plateau (window);
	vec2 c = {0, 0};
	bool end = false;
	while (!end)
	{
		SDL_Event event;
		SDL_WaitEvent (&event);
		switch (event.type)
		{
			case SDL_VIDEORESIZE:
				resize_window(window, &event);
				plateau = actu_plateau(plateau);
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
					end = true;
				else if (event.key.keysym.sym == SDLK_f)
				{
					window = fullscreen_window(window);
					plateau = actu_plateau(plateau);
				}
				else if (event.key.keysym.sym == SDLK_RETURN)
					end = selection (plateau, c);
				else if (SDLK_UP <= event.key.keysym.sym && event.key.keysym.sym <= SDLK_LEFT)
					deplacement(plateau, &event, &c);
				break;
			case SDL_MOUSEBUTTONUP:
				end = selection (plateau, c);
				break;
			case SDL_MOUSEMOTION:
				deplacement(plateau, &event, &c);
				break;
			case SDL_QUIT:
				end = true;
				break;
		}
	}
	while (!end)
	{
		SDL_Event event;
		SDL_WaitEvent (&event);
		if (event.type == SDL_QUIT)
			end = true;
	}
	free_plateau (plateau);
	Mix_HaltMusic();
	Mix_RewindMusic();
}
