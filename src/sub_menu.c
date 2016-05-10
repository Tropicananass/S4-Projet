/*
 * sub_menu.c
 *
 *  Created on: 9 mai 2016
 *      Author: nathan
 */

#include "sub_menu.h"

#include <stdbool.h>

#include "menu.h"
#include "affichage_menu.h"
#include "sound.h"

#define VOL(x) x * MIX_MAX_VOLUME / 100

int menu_music (SDL_Surface* window, scrolling_t scroll)
{
	bool en_jeu = Mix_PlayingMusic();
	if (!en_jeu)
		Mix_PlayMusic (param->music, -1);
	char* entries [5] = {"-", "MUSIC", "", "Retour", "+"};
	entries [2] = malloc (sizeof (char)* 4);
	unsigned int vol = param->music_vol * 100 / MIX_MAX_VOLUME;
	unsigned int tmp = vol;
	sprintf (entries [2], "%u", vol);
	menu_t m = init_menu (window, entries);
	Affiche_menu(m);
	int retour;
	bool end = false;

	while (!end)
	{
		SDL_Event event;
		if (scroll == NULL)
			SDL_WaitEvent (&event);
		else
			event = scroll_msg (window, scroll);
		if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
			retour = M_DOWN;
		else
			retour = evenement_menu(window, m, event, 1);
		if ((event.type == SDL_VIDEORESIZE || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_f)) && scroll != NULL)
			scroll = resize_scroll(window, scroll);
		switch (retour)
		{
		case M_UP:
			if (param->music_vol > 0)
			{
				tmp = vol;
				vol = 0;
			}
			else
				vol = tmp;
			sprintf (entries [2], "%d", vol);
			param->music_vol = VOL (vol);
			Mix_VolumeMusic (param->music_vol);
			vec2 c = m->cur;
			m->cur.x = 1;
			m->cur.y = 1;
			Affiche_entry (m, 0);
			m->cur = c;
			break;
		case M_LEFT :
			if (param->music_vol > 0)
			{
				--vol;
				sprintf (entries [2], "%d", vol);
				param->music_vol = VOL (vol);
				Mix_VolumeMusic (param->music_vol);
				vec2 c = m->cur;
				m->cur.x = 1;
				m->cur.y = 1;
				Affiche_entry (m, 0);
				m->cur = c;
			}
			break;
		case M_RIGHT :
		{
			if (param->music_vol < MIX_MAX_VOLUME)
			{
				++vol;
				sprintf (entries [2], "%d", vol);
				param->music_vol = VOL (vol);
				Mix_VolumeMusic (param->music_vol);
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
	if (!en_jeu)
		Mix_HaltMusic();
	return retour;
}

int menu_son (SDL_Surface* window, scrolling_t scroll)
{
	char* entries [5] = {"-", " SON ", "", "Retour", "+"};
	entries [2] = malloc (sizeof (char)* 4);
	unsigned int vol = param->chunk_vol * 100 / MIX_MAX_VOLUME;
	sprintf (entries [2], "%u", vol);
	menu_t m = init_menu (window, entries);
	Affiche_menu(m);
	int retour;
	bool end = false;
	int tmp = vol;

	while (!end)
	{
		SDL_Event event;
		if (scroll == NULL)
			SDL_WaitEvent (&event);
		else
			event = scroll_msg (window, scroll);
		if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
			retour = M_DOWN;
		else
			retour = evenement_menu(window, m, event, 1);
		if ((event.type == SDL_VIDEORESIZE || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_f)) && scroll != NULL)
			scroll = resize_scroll(window, scroll);
		switch (retour)
		{
		case M_UP:
			if (param->chunk_vol > 0)
			{
				tmp = vol;
				vol = 0;
			}
			else
				vol = tmp;
			sprintf (entries [2], "%d", vol);
			param->chunk_vol = VOL (vol);
			play_clik();
			vec2 c = m->cur;
			m->cur.x = 1;
			m->cur.y = 1;
			Affiche_entry (m, 0);
			m->cur = c;
			break;
		case M_LEFT :
			if (param->chunk_vol > 0)
			{
				--vol;
				sprintf (entries [2], "%d", vol);
				param->chunk_vol = VOL (vol);
				play_clik();
				vec2 c = m->cur;
				m->cur.x = 1;
				m->cur.y = 1;
				Affiche_entry (m, 0);
				m->cur = c;
			}
			break;
		case M_RIGHT :
		{
			if (param->chunk_vol < MIX_MAX_VOLUME)
			{
				++vol;
				sprintf (entries [2], "%d", vol);
				param->chunk_vol = VOL (vol);
				play_clik();
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
	return retour;
}
