/*
 * menu.c
 *
 *  Created on: 9 mai 2016
 *      Author: nathan
 */

#include "menu.h"

#include "action_menu.h"
#include "affichage_menu.h"
#include "window.h"



menu_t init_menu (SDL_Surface* window, char** entries)
{
	menu_t m = malloc (sizeof (struct s_menu));
	m->entries = entries;
	m->window = window;
	m->cur.x = 1;
	m->cur.y = 1;
	m->c = 0;
	return m;
}

void free_menu (menu_t m)
{
	free (m);
}

int evenement_menu (SDL_Surface* window, menu_t m, SDL_Event event, bool persist_select)
{
	switch (event.type)
	{
		case SDL_VIDEORESIZE:
			resize_window(window, &event);
			Affiche_menu (m);
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_f)
			{
				window = fullscreen_window(window);
				Affiche_menu (m);
			}
			else if (event.key.keysym.sym == SDLK_RETURN)
				return selection_menu (m);
			else if (SDLK_UP <= event.key.keysym.sym && event.key.keysym.sym <= SDLK_LEFT)
				deplacement_menu_key (m, event.key.keysym.sym);
			else
				east1 (window, event.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONUP:
			return selection_menu (m);
			break;
		case SDL_MOUSEMOTION:
			deplacement_menu_mouse (m, &event);
			break;
		default:
		{
			Uint8 *keyboard = SDL_GetKeyState(NULL);
			if (keyboard [SDLK_UP])
				deplacement_menu_key (m, SDLK_UP);
			if (keyboard [SDLK_DOWN])
				deplacement_menu_key (m, SDLK_DOWN);
			if (keyboard [SDLK_LEFT])
				deplacement_menu_key (m, SDLK_LEFT);
			if (keyboard [SDLK_RIGHT])
				deplacement_menu_key (m, SDLK_RIGHT);
			if (persist_select)
			{
				if (keyboard [SDLK_RETURN])
					return selection_menu (m);
				if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
					return selection_menu (m);
			}
		}
	}
	return M_NOT;
}
