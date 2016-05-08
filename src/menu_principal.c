#include "menu_principal.h"

#include "action_menu_principal.h"
#include "affichage_menu_principal.h"
#include "globals.h"
#include "window.h"
#include "scrolling.h"

menu_t init_menu_principal (SDL_Surface* window)
{
	menu_t m = malloc (sizeof (struct s_menu));
	m->window = window;
	m->cur.x = 1;
	m->cur.y = 1;
	m->c = 0;
	return m;
}

menu_t init_menu_options (SDL_Surface* window)
{
	menu_t m = malloc (sizeof (struct s_menu));
	m->window = window;
	m->cur.x = 1;
	m->cur.y = 1;
	return m;
}

void free_menu (menu_t m)
{
	free (m);
}

scrolling_t resize_scroll (SDL_Surface* w, scrolling_t s)
{
	char** msg = s->msgstr;
	SDL_Color* c = s->c;
	int nb = s->nb_surf;
	free_scroll (s);
	s = init_scroll (w, msg, c, nb);
	return s;
}

int menu_options (SDL_Surface* window, scrolling_t s, menu_t m)
{
	Affiche_menu_options (window, m->r, m->l);
	bool end = 0;
	if (end)
		end = 1;
	/*while (!end)
	{
		SDL_Event event;
		event = scroll_msg (window, s);
		switch (event.type)
		{
			case SDL_VIDEORESIZE:
				resize_window(window, &event);
				Affiche_menu_principal(m);
				s = resize_scroll(window, s);
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
					end = 1;
				break;
				else if (event.key.keysym.sym == SDLK_f)
				{
					window = fullscreen_window(window);
					Affiche_menu_principal(m);
					scroll = resize_scroll(window, scroll);
				}
				else if (event.key.keysym.sym == SDLK_RETURN)
					end = selection_menu (m, &retour);
				else if (SDLK_UP <= event.key.keysym.sym && event.key.keysym.sym <= SDLK_LEFT)
					deplacement_menu (m, &event);
				break;
			case SDL_MOUSEBUTTONUP:
				end = selection_menu (m, &retour);
				break;
			case SDL_MOUSEMOTION:
				deplacement_menu (m, &event);
				break;
			case SDL_QUIT:
				end = 1;
				break;
		}
	}*/
	return 0;
}

int menu_principal (SDL_Surface* window)
{
	menu_t m = init_menu_principal (window);
	Affiche_menu_principal(m);
	int retour;
	bool end = 0;
	char* message [] = {"Crédits : petite bite & gros chakal Corp.", "Breaking News : Le Soudan en manque de soudeurs", "lmqsdkmq", "There is nothing to see here ... BASTARD !"};
	SDL_Color c [] = {{170,10,107}, {60,255,1}, {0, 0, 0}, {200, 180, 201}};
	scrolling_t scroll = init_scroll (window, message, c, 4);
	while (!end)
	{
		SDL_Event event;
		event = scroll_msg (window, scroll);
		switch (event.type)
		{
			case SDL_VIDEORESIZE:
				resize_window(window, &event);
				Affiche_menu_principal(m);
				scroll = resize_scroll(window, scroll);
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					retour = M_QUITTER;
					end = 1;
				}
				else if (event.key.keysym.sym == SDLK_f)
				{
					window = fullscreen_window(window);
					Affiche_menu_principal(m);
					scroll = resize_scroll(window, scroll);
				}
				else if (event.key.keysym.sym == SDLK_RETURN)
					end = selection_menu (m, &retour);
				else if (SDLK_UP <= event.key.keysym.sym && event.key.keysym.sym <= SDLK_LEFT)
					deplacement_menu_key (m, event.key.keysym.sym);
				else
					east1 (window, event.key.keysym.sym);
				break;
			case SDL_MOUSEBUTTONUP:
				end = selection_menu (m, &retour);
				break;
			case SDL_MOUSEMOTION:
				deplacement_menu_mouse (m, &event);
				break;
			case SDL_QUIT:
				retour = M_QUITTER;
				end = 1;
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
			}

		}
		if (retour == M_OPTIONS)
		{
			menu_options (window, scroll, m);
			retour = -1;
		}

	}
	free_menu(m);
	free_scroll (scroll);
	return retour;
}
