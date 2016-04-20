#include "affichage_menu_principal.h"

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_rotozoom.h>
#include "param.h"
#include "draw.h"

void rectangle (SDL_Surface* window, SDL_Rect* pos)
{
	SDL_Surface* rect = SDL_CreateRGBSurface (SDL_HWSURFACE, pos->w, pos->h, 32, 0, 0, 0, 0);
	SDL_FillRect (rect, NULL, param->ex);
	SDL_BlitSurface (rect, NULL, window, pos);
	SDL_FreeSurface (rect);
	rect = SDL_CreateRGBSurface (SDL_HWSURFACE, pos->w - 6, pos->h - 6, 32, 0, 0, 0, 0);
	SDL_FillRect (rect, NULL, param->in);
	pos->x += 3;
	pos->y += 3;
	SDL_BlitSurface (rect, NULL, window,  pos);
	SDL_FreeSurface (rect);
	pos->x -= 3;
	pos->y -= 3;
}

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
	SDL_Surface* hex_entry = SDL_CreateRGBSurface (SDL_HWSURFACE, hex->w, hex->h, 32, 0, 0, 0, 0);
	SDL_BlitSurface (hex, NULL, hex_entry, NULL);

	SDL_Surface* entry = TTF_RenderText_Blended(font, title, c);
	Redim_entry(&entry, hex->w / 2, l - 20);

	SDL_Rect position = {hex->w / 2 - l / 2 + (l - entry->w) / 2 - 1, hex->w / 2 / 2 + (hex->w / 2 - entry->h) / 2};

	SDL_BlitSurface (entry , NULL, hex_entry, &position);
	SDL_SetColorKey (hex_entry , SDL_SRCCOLORKEY, SDL_MapRGB(hex->format, 0, 0, 0));

	SDL_FreeSurface (entry);
	return hex_entry;
}

void case_menu (menu_t m, int rayon, int xorig)
{
	char* police = "ressources/KeepCalm-Medium.ttf";
	TTF_Font *fontMenu = TTF_OpenFont(police, rayon - 20);
	if (fontMenu == NULL)
	{
		perror (police);
		exit (1);
	}
	SDL_Color fontColor = {100, 100, 100};

	int l;
	SDL_Surface* hex = SDL_CreateRGBSurface (SDL_HWSURFACE, 2*rayon, 2*rayon, 32, 0, 0, 0, 0);
	Hexagon (hex, rayon, param->ex, param->in, 10, &l);

	/* Charger */
	int dx = xorig - rayon + l/2 + 1;
	SDL_Rect position = {dx + 1 * (l + 1) / 2, 0 + 1 * (1.5 * rayon)};
	SDL_Surface* hex_entry = Incruste(hex, "Charger", l, fontMenu, fontColor);
	SDL_BlitSurface (hex_entry, NULL, m->window, &position);
	SDL_FreeSurface (hex_entry);

	m->l = l;
	m->r = rayon;
	m->pos.x = dx;
	m->pos.y = 0;

	/* Option */
	dx = xorig + 2 * (l + 1) - rayon + l/2 + 1;
	position.x = dx + 1 * (l + 1) / 2;
	hex_entry = Incruste(hex, "Options", l, fontMenu, fontColor);
	SDL_BlitSurface (hex_entry, NULL, m->window, &position);
	SDL_FreeSurface (hex_entry);

	dx = xorig + 1 * (l + 1) - rayon + l/2 + 1;
	char* entry [3] = {"Jouer", "HEX", "Quitter"};
	for (int j = 0; j < 3; ++j)
	{
		position.x = dx + j * (l + 1) / 2;
		position.y = 0 + j * (1.5 * rayon);
		hex_entry = Incruste(hex, entry[j], l, fontMenu, fontColor);
		SDL_BlitSurface (hex_entry, NULL, m->window, &position);
		SDL_FreeSurface (hex_entry);
	}

	SDL_FreeSurface(hex);
	TTF_CloseFont (fontMenu);
}

void Affiche_menu_principal (menu_t m)
{
	int width = m->window->w - 0;
	int height = m->window->h - 0;
	
	height -= height/12;

	int r1 = width / (4 * RAC3);
	int r2 = height / 5;

	int xorig = 0;

	if (r1 < r2)
		r1 = r1 - r1 % 2;
	else
	{
		xorig = (width - 4 * RAC3 * r2) / 2;
		r1 = r2 - r2 % 2;
	}

	case_menu (m, r1, xorig);
	Affiche_entry(m, "HEX", POINTE);
	SDL_Flip (m->window);
}

void Affiche_entry (menu_t m, char* entry, bool pointe)
{
	char* police = "ressources/KeepCalm-Medium.ttf";
	TTF_Font *font = TTF_OpenFont(police, m->r - 20);
	if (font == NULL)
	{
		perror (police);
		exit (1);
	}
	int dx = m->pos.x + m->cur.x * (m->l + 1);
	SDL_Rect position = {dx + m->cur.y * (m->l + 1) / 2, m->pos.y + m->cur.y * (1.5 * m->r)};
	SDL_Surface* hex = SDL_CreateRGBSurface (SDL_HWSURFACE, 2*m->r, 2*m->r, 32, 0, 0, 0, 0);
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
	hex_entry = Incruste (hex, entry, m->l, font, c);
	SDL_BlitSurface (hex_entry, NULL, m->window, &position);
	SDL_Flip (m->window);
	TTF_CloseFont (font);
}
