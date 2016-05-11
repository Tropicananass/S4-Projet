/*
 * menu_en_jeu.c
 *
 *  Created on: 9 mai 2016
 *      Author: nathan
 */

#include "menu_en_jeu.h"

#include "affichage_menu.h"
#include "affichage_plateau.h"
#include "menu.h"
#include "sub_menu.h"
#include "sauvegarde.h"


void random_char_9 (char* s)
{
	for (int i = 0; i < 8; ++i)
		s[i] = 'A' + rand() % 26;
	s[9]= '\0';
}

bool d_menu_mouse (plateau_t p, SDL_MouseMotionEvent motion)
{
	if (p->menu.x < motion.x && motion.x < p->menu.x + p->menu.w && p->menu.y < motion.y && motion.y < p->menu.y + p->menu.h)
	{
		Menu_button (p, 1);
		return true;
	}
	return false;
}

int menu_hist (plateau_t p)
{
	char* entries [7] = {"Case", "", "", "Retour", ">", "<", ""};//{"Case", "Joueur 1", " 6 - 4 ", "Retour", ">", "<", " Coup 4 "};
	entries [1] = malloc (sizeof (char) * 9);
	entries [2] = malloc (sizeof (char) * 8);
	entries [6] = malloc (sizeof (char) * 9);
	int cur = p->nb_coups - 1;
	sprintf (entries [1], "Joueur %d", PLAYER(cur%2));
	sprintf (entries [2], " %2d - %2d ", p->hist[cur] / NBSIDE + 1, p->hist[cur] % NBSIDE + 1);
	sprintf (entries [6], " Coup %3d ", cur);
	menu_t m = init_menu (p->window, entries);
	m->nb_entries = 7;
	Affiche_menu(m);
	int retour;
	bool end = false;

	while (!end)
	{
		SDL_Event event;
		SDL_WaitEvent (&event);
		if (event.type == SDL_QUIT)
			exit (0);
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
			retour = M_DOWN;
		else
			retour = evenement_menu(p->window, m, event, 0);
		switch (retour)
		{
		case M_UP:
			break;
		case M7_UP:
			if (cur <= 0)
				cur = p->nb_coups - 1;
			else
				--cur;
			sprintf (entries [1], "Joueur %d", PLAYER(cur%2));
			sprintf (entries [2], " %2d - %2d ", p->hist[cur] / NBSIDE + 1, p->hist[cur] % NBSIDE + 1);
			sprintf (entries [6], " Coup %3d ", cur);
			Affiche_menu (m);
			break;
		case M_LEFT :
			break;
		case M_RIGHT :
			if (cur >= p->nb_coups - 1)
				cur = 0;
			else
				++cur;
			sprintf (entries [1], "Joueur %d", PLAYER(cur%2));
			sprintf (entries [2], " %2d - %2d ", p->hist[cur] / NBSIDE + 1, p->hist[cur] % NBSIDE + 1);
			sprintf (entries [6], " Coup %3d ", cur);
			Affiche_menu (m);
			break;
		case M_DOWN :
			end = true;
			break;
		case M7_DOWN :
			break;
		case M_MID :
			break;
		}
	}
	free_menu(m);
	free (entries[1]);
	free (entries[2]);
	free (entries[6]);
	return retour;
}

int menu_save (plateau_t p)
{
	char* entries [5] = {"<", "Save", "", "Retour", ">"};
	entries [2] = malloc (sizeof (char) * 12);
	int cur = 0;
	strcpy (entries [2], "_\0");
	menu_t m = init_menu (p->window, entries);
	Affiche_menu(m);
	int retour;
	bool end = false;

	while (!end)
	{
		SDL_Event event;
		SDL_WaitEvent (&event);
		if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
		{
			retour = M_DOWN;
		}
		else if (event.type == SDL_KEYDOWN && SDLK_a <= event.key.keysym.sym && event.key.keysym.sym <= SDLK_z && cur < 8)
		{
			Uint8* keyboard = SDL_GetKeyState(NULL);
			if ((keyboard [SDLK_RSHIFT] || keyboard [SDLK_LSHIFT]) ^ keyboard [SDLK_CAPSLOCK])
				entries [2] [cur] = event.key.keysym.sym - 'a' + 'A';
			else
				entries [2] [cur] = event.key.keysym.sym;
			++cur;
			if (cur == 8)
				entries [2] [cur] = '\0';
			else
			{
				entries [2] [cur] = '_';
				entries [2] [cur + 1] = '\0';
			}
			vec2 c = m->cur;
			m->cur.x = 1;
			m->cur.x = 1;
			Affiche_entry (m, 0);
			m->cur = c;
		}
		else if (event.type == SDL_KEYDOWN && SDLK_KP0 <= event.key.keysym.sym && event.key.keysym.sym <= SDLK_KP9 && cur < 8)
		{
			entries [2] [cur] = event.key.keysym.sym - SDLK_KP0 + '0';
			++cur;
			if (cur == 8)
				entries [2] [cur] = '\0';
			else
			{
				entries [2] [cur] = '_';
				entries [2] [cur + 1] = '\0';
			}
			vec2 c = m->cur;
			m->cur.x = 1;
			m->cur.x = 1;
			Affiche_entry (m, 0);
			m->cur = c;
		}
		else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_BACKSPACE && cur > 0)
		{
			entries [2] [cur] = '\0';
			--cur;
			entries [2] [cur] = '_';
			vec2 c = m->cur;
			m->cur.x = 1;
			m->cur.x = 1;
			Affiche_entry (m, 0);
			m->cur = c;
		}
		else
			retour = evenement_menu(p->window, m, event, 0);

		switch (retour)
		{
		case M_UP:
			if (0 < cur && strlen (entries [2]) < 9)
			{
				end = true;
				char** liste;
				int nb = listeSauvegarde (&liste);
				for (int i = 0; i < nb && end; ++i)
					if (!strcmp (entries [2], liste [i]))
					{
						strcpy (entries [2], "existe déjà");
						cur = 1;
						vec2 c = m->cur;
						m->cur.x = 1;
						m->cur.x = 1;
						Affiche_entry (m, 0);
						m->cur = c;
						end = 0;
					}
				free_liste (liste, nb);
				if (end)
				{
					entries [2] [cur] = '\0';
					sauvegarde (entries [2], p->hist, p->ia);
				}
			}
			break;
		case M_LEFT :
		{
			random_char_9 (entries [2]);
			cur = 8;
			vec2 c = m->cur;
			m->cur.x = 1;
			m->cur.x = 1;
			Affiche_entry (m, 0);
			m->cur = c;
			break;
		}
		case M_RIGHT :
		{
			random_char_9 (entries [2]);
			cur = 8;
			vec2 c = m->cur;
			m->cur.x = 1;
			m->cur.x = 1;
			Affiche_entry (m, 0);
			m->cur = c;
			break;
		}
		case M_DOWN :
			end = true;
			break;
		case M_MID :
			break;
		}
	}


	free_menu(m);
	free (entries [2]);
	return retour;
}

