#include "affichage_menu.h"

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_rotozoom.h>
#include <SDL/SDL_image.h>
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

	SDL_Surface* entry = TTF_RenderUTF8_Blended(font, title, c);
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
	SDL_Surface* hex_entry = Incruste(hex, m->entries [0], l, param->font, fontColor);
	SDL_BlitSurface (hex_entry, NULL, m->window, &position);
	SDL_FreeSurface (hex_entry);

	m->l = l;
	m->r = rayon;
	m->pos.x = dx;
	m->pos.y = 0;

	/* Option */
	dx = xorig + 2 * (l + 1) - rayon + l/2 + 1;
	position.x = dx + 1 * (l + 1) / 2;
	hex_entry = Incruste(hex, m->entries [4], l, param->font, fontColor);
	SDL_BlitSurface (hex_entry, NULL, m->window, &position);
	SDL_FreeSurface (hex_entry);

	dx = xorig + 1 * (l + 1) - rayon + l/2 + 1;
	for (int j = 0; j < 3; ++j)
	{
		position.x = dx + j * (l + 1) / 2;
		position.y = 0 + j * (1.5 * rayon);
		hex_entry = Incruste(hex, m->entries [j+1], l, param->font, fontColor);
		SDL_BlitSurface (hex_entry, NULL, m->window, &position);
		SDL_FreeSurface (hex_entry);
	}

	SDL_FreeSurface(hex);
}

/* Externes */

void Affiche_menu (menu_t m)
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
	Affiche_entry(m, POINTE);
	SDL_Flip (m->window);
}

void Affiche_entry (menu_t m, bool pointe)
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
	switch (m->cur.x * 10 + m->cur.y)
	{
	case 01:
		hex_entry = Incruste (hex, m->entries [0], m->l, param->font, c);
		break;
	case 10:
		hex_entry = Incruste (hex, m->entries [1], m->l, param->font, c);
		break;
	case 11:
		hex_entry = Incruste (hex, m->entries [2], m->l, param->font, c);
		break;
	case 12:
		hex_entry = Incruste (hex, m->entries [3], m->l, param->font, c);
		break;
	case 21:
		hex_entry = Incruste (hex, m->entries [4], m->l, param->font, c);
		break;
	}

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
}

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
}*/

/* Externe

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
}*/

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
	int cur = 0;
	char* passcode = "stopdancingstupidbanana";
	bool pause = false;
	while (!end)
	{
		if (!(SDL_GetTicks() % 100) && !pause)
		{
			SDL_BlitSurface (gif [i%8], NULL, window, &p);
			SDL_Flip (window);
			SDL_BlitSurface (erase, NULL, window, &p);
			i = (i + 1) % 8 + 8;
		}
		SDL_Event e;
		if (SDL_PollEvent (&e) && e.type == SDL_KEYDOWN)
		{
			switch (e.key.keysym.sym)
			{
			case SDLK_p:
					pause = true;
					break;
			case SDLK_r:
					pause = false;
					break;
			default:;
			}
			//Uint8* key = SDL_GetKeyState(NULL);
			if (e.key.keysym.sym == passcode[cur])
				++cur;
			else
				cur = 0;
			if (cur == strlen (passcode))
				end = true;
		}
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
