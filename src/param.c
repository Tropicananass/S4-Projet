/*
 * param.c
 *
 *  Created on: 18 avr. 2016
 *      Author: nathan
 */

#include "param.h"

#include <stdio.h>

#define C_IN SDL_MapRGB (w->format, 50, 50, 50)
#define C_EX SDL_MapRGB (w->format, 100, 100, 100)
#define C_J1 SDL_MapRGB (w->format, 255, 0, 0)
#define C_J2 SDL_MapRGB (w->format, 0, 0, 255)
#define C_BACKGROUND SDL_MapRGB (w->format, 0, 0, 0)

void load_param (SDL_Surface* w)
{
	Uint32 c_default [5] = {C_IN, C_EX, C_J1, C_J2, C_BACKGROUND};
	FILE* param_file = fopen ("default", "r");
	param = malloc (sizeof (struct s_param));
	if (param_file == NULL)
	{
		SDL_Color* rgb = &(param->rgb_in);
		Uint32* c = &(param->in);
		for (int i = 0; i < 5; ++i)
		{
			*c = c_default [i];
			SDL_GetRGB(*c, w->format, &(rgb->r), &(rgb->g), &(rgb->b));
			rgb += 1;
			c += 1;
		}
	}
	else
	{
		char buffer [100];
		SDL_Color* rgb = &(param->rgb_in);
		Uint32* c = &(param->in);
		fscanf (param_file, "%s\n", buffer);
		for (int i = 0; i < 5; ++i)
		{
			int r = -1, g = -1, b = -1;
			fscanf (param_file, "%s = %d %d %d", buffer, &r, &g, &b);
			if (0 > r || r > 255 || 0 > g || g > 255 || 0 > b || b > 255)
			{
				*c = c_default [i];
				SDL_GetRGB(*c, w->format, &(rgb->r), &(rgb->g), &(rgb->b));
			}
			else
			{
				rgb->r = r; rgb->g = g; rgb->b = b;
				*c = SDL_MapRGB (w->format, rgb->r, rgb->g, rgb->b);
			}
			rgb += 1;
			c += 1;
		}
		fclose (param_file);
	}
}

void save_param (SDL_Surface* w)
{
	FILE* param_file = fopen ("default", "w");
	fprintf (param_file, "color\n");
	char* field [5] = {"in", "ex", "j1", "j2", "background",};
	SDL_Color* rgb = &(param->rgb_in);
	for (int i = 0; i < 5 ; ++i)
	{
		fprintf (param_file, "%s = %d %d %d\n", field [i], rgb->r, rgb->g, rgb->b);
		//printf ("%d - %d - %d\n", rgb->r, rgb->g, rgb->b);
		rgb += 1;
	}
	free (param);
	fclose (param_file);
}
