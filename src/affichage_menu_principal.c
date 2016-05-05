#include "affichage_menu_principal.h"

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_rotozoom.h>
#include <assert.h>
#include "param.h"
#include "draw.h"

#define NBOPTIONS 5

#define LEFT 0
#define RIGHT 1


/* Menu */
/* Interne */

void Redim_entry (SDL_Surface** entry, int rayon, int l)
{
	SDL_Surface* entry_dim;
	if ((*entry)->h / (float)(*entry)->w < rayon / (float)l)
		entry_dim = rotozoomSurface(*entry, 0, l/(float)(*entry)->w, 1);
	else
		entry_dim = rotozoomSurface(*entry, 0, rayon/(float)(*entry)->h, 1);
	SDL_FreeSurface (*entry);
	*entry = entry_dim;
}

SDL_Surface* Incruste (SDL_Surface* hex, char* title, int l, TTF_Font* font, SDL_Color c)
{
	SDL_Surface* hex_entry = SDL_CreateRGBSurface (SDL_HWSURFACE, hex->w, hex->h, hex->format->BitsPerPixel, 0, 0, 0, 0);
	SDL_BlitSurface (hex, NULL, hex_entry, NULL);

	SDL_Surface* entry = TTF_RenderText_Blended(font, title, c);
	Redim_entry(&entry, hex->w / 2, l - 20);

	SDL_Rect position = {hex->w / 2 - l / 2 + (l - entry->w) / 2 - 1, hex->w / 2 / 2 + (hex->w / 2 - entry->h) / 2};

	SDL_BlitSurface (entry , NULL, hex_entry, &position);
	SDL_SetColorKey (hex_entry, SDL_SRCCOLORKEY, SDL_MapRGB(hex->format, 0, 0, 0));

	SDL_FreeSurface (entry);
	return hex_entry;
}



void Case_menu (menu_t m, int rayon)
{
	SDL_Color fontColor = {100, 100, 100};

	int l;
	SDL_Surface* hex = SDL_CreateRGBSurface (SDL_HWSURFACE, 2*rayon, 2*rayon, m->window->format->BitsPerPixel, 0, 0, 0, 0);
	Hexagon (hex, rayon, param->ex, param->in, 10, &l);
	int xorig = (m->window->w - 4 * (l + 1)) / 2;
	/* Charger */
	int dx =  xorig - rayon + l/2 + 1;
	SDL_Rect position = {dx + 1 * (l + 1) / 2, 0 + 1 * (1.5 * rayon)};
	SDL_Surface* hex_entry = Incruste(hex, "Charger", l, param->font, fontColor);
	SDL_BlitSurface (hex_entry, NULL, m->window, &position);
	SDL_FreeSurface (hex_entry);

	m->l = l;
	m->r = rayon;
	m->pos.x = dx;
	m->pos.y = 0;

	/* Option */
	dx = xorig + 2 * (l + 1) - rayon + l/2 + 1;
	position.x = dx + 1 * (l + 1) / 2;
	hex_entry = Incruste(hex, "Options", l, param->font, fontColor);
	SDL_BlitSurface (hex_entry, NULL, m->window, &position);
	SDL_FreeSurface (hex_entry);

	dx = xorig + 1 * (l + 1) - rayon + l/2 + 1;
	char* entry [3] = {"Jouer", "HEX", "Quitter"};
	for (int j = 0; j < 3; ++j)
	{
		position.x = dx + j * (l + 1) / 2;
		position.y = 0 + j * (1.5 * rayon);
		hex_entry = Incruste(hex, entry[j], l, param->font, fontColor);
		SDL_BlitSurface (hex_entry, NULL, m->window, &position);
		SDL_FreeSurface (hex_entry);
	}

	SDL_FreeSurface(hex);
}

/* Externes */

void Affiche_menu_principal (menu_t m)
{
	int width = m->window->w - 20;
	int height = m->window->h * 11. / 12;

	int r1 = width / (3 * RAC3);
	int r2 = height / 5;

	if (r1 < r2)
		r1 = r1 - r1 % 2;
	else
		r1 = r2 - r2 % 2;

	Case_menu (m, r1);
	Affiche_entry(m, "HEX", POINTE);
	SDL_Flip (m->window);
}