int menu_en_jeu_part2 (plateau_t p)
{
	char* entries [5] = {"<", "Music", "HEX", "Quitter", " Son "};
	menu_t m = init_menu (p->window, entries);
	Affiche_menu(m);
	int retour;
	bool end = false;

	while (!end)
	{
		SDL_Event event;
		SDL_WaitEvent (&event);
		if (event.type == SDL_QUIT)
			exit (0);
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
			retour = M_DOWN;
		else
			retour = evenement_menu(p->window, m, event, 0);
		switch (retour)
		{
		case M_UP:
			menu_music (p->window, NULL);
			m->cur.x = 1;
			m->cur.y = 1;
			Affiche_menu(m);
			break;
		case M_LEFT :
			end = true;
			break;
		case M_RIGHT :
			menu_son (p->window, NULL);
			m->cur.x = 1;
			m->cur.y = 1;
			Affiche_menu(m);
			break;
		case M_DOWN :
			end = true;
			break;
		case M_MID :
			end = true;
			break;
		}
	}
	free_menu(m);
	return retour;
}

int menu_en_jeu_part1 (plateau_t p)
{
	SDL_Surface* save = SDL_CreateRGBSurface (SDL_HWSURFACE, p->window->w, p->window->h, p->window->format->BitsPerPixel, 0, 0, 0, 0);
	SDL_BlitSurface (p->window, NULL, save, NULL);
	vec2 resize = {p->window->w, p->window->h};
	char* entries [7] = {"Hist", "Save", "HEX", "Quitter", ">", "Cancel", "Retour"};
	menu_t m = init_menu (p->window, entries);
	m->nb_entries = 7;
	Affiche_menu(m);
	int retour;
	bool end = false;

	while (!end)
	{
		SDL_Event event;
		SDL_WaitEvent (&event);
		if (event.type == SDL_QUIT)
			exit (0);
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
			retour = M_DOWN;
		else
			retour = evenement_menu(p->window, m, event, 0);
		if (retour == M_UP || retour == M_RIGHT)
		{
			if (resize.x == p->window->w && resize.y == p->window->h)
				SDL_BlitSurface (save, NULL, p->window, NULL);
			else
				SDL_FillRect (p->window, NULL, SDL_MapRGB(p->window->format, 0, 0, 0));
		}
		switch (retour)
		{
		case M_UP:
			menu_save (p);
			m->cur.x = 1;
			m->cur.y = 1;
			Affiche_menu(m);
			break;
		case M_LEFT :
			if (p->nb_coups > 0)
			{
				menu_hist (p);
				m->cur.x = 1;
				m->cur.y = 1;
				Affiche_menu(m);
			}
			break;
		case M_RIGHT :
			end = true;
			break;
		case M_DOWN :
			end = true;
			break;
		case M_MID :
			break;
		case M7_DOWN :
			end = true;
			break;
		case M7_UP :
			annuler (p);
			break;
		}
	}
	free_menu(m);
	return retour;
}

int menu_en_jeu (plateau_t p)
{

	SDL_Surface* save = SDL_CreateRGBSurface (SDL_HWSURFACE, p->window->w, p->window->h, p->window->format->BitsPerPixel, 0, 0, 0, 0);
	SDL_BlitSurface (p->window, NULL, save, NULL);
	SDL_Surface* ombre = SDL_CreateRGBSurface (SDL_HWSURFACE, p->window->w, p->window->h, p->window->format->BitsPerPixel, 0, 0, 0, 0);
	SDL_FillRect (ombre, NULL, SDL_MapRGB (p->window->format, 0, 0, 0));
	SDL_SetAlpha (ombre, SDL_SRCALPHA, 175);
	SDL_BlitSurface (ombre, NULL, p->window, NULL);
	SDL_FreeSurface (ombre);

	int retour = M_LEFT;
	do
	{
		if (retour == M_LEFT)
			retour = menu_en_jeu_part1(p);
		else if (retour == M_RIGHT)
			retour = menu_en_jeu_part2(p);
	} while (retour != M_MID && retour != M_DOWN && retour != M7_DOWN);

	SDL_BlitSurface (save, NULL, p->window, NULL);
	SDL_FreeSurface (save);
	SDL_Event e;
	do
	{
		SDL_WaitEvent (&e);
	} while (e.type != SDL_MOUSEBUTTONUP && e.type != SDL_KEYUP);
	return retour;
}
