#include "menu_principal.h"

#include <math.h>

#include "action_menu.h"
#include "affichage_menu.h"
#include "menu.h"
#include "globals.h"
#include "window.h"
#include "scrolling.h"
#include "sound.h"
#include "sauvegarde.h"
#include "sub_menu.h"

int menu_taille (SDL_Surface* window, scrolling_t scroll)
{
	char* entries [5] = {"<", "TAILLE", "", "Retour", ">"};
	entries [2] = malloc (sizeof (char)* 8);
	sprintf (entries [2], "%dx%d", param->size, param->size);
	menu_t m = init_menu (window, entries);
	Affiche_menu(m);
	int retour;
	bool end = false;

	while (!end)
	{
		SDL_Event event;
		event = scroll_msg (window, scroll);
		if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
		{
			retour = M_DOWN;
		}
		else
			retour = evenement_menu(window, m, event, 0);
		if (event.type == SDL_VIDEORESIZE || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_f))
			scroll = resize_scroll(window, scroll);
		switch (retour)
		{
		case M_UP:
			break;
		case M_LEFT :
			if (param->size > 1)
			{
				--param->size;
				sprintf (entries [2], "%dx%d", param->size, param->size);
				vec2 c = m->cur;
				m->cur.x = 1;
				m->cur.y = 1;
				Affiche_entry (m, 0);
				m->cur = c;
			}
			break;
		case M_RIGHT :
		{
			if (param->size < MAXSIZE)
			{
				++param->size;
				sprintf (entries [2], "%dx%d", param->size, param->size);
				vec2 c = m->cur;
				m->cur.x = 1;
				m->cur.y = 1;
				Affiche_entry (m, 0);
				m->cur = c;
			}
			break;
		}
		case M_DOWN :
			end = true;
			break;
		case M_MID :
			break;
		}
	}
	free (entries [2]);
	free_menu(m);
	Mix_HaltMusic();
	return retour;
}

int menu_options (SDL_Surface* window, scrolling_t scroll)
{
	char* entries [5] = {"Music", "Taille", "OPTIONS", "Retour", " Son "};
	menu_t m = init_menu (window, entries);
	Affiche_menu(m);
	int retour;
	bool end = false;

	while (!end)
	{
		SDL_Event event;
		event = scroll_msg (window, scroll);
		if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
		{
			retour = M_DOWN;
		}
		else
			retour = evenement_menu(window, m, event, 0);
		if (event.type == SDL_VIDEORESIZE || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_f))
			scroll = resize_scroll(window, scroll);
		switch (retour)
		{
		case M_UP:
			menu_taille (window, scroll);
			m->cur.x = 1;
			m->cur.y = 1;
			Affiche_menu(m);
			break;
		case M_LEFT :
			menu_music (window, scroll);
			m->cur.x = 1;
			m->cur.y = 1;
			Affiche_menu(m);
			break;
		case M_RIGHT :
			menu_son (window, scroll);
			m->cur.x = 1;
			m->cur.y = 1;
			Affiche_menu(m);
			break;
		case M_DOWN :
			end = true;
			break;
		case M_MID :
			break;
		}
	}
	free_menu(m);
	return retour;
}