void Affiche_entry (menu_t m, char* entry, bool pointe)
{
	int dx = m->pos.x + m->cur.x * (m->l + 1);
	SDL_Rect position = {dx + m->cur.y * (m->l + 1) / 2, m->pos.y + m->cur.y * (1.5 * m->r)};
	SDL_Surface* hex = SDL_CreateRGBSurface (SDL_HWSURFACE, 2*m->r, 2*m->r, m->window->format->BitsPerPixel, 0, 0, 0, 0);
	SDL_Surface* hex_entry;
	SDL_Color c;
	if (pointe)
	{
		Hexagon (hex, m->r, param->j1, param->in, 10, &(m->l));
		c.r = 255; c.g = 0; c.b = 0;
	}
	else
	{
		Hexagon (hex, m->r, param->ex, param->in, 10, &(m->l));
		c.r = 100; c.g = 100; c.b = 100;
	}
	hex_entry = Incruste (hex, entry, m->l, param->font, c);
	SDL_FreeSurface (hex);
	SDL_BlitSurface (hex_entry, NULL, m->window, &position);
	SDL_FreeSurface (hex_entry);
	SDL_Flip (m->window);
}

/* Menu options */
/* Interne */

/*void print_arrow (SDL_Surface* window, bool side, )
{
	SDL_Surface* arrow = SDL_CreateRGBSurface (SDL_HWSURFACE, 2*m->r, 2*m->r, window->format->BitsPerPixel, 0, 0, 0, 0);
}*/

SDL_Rect cadre_options (SDL_Surface* window, int l)
{
	SDL_Rect pos = {(window->w - 4 * (l + 1)) / 2 + (l + 1) / 2, 0, 2 * l + 1, window->h * 11. /12};
	SDL_FillRect (window, &pos, param->ex);
	pos.x += 10;
	pos.y += 10;
	pos.w -= 20;
	pos.h -= 20;
	SDL_FillRect (window, &pos, param->in);
	return pos;
}

int create_options_surfaces (SDL_Surface* entry [NBOPTIONS])
{
	char* entry_name [NBOPTIONS] = {"Musique : ", "Son : ", "Couleurs : ", "Taille : ", "Retour"};
	// Also modify NBOPTIONS
	int maxw = 0;
	for (int i = 0; i < NBOPTIONS; ++i)
	{
		entry [i] = TTF_RenderUTF8_Blended (param->font, entry_name [i], param->rgb_ex);
		if (maxw < entry[i]->w)
			maxw = entry[i]->w;
	}
	return maxw;
}

SDL_Rect blit_redim_options (SDL_Surface* window, SDL_Surface* entry [NBOPTIONS], SDL_Rect pos, int maxw, int size, int margev)
{
	for (int i = 0; i < NBOPTIONS - 1; ++i)
	{
		SDL_Surface* entry_dim;
		if (pos.w / 2 < entry[i]->w)
		{
			entry_dim = rotozoomSurface(entry[i], 0, pos.w / (2. * maxw), 1);
			SDL_FreeSurface (entry[i]);
		}
		else if (size < entry[i]->h)
		{
			entry_dim = rotozoomSurface(entry[i], 0, pos.w / (2. * maxw), 1);
			SDL_FreeSurface (entry[i]);
		}
		else
			entry_dim = entry[i];

		SDL_Rect pos_entry = {pos.x + (pos.w / 2 - entry_dim->w) / 2, pos.y + (size - entry_dim->h) / 2, 0, 0};
		SDL_BlitSurface (entry_dim, NULL, window, &pos_entry);

		pos.y += size + margev;

		SDL_FreeSurface (entry_dim);
	}
	return pos;
}

