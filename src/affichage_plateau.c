#include "affichage_plateau.h"

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "globals.h"

//gcc src/affichage_plateau.c -Wall -std=c99 `sdl-config --cflags --libs` -g -lm -o src/plateau

/* Internes */
/* source : http://www.gnurou.org/writing/linuxmag/sdl/partie1 */

void putPixel(SDL_Surface * surface, Uint16 x, Uint16 y, Uint32 color)
{
    /* Nombre de bits par pixels de la surface d'écran */
    Uint8 bpp = surface->format->BytesPerPixel;
    /* Pointeur vers le pixel à remplacer (pitch correspond à la taille 
       d'une ligne d'écran, c'est à dire (longueur * bitsParPixel) 
       pour la plupart des cas) */
    Uint8 * p = ((Uint8 *)surface->pixels) + y * surface->pitch + x * bpp;
     switch(bpp)
    {
	  case 1:
		*p = (Uint8) color;
		break;
        case 2:
            *(Uint16 *)p = (Uint16) color;
            break;
        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
                *(Uint16 *)p = ((color >> 8) & 0xff00) | ((color >> 8) & 0xff);
                *(p + 2) = color & 0xff;
            }
            else
            {
                *(Uint16 *)p = color & 0xffff;
                *(p + 2) = ((color >> 16) & 0xff) ;
            }
            break;
        case 4:
            *(Uint32 *)p = color;
            break;
    }
}

void Central_Square (SDL_Surface* hex, int* x, int* y, Uint32 color, int r)
{
	int width = x[0] - x[2];
	int height = r;//y[2] - y[3];
	
	SDL_Surface* square = SDL_CreateRGBSurface (SDL_HWSURFACE, width, height, 32, 0, 0, 0, 0);
	SDL_FillRect (square, NULL, color);
	SDL_Rect position = {x[3], y[3]};
	SDL_BlitSurface (square, NULL, hex, &position);
	SDL_FreeSurface (square);
}

void Triangles (SDL_Surface* hex, int* x, int* y, Uint32 color, int r)
{
	int h = r/2;//y[1] - y[2];
	int l = (x[0] - x[2])/2;
	SDL_LockSurface (hex);
	for (int j = 0; j < h; ++j)
		for (int i = - l * (h - j) / h; i <= l * (h - j) / h; ++i)
		{
			putPixel (hex, x[1] + i - 1, y[3] - j, color);
			putPixel (hex, x[1] + i - 1, y[0] + j, color);
		}
	SDL_UnlockSurface (hex);
}

void Hexagon (SDL_Surface* hex, int rayon, Uint32 color_out, Uint32 color_in, int* l)
{
	int x [4];
	int y [4];
	for (int i = 0; i < 4; ++i)
	{
		x [i] = cos(i * PI/3 + PI/6) * rayon + rayon;
		y [i] = sin(i * PI/3 + PI/6) * rayon + rayon;
	}
	*l = x[0] - x[2];
	Central_Square (hex, x, y, color_out, rayon);
	Triangles (hex, x, y, color_out, rayon);
	for (int i = 0; i < 4; ++i)
	{
		x [i] = cos(i * PI/3 + PI/6) * (rayon - 4) + rayon;
		y [i] = sin(i * PI/3 + PI/6) * (rayon - 4) + rayon;
	}
	Central_Square (hex, x, y, color_in, rayon - 4);
	Triangles (hex, x, y, color_in, rayon - 4);
	SDL_Flip (hex);
	SDL_SetColorKey (hex , SDL_SRCCOLORKEY, SDL_MapRGB( hex->format, 0, 0, 0)); // set black as transparent
}

void Quadrille (plateau_t p)
{
	SDL_Surface* hex = SDL_CreateRGBSurface (SDL_HWSURFACE, 2 * p->r, 2 * p->r, 32, 0, 0, 0, 0);
	Hexagon (hex, p->r, SDL_MapRGB (p->window->format, 100, 100, 100), SDL_MapRGB (p->window->format, 50, 50, 50), &(p->l));
	for (int i = 0; i < NBSIDE; ++i)
	{
		int dx = p->marge_hori + i * (p->l + 1);//(2 * rayon * cos(PI/6));
		for (int j = 0; j < NBSIDE; ++j)
		{
			SDL_Rect position = {dx + j * (p->l + 1) / 2, p->marge_vert + j * (1.5 * p->r)};
			SDL_BlitSurface (hex, NULL, p->window, &position);
		}
	}
	SDL_FreeSurface(hex);
	SDL_Flip (p->window);
}

