/*
 * scrolling.c
 *
 *  Created on: 8 mai 2016
 *      Author: nathan
 */

#include "scrolling.h"

#include <stdbool.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_rotozoom.h>
#include <SDL/SDL_ttf.h>

/* Scroll */
/* Interne */

void Redim (SDL_Surface** entry, int size)
{
	SDL_Surface* entry_dim;
	entry_dim = rotozoomSurface(*entry, 0, size/(float)(*entry)->h, 1);
	SDL_FreeSurface (*entry);
	*entry = entry_dim;
}

int Cadre_scroll (SDL_Surface* window)
{
	int size = window->h / 24;
	SDL_Rect bande = {0, window->h * 45. / 48, window->w, size};
	SDL_FillRect (window, &bande, param->in);
	SDL_Flip (window);
	return size;
}

void create_messages_surfaces (scrolling_t s, int size, int bpp)
{
	s->msg = malloc (sizeof (SDL_Surface*) * s->nb_surf);
	for (int i = 0; i < s->nb_surf; ++i)
	{
		SDL_Surface* texte = TTF_RenderUTF8_Blended(param->font, s->msgstr [i], s->c [i]);//, param->rgb_ex);
		Redim(&texte, size);
		s->msg [i] = SDL_CreateRGBSurface (SDL_HWSURFACE, texte->w + 2, texte->h, bpp, 0, 0, 0, 0);
		SDL_Rect temp = {0, 0, s->msg [i]->w, s->msg [i]->h};
		SDL_FillRect (s->msg [i], &temp, param->in);
		SDL_BlitSurface (texte, NULL, s->msg [i], &temp);
		SDL_FreeSurface (texte);
	}
}

SDL_Surface* create_one_surface (char* msgstr, SDL_Color c, int size, int bpp)
{
	SDL_Surface* texte = TTF_RenderUTF8_Blended(param->font, msgstr, c);
	Redim(&texte, size);
	SDL_Surface *msg = SDL_CreateRGBSurface (SDL_HWSURFACE, texte->w + 1, texte->h, bpp, 0, 0, 0, 0);
	SDL_Rect temp = {0, 0, msg->w, msg ->h};
	SDL_FillRect (msg, &temp, param->in);
	SDL_BlitSurface (texte, NULL, msg, &temp);
	SDL_FreeSurface (texte);
	return msg;

}

void nb_msg_max (scrolling_t s, SDL_Surface* window)
{
	s->nb_msg_max = 1;
	for (int i = 0; i < s->nb_surf; ++i)
	{
		int nb_msg = 1;
		int taille_cumule = s->msg [i]->w;
		while (window->w > taille_cumule)
		{
			taille_cumule += s->msg [(i + nb_msg)%s->nb_surf]->w + 10;
			++nb_msg;
		}
		if (s->nb_msg_max < nb_msg)
			s->nb_msg_max = nb_msg;
	}
	++s->nb_msg_max;
	while (s->nb_msg_max % s->nb_surf != 0)
		++s->nb_msg_max;
}

void init_pos_dim (scrolling_t s, SDL_Surface* window)
{
	s->pos = malloc (sizeof (SDL_Rect) * s->nb_msg_max);
	s->dim = malloc (sizeof (SDL_Rect) * s->nb_msg_max);
	for (int i = 0; i < s->nb_msg_max; ++i)
	{
		s->pos [i].x = window->w;
		s->pos [i].y = window->h * 45. / 48;
		s->dim [i].x = 0;
		s->dim [i].y = 0;
		s->dim [i].w = s->msg [i%s->nb_surf]->w;
		s->dim [i].h = s->msg [i%s->nb_surf]->h;
	}
}

/* Externe */

scrolling_t init_scroll (SDL_Surface* window, char** msg, SDL_Color* c, int nb_mess)
{
	scrolling_t s = malloc (sizeof (struct s_scrolling));

	int size = Cadre_scroll (window);
	s->nb_surf = nb_mess;
	s->msgstr = msg;
	s->c = c;
	/* Creation surfaces */
	create_messages_surfaces (s, size, window->format->BitsPerPixel);

	nb_msg_max (s, window);

	init_pos_dim (s, window);

	s->first_msg = 0;
	s->nb_msg = 1;
	return s;
}