SDL_Rect blit_redim_retour (SDL_Surface* window, SDL_Surface* entry [NBOPTIONS], SDL_Rect pos, int maxw, int size)
{
	SDL_Surface* entry_dim;
	if (pos.w / 2 < entry[NBOPTIONS-1]->w)
	{
		entry_dim = rotozoomSurface(entry[NBOPTIONS-1], 0, pos.w / (2. * maxw), 1);
		SDL_FreeSurface (entry[NBOPTIONS-1]);
	}
	else if (size < entry[NBOPTIONS-1]->h)
	{
		entry_dim = rotozoomSurface(entry[NBOPTIONS-1], 0, pos.w / (2. * maxw), 1);
		SDL_FreeSurface (entry[NBOPTIONS-1]);
	}
	else
		entry_dim = entry[NBOPTIONS-1];

	SDL_Rect pos_entry = {pos.x + (pos.w - entry_dim->w)/2 + 10, pos.y + (size - entry_dim->h) / 2, 0, 0};
	SDL_Rect pos_cadre = {pos_entry.x - 10, pos_entry.y - 10, entry_dim->w + 20, entry_dim->h + 20};
	SDL_FillRect (window, &pos_cadre, param->ex);
	pos_cadre.x += 5;
	pos_cadre.y += 5;
	pos_cadre.w -= 10;
	pos_cadre.h -= 10;
	SDL_FillRect (window, &pos_cadre, param->in);
	SDL_BlitSurface (entry_dim, NULL, window, &pos_entry);

	SDL_FreeSurface (entry_dim);

	return pos;
}

/* Externe */

void Affiche_menu_options (SDL_Surface* window, int rayon, int l)
{
	SDL_Surface* hex = SDL_CreateRGBSurface (SDL_HWSURFACE, rayon * 2, rayon * 2, window->format->BitsPerPixel, 0, 0, 0, 0);
	Hexagon_single (hex, rayon, SDL_MapRGB(hex->format, 0, 0, 0), &l);
	int dx = (window->w - 4 * (l + 1)) / 2 - rayon + l/2 + 1  + (l + 1);
	SDL_Rect pos_hex = {dx + 2 * (l + 1) / 2, 2 * (1.5 * rayon)};
	SDL_BlitSurface (hex, NULL, window, &pos_hex);
	SDL_FreeSurface (hex);

	SDL_Rect pos = cadre_options (window, l);

	int size = pos.h * 2 / 3. / NBOPTIONS;
	int margev = pos.h / 3. / (NBOPTIONS + 1);

	pos.y += margev;
	pos.w -= 40;
	pos.x += 10;

	SDL_Surface* entry [NBOPTIONS];

	int maxw = create_options_surfaces (entry);

	pos = blit_redim_options (window, entry, pos, maxw, size, margev);
	pos = blit_redim_retour (window, entry, pos, maxw, size);

	for (int i = 0 ; i < window->h; ++i)
		putPixel (window, pos.x + pos.w/2 + 10, i, SDL_MapRGB (window->format, 0, 255, 0));


	SDL_Flip (window);
}

/* Scroll */
/* Interne */

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
	char* message [NBMESS] = {"Crédits : petite bite & gros chakal Corp.", "Breaking News : Le Soudan en manque de soudeurs", "lmqsdkmq", "There is nothing to see here ... BASTARD !"};
	// Also modify NBMESS
	SDL_Color c [NBMESS] = {{170,10,107}, {60,255,1}, {0, 0, 0}, {200, 180, 201}};
	for (int i = 0; i < NBMESS; ++i)
	{
		assert (message [i] != NULL);
		SDL_Surface* texte = TTF_RenderUTF8_Blended(param->font, message [i], c [i]);//, param->rgb_ex);
		Redim_entry(&texte, size, texte->w);
		s->msg [i] = SDL_CreateRGBSurface (SDL_HWSURFACE, texte->w + 1, texte->h, bpp, 0, 0, 0, 0);
		SDL_Rect temp = {0, 0, s->msg [i]->w, s->msg [i]->h};
		SDL_FillRect (s->msg [i], &temp, param->in);
		SDL_BlitSurface (texte, NULL, s->msg [i], &temp);
		SDL_FreeSurface (texte);
	}
}

