/*
 * param.c
 *
 *  Created on: 18 avr. 2016
 *      Author: nathan
 */

#include "param.h"

#include <stdio.h>
#include <sys/stat.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>

#include "sound.h"

#define C_IN SDL_MapRGB (w->format, 50, 50, 50)
#define C_EX SDL_MapRGB (w->format, 100, 100, 100)
#define C_J1 SDL_MapRGB (w->format, 255, 0, 0)
#define C_J2 SDL_MapRGB (w->format, 0, 0, 255)
#define C_BACKGROUND SDL_MapRGB (w->format, 0, 0, 0)

#define F_FONT "ressources/KeepCalm-Medium.ttf"

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
		param->font = TTF_OpenFont(F_FONT, 100);
		if (param->font == NULL)
		{
			perror (F_FONT);
			exit (1);
		}
		param->chunk_vol = 0.5 * MIX_MAX_VOLUME;
		param->music_vol = 0.5 * MIX_MAX_VOLUME;
	}
	else
	{
		char buffer [100];
		char police [BUFSIZ] = "";
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
		fscanf (param_file, "%s", buffer);
		fscanf (param_file, "%s = %s", buffer, police);

		struct stat  i_dont_care;
		if (!strcmp (police,"") || stat (police, &i_dont_care))
			strcpy (police, F_FONT);
		param->font = TTF_OpenFont(police, 100);
		if (param->font == NULL)
		{
			perror (police);
			exit (1);
		}
		fscanf (param_file, "%s = %u", buffer, &(param->chunk_vol));
		if (0 > param->chunk_vol || param->chunk_vol > MIX_MAX_VOLUME)
			param->chunk_vol = 0.5 * MIX_MAX_VOLUME;
		fscanf (param_file, "%s = %u", buffer, &(param->music_vol));
		if (0 > param->music_vol || param->music_vol > MIX_MAX_VOLUME)
			param->music_vol = 0.5 * MIX_MAX_VOLUME;
		fclose (param_file);
	}
	param->size = 11;
	Load_sound ();
}

void save_param (SDL_Surface* w)
{
	FILE* param_file = fopen ("default", "r+");
	if (param_file == NULL)
		param_file = fopen ("default", "w+");

	fprintf (param_file, "Colors\n");
	char* field [5] = {"in", "ex", "j1", "j2", "background",};
	SDL_Color* rgb = &(param->rgb_in);
	for (int i = 0; i < 5 ; ++i)
	{
		fprintf (param_file, "%s = %d %d %d\n", field [i], rgb->r, rgb->g, rgb->b);
		//printf ("%d - %d - %d\n", rgb->r, rsgb->g, rgb->b);
		rgb += 1;
	}

	fprintf (param_file, "Police\n");

	char buffer [100] = "";
	char police [BUFSIZ] = "";
	int readed = fscanf (param_file, "%s = %s\n", buffer, police);
	if (strcmp(police, F_FONT))
	{
		struct stat i_dont_care;
		if (stat (police, &i_dont_care))
		{
			if (readed == 2)
				fseek (param_file, -strlen(buffer) - strlen(police) - 3, SEEK_CUR);
			else if (readed == 1)
				fseek (param_file, -strlen(buffer), SEEK_CUR);
			fprintf (param_file, "police = %s\n", F_FONT);
		}
	}
	fprintf (param_file, "son_vol = %u\n", param->chunk_vol);
	fprintf (param_file, "music_vol = %u\n", param->music_vol);
	free (param);
	fclose (param_file);

	Free_sound ();
}