void free_scroll (scrolling_t s)
{
	free (s->pos);
	free (s->dim);

	for (int i = 0; i < s->nb_surf; ++i)
		SDL_FreeSurface(s->msg [i]);

	free (s->msg);
	free (s);
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

SDL_Event scroll_msg (SDL_Surface* window, scrolling_t s)
{
	int t0 = SDL_GetTicks();
	SDL_Event event = {0};
	bool delay;
	while ((delay = !SDL_PollEvent(&event)) && SDL_GetTicks() - t0 < 200)
	{
		for (int i = s->first_msg; i < s->first_msg + s->nb_msg; ++i)
		{
			int c = i % (s->nb_msg_max);
			SDL_BlitSurface (s->msg [c % s->nb_surf], s->dim + c, window, s->pos + c);
			s->pos [c].x -= 1;
			if (s->pos [c].x == -1)
				s->dim [c].x += 1;
		}

		if (s->dim [s->first_msg].x == s->msg [s->first_msg%s->nb_surf]->w)
		{
			s->dim [s->first_msg].x = 0;
			s->pos [s->first_msg].x = window->w;
			s->first_msg = (s->first_msg + 1) % (s->nb_msg_max);
			--(s->nb_msg);
		}

		int last = (s->first_msg + s->nb_msg - 1) % (s->nb_msg_max);
		if (s->pos[last].x + s->msg [last%s->nb_surf]->w - s->dim[last].x < window->w - 20 && s->nb_msg < s->nb_msg_max)
			s->nb_msg = (s->nb_msg + 1) % ((s->nb_msg_max) + 1);

		SDL_Flip(window);
		if (delay)
			SDL_Delay(5);
	}
	return event;
}

d_scrolling_t init_dynamic_scroll (SDL_Surface* window, plateau_t p)
{
	d_scrolling_t d = malloc (sizeof (struct s_d_scrolling));
	d->s = malloc (sizeof (struct s_scrolling));
	char* msg [2] = {"Au tour du Joueur 2", "Au tour du Joueur 1"};
	SDL_Color c [2] = {param->rgb_j2, param->rgb_j1};

	int size = Cadre_scroll (window);
	d->s->nb_surf = 2;
	d->s->c = c;
	d->s->msgstr = msg;
	create_messages_surfaces (d->s, size, window->format->BitsPerPixel);

	d->id = 0;
	d->precedent = NULL;

	nb_msg_max (d->s, window);

	SDL_Surface* j1 = d->s->msg [1];

	char new [50] = "Dernier coup : joueur 2 en 00 - 00";
	d->s->msg[1] = create_one_surface (new, param->rgb_j1, d->s->msg[0]->h, window->format->BitsPerPixel);


	init_pos_dim (d->s, window);

	d->s->first_msg = 0;
	d->s->nb_msg = 1;

	d->s->msg [1] = j1;
	d->s->nb_surf = 1;

	return d;
}

SDL_Event dynamic_scroll (SDL_Surface* window, d_scrolling_t d, plateau_t p)
{
	SDL_Surface* j1 = d->s->msg [1];
	SDL_Surface* j2 = d->s->msg [0];
	if (p->nb_coups != 0)
	{
		if (d->id != p->nb_coups%2)
		{
			d->s->nb_surf = 2;
			Cadre_scroll (window);
			d->id = !d->id;
			char new [50];
			sprintf (new, "Dernier coup : joueur %d en %2d - %2d", PLAYER(!p->player), p->hist[p->nb_coups - 1] / NBSIDE + 1, p->hist[p->nb_coups - 1] % NBSIDE + 1);
			if (p->player)
				d->s->msg[1] = create_one_surface (new, param->rgb_j1, d->s->msg[0]->h, window->format->BitsPerPixel);
			else
				d->s->msg[1] = create_one_surface (new, param->rgb_j2, d->s->msg[0]->h, window->format->BitsPerPixel);
		}
		else
			d->s->msg [1] = d->precedent;
	}

	if (PLAYER(p->player) == J1)
		d->s->msg [0] = j1;
	SDL_Event e = scroll_msg (window, d->s);

	if (p->nb_coups != 0)
	{
		if (d->precedent != NULL && d->precedent != d->s->msg [1])
			SDL_FreeSurface (d->precedent);
		d->precedent = d->s->msg [1];
	}
	d->s->msg [1] = j1;
	d->s->msg [0] = j2;

	return e;
}

d_scrolling_t resize_dynamic_scroll (SDL_Surface* window, d_scrolling_t d, plateau_t p)
{
	free_scroll (d->s);
	d->s = malloc (sizeof (struct s_scrolling));
	char* msg [2] = {"Au tour du Joueur 2", "Au tour du Joueur 1"};
	SDL_Color c [2] = {param->rgb_j2, param->rgb_j1};

	int size = Cadre_scroll (window);
	d->s->nb_surf = 2;
	d->s->c = c;
	d->s->msgstr = msg;
	create_messages_surfaces (d->s, size, window->format->BitsPerPixel);

	d->id = p->nb_coups%2;
	d->precedent = NULL;

	nb_msg_max (d->s, window);

	SDL_Surface* j1 = d->s->msg [1];

	char new [50];
	sprintf (new,  "Dernier coup : joueur %d en %2d - %2d  ", PLAYER(!p->player), p->hist[p->nb_coups - 1] / NBSIDE + 1, p->hist[p->nb_coups - 1] % NBSIDE + 1);
	if (PLAYER(!p->player) == J1)
		d->s->msg[1] = create_one_surface (new, param->rgb_j1, d->s->msg[0]->h, window->format->BitsPerPixel);
	else
		d->s->msg[1] = create_one_surface (new, param->rgb_j2, d->s->msg[0]->h, window->format->BitsPerPixel);


	init_pos_dim (d->s, window);

	d->s->first_msg = 0;
	d->s->nb_msg = 1;
	if (d->precedent != NULL && d->precedent != d->s->msg [1])
				SDL_FreeSurface (d->precedent);
	d->precedent = d->s->msg [1];
	d->s->msg [1] = j1;
	d->s->nb_surf = 2;

	return d;

}