/*Externes*/

void Affiche_hexagon (plateau_t p, int x, int y, int state)
{
	SDL_Rect position = {p->marge_hori + x * (p->l + 1) + y * (p->l + 1) / 2, p->marge_vert + y * (1.5 * p->r)};
	SDL_Surface* hex = SDL_CreateRGBSurface (SDL_HWSURFACE, 2 * p->r, 2 * p->r, 32, 0, 0, 0, 0);
	Uint32 c;
	switch (state)
	{
	case J1 :
		c = SDL_MapRGB (p->window->format, 255, 0, 0);
		break;
	case J2 :
		c = SDL_MapRGB (p->window->format, 0, 0, 255);
		break;
	case NORMAL :
		c = SDL_MapRGB (p->window->format, 100, 100, 100);
		break;
	}
	Hexagon (hex, p->r, c, SDL_MapRGB (p->window->format, 50, 50, 50), &(p->l));
	SDL_BlitSurface (hex, NULL, p->window, &position);
	SDL_FreeSurface (hex);
	SDL_Flip (p->window);
}

plateau_t init_plateau (SDL_Surface* window)
{
	plateau_t p = malloc (sizeof (struct s_plateau));
	p->window = window;
	p->marge_hori = 40;
	p->marge_vert = 40;
	int width = window->w - 2 * p->marge_hori;
	int height = window->h - 2 * p->marge_vert;
	int r1 = (height - (NBSIDE - 1)) / (.5 + NBSIDE * 1.5);
	int r2 = width / ((3 * NBSIDE - 1) * cos (PI/6));
	if (r1 < r2)
		p->r = r1 - r1%2;
	else
		p->r = r2 - r2%2;
	Quadrille (p);
	return p;
}

plateau_t actu_plateau (plateau_t p)
{
	p->marge_hori = 40;
	p->marge_vert = 40;
	int width = p->window->w - 2 * p->marge_hori;
	int height = p->window->h - 2 * p->marge_vert;
	int r1 = (height - (NBSIDE - 1)) / (.5 + NBSIDE * 1.5);
	int r2 = width / ((3 * NBSIDE - 1) * cos (PI/6));
	if (r1 < r2)
		p->r = r1 - r1%2;
	else
		p->r = r2 - r2%2;
	Quadrille (p);
	return p;
}

void free_plateau (plateau_t p)
{
	free (p);
}


/*int main (int argc, char* argv[])
{
	if (SDL_Init (SDL_INIT_VIDEO))
		fprintf (stderr, "Erreur d'inistialisation SDL : %s\n", SDL_GetError());
	
	SDL_DisplayFormat() dm;
	if (SDL_GetDisplayMode(0, &dm) != 0) {
	    SDL_Log("SDL_GetDisplayMode failed: %s", SDL_GetError());
	    return 1;
	}
	SDL_Surface* window = SDL_SetVideoMode (DWIDTH, DHEIGHT, BPP, SDL_HWSURFACE | SDL_RESIZABLE);
	SDL_FillRect (window, NULL, SDL_MapRGB(window->format, 0, 0, 0));
	SDL_Flip (window);
	SDL_Color c = {100, 100, 100};
	Affiche_plateau (window, c);
	
	bool end = false;
	
	while (!end)
	{
		SDL_Event event;
		SDL_WaitEvent (&event);
		switch (event.type)
		{
			case SDL_VIDEORESIZE:
				if (event.resize.w >= DWIDTH && event.resize.h >= DHEIGHT )
				{
					window = SDL_SetVideoMode (event.resize.w, event.resize.h, BPP, SDL_HWSURFACE | SDL_RESIZABLE);
					Affiche_plateau (window, c);
				}
				else // force la fenetre a minimum Default Size : marche moyen
					window = SDL_SetVideoMode (DWIDTH, DHEIGHT, BPP, SDL_HWSURFACE | SDL_RESIZABLE);
				break;
			case SDL_QUIT:
				end = true;
				break;
		}
	}
	SDL_FreeSurface (window);
	SDL_Quit ();
	return 0;
}*/