int menu_charger (SDL_Surface* window, scrolling_t scroll, char** file)
{
	char* entries [5] = {"<", "CHARGER", "", "Retour", ">"};
	entries [2] = malloc (sizeof (char)* 9);
	char** liste;
	int nb_sav = listeSauvegarde(&liste);
	if (nb_sav == 0)
	{
		liste = malloc(sizeof(char*));
		*liste = malloc(sizeof(char) * 5);
		strcpy (*liste, "None");
		nb_sav = 1;
	}
	int cur = 0;
	sprintf (entries [2], "%s", liste[cur]);
	menu_t m = init_menu (window, entries);
	Affiche_menu(m);
	int retour;
	bool end = false;

	while (!end)
	{
		SDL_Event event;
		event = scroll_msg (window, scroll);
		if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
		{
			retour = M_DOWN;
		}
		else
			retour = evenement_menu(window, m, event, 0);
		if (event.type == SDL_VIDEORESIZE || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_f))
			scroll = resize_scroll(window, scroll);
		switch (retour)
		{
		case M_UP:
			*file = malloc (sizeof (char) * (strlen(entries[2]) + 1));
			strcpy (*file, entries [2]);
			end = true;
			retour = M_UP;
			break;
		case M_LEFT :
		{

			if (!cur)
				cur = nb_sav - 1;
			else
				--cur;
			sprintf (entries [2], "%s", liste[cur]);
			vec2 c = m->cur;
			m->cur.x = 1;
			m->cur.y = 1;
			Affiche_entry (m, 0);
			m->cur = c;
			break;
		}
		case M_RIGHT :
		{
			cur = (cur + 1) % nb_sav;
			sprintf (entries [2], "%s", liste[cur]);
			vec2 c = m->cur;
			m->cur.x = 1;
			m->cur.y = 1;
			Affiche_entry (m, 0);
			m->cur = c;
			break;
		}
		case M_DOWN :
			end = true;
			break;
		case M_MID :
			if (confirmation (window, scroll, "supprimer", entries [2]) == M_UP)
			{
				supprimer (entries [2]);
				free_liste (liste, nb_sav);
				nb_sav = listeSauvegarde(&liste);
				if (nb_sav == 0)
				{
					liste = malloc(sizeof(char*));
					*liste = malloc(sizeof(char) * 5);
					strcpy (*liste, "None");
					nb_sav = 1;
				}
				cur = 0;
				sprintf (entries [2], "%s", liste[cur]);
			}
			m->cur.x = 1;
			m->cur.y = 1;
			Affiche_menu(m);
			break;
		}
	}
	free (entries [2]);
	free_liste (liste, nb_sav);
	free_menu(m);
	return retour;
}

int menu_principal (SDL_Surface* window, char** file)
{
	char* entries [5] = {"Charger", "Jouer", "HEX", "Quitter", "Options"};
	menu_t m = init_menu (window, entries);
	Affiche_menu(m);
	int retour;
	bool end = false;
	char* message [] = {"Crédits : Hugo Mathieux - Nathan Gouardères - Nolain Lehoux", "Crédits : Hugo Mathieux - Nathan Gouardères - Nolain Lehoux", "Crédits : Hugo Mathieux - Nathan Gouardères - Nolain Lehoux", "Crédits : Hugo Mathieux - Nathan Gouardères - Nolain Lehoux"};//"Crédits : petite bite & gros chakal Corp.", "Breaking News : Le Soudan en manque de soudeurs", "lmqsdkmq", "There is nothing to see here ... BASTARD !"};
	SDL_Color c [] = {{170,10,107}, {60,255,1}, {0, 0, 0}, {200, 180, 201}};
	scrolling_t scroll = init_scroll (window, message, c, 4);

	SDL_Event event;
	while (!end)
	{
		event = scroll_msg (window, scroll);
		if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
		{
			retour = M_DOWN;
		}
		else
			retour = evenement_menu(window, m, event, 0);
		switch (retour)
		{
		case M_UP:
			end = true;
			break;
		case M_LEFT :
			if (menu_charger (window, scroll, file) == M_UP)
			{
				end = true;
				retour = M_LEFT;
			}
			m->cur.x = 1;
			m->cur.y = 1;
			Affiche_menu(m);
			break;
		case M_RIGHT :
			menu_options (window, scroll);
			m->cur.x = 1;
			m->cur.y = 1;
			Affiche_menu(m);
			break;
		case M_DOWN :
			end = true;
			break;
		case M_MID :
			break;
		}
		if (event.type == SDL_VIDEORESIZE || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_f))
			scroll = resize_scroll(window, scroll);
	}
	free_menu(m);
	free_scroll (scroll);
	do
	{
		SDL_WaitEvent (&event);
	} while (event.type != SDL_MOUSEBUTTONUP && event.type != SDL_KEYUP);
	return retour;
}