void nb_msg_max (scrolling_t s, SDL_Surface* window)
{
	s->nb_msg_max = 1;
	for (int i = 0; i < NBMESS; ++i)
	{
		int nb_msg = 1;
		int taille_cumule = s->msg [i]->w;
		while (window->w > taille_cumule)
		{
			taille_cumule += s->msg [(i + nb_msg)%NBMESS]->w + 10;
			++nb_msg;
		}
		if (s->nb_msg_max < nb_msg)
			s->nb_msg_max = nb_msg;
	}
	++s->nb_msg_max;
	while (s->nb_msg_max % NBMESS != 0)
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
		s->dim [i].w = s->msg [i%NBMESS]->w;
		s->dim [i].h = s->msg [i%NBMESS]->h;
	}
}

/* Externe */

scrolling_t init_scroll (SDL_Surface* window)
{
	scrolling_t s = malloc (sizeof (struct s_scrolling));

	int size = Cadre_scroll (window);

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

	for (int i = 0; i < NBMESS; ++i)
		SDL_FreeSurface(s->msg [i]);

	free (s);
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
			SDL_BlitSurface (s->msg [c % NBMESS], s->dim + c, window, s->pos + c);
			s->pos [c].x -= 1;
			if (s->pos [c].x == -1)
				s->dim [c].x += 1;
		}

		if (s->dim [s->first_msg].x == s->msg [s->first_msg%NBMESS]->w)
		{
			s->dim [s->first_msg].x = 0;
			s->pos [s->first_msg].x = window->w;
			s->first_msg = (s->first_msg + 1) % (s->nb_msg_max);
			--(s->nb_msg);
		}

		int last = (s->first_msg + s->nb_msg - 1) % (s->nb_msg_max);
		if (s->pos[last].x + s->msg [last%NBMESS]->w - s->dim[last].x < window->w - 20 && s->nb_msg < s->nb_msg_max)
			s->nb_msg = (s->nb_msg + 1) % ((s->nb_msg_max) + 1);

		SDL_Flip(window);
		if (delay)
			SDL_Delay(5);
	}
	return event;
}

void banane (SDL_Surface* window)
{
	SDL_Surface* gif [8];
	SDL_Rect p = {0, 0};

	for (int i = 1; i <= 8; ++i)
	{
		char dir [254];
		sprintf (dir, "ressources/.east/Dancing_Banana%d.png", i);
		SDL_Surface* tmpB = IMG_Load (dir);
		float ch = window->h / (float)tmpB->h, cw = window->w / (float)tmpB->w;
		if (cw < ch)
		{
			gif [i - 1] = rotozoomSurface(tmpB, .0, cw, 1);
			p.y = (window->h - gif [i - 1]->h) / 2;
		}
		else
		{
			gif [i - 1] = rotozoomSurface(tmpB, .0, ch, 1);
			p.x = (window->w - gif [i - 1]->w) / 2;
		}
		SDL_FreeSurface (tmpB);
	}

	SDL_Surface* erase = SDL_CreateRGBSurface (SDL_HWSURFACE, gif[0]->w, gif[0]->h, window->format->BitsPerPixel, 0, 0, 0, 0);
	p.w = gif [0]->w;
	p.h = gif [0]->h;
	SDL_BlitSurface (window, &p, erase, NULL);

	bool end = false;
	int i = 0;
	while (!end)
	{
		if (!(SDL_GetTicks() % 100))
		{
			SDL_BlitSurface (gif [i%8], NULL, window, &p);
			SDL_Flip (window);
			SDL_BlitSurface (erase, NULL, window, &p);
			i = (i + 1) % 8 + 8;
		}
		SDL_Event e;
		SDL_PollEvent (&e);
		if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
			end = true;
	}

	SDL_BlitSurface (erase, NULL, window, &p);
	SDL_Flip (window);

	for (int i = 0; i < 8; ++i)
	{
		SDL_FreeSurface (gif [i]);
	}
	SDL_FreeSurface (erase);
}

void east1 (SDL_Surface* window, SDLKey key)
{
	static int banana = 0;
	switch (key)
	{
	case SDLK_a:
		if (banana % 2)
			++banana;
		else
			banana = 0;
		break;
	case SDLK_b:
		if (banana == 0)
			++banana;
		else
			banana = 0;
		break;
	case SDLK_n:
		if (banana == 2 || banana == 4)
			++banana;
		else
			banana = 0;
		break;
	default:
		banana = 0;
	}

	if (banana == 6)
	{
		banane (window);
		banana = 0;
	}
}
