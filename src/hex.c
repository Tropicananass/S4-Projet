/********************************************************
 *			Jeux Hex			*
 *	
 ********************************************************/

#include <SDL/SDL.h>
#include <stdlib.h>
#include <time.h>
#include "window.h"
#include "globals.h"
#include "menu_principal.h"
#include "en_jeu.h"
#include "param.h"
#include "sauvegarde.h"

int main(int argc, char **argv)
{
	srand(time(0));
	SDL_Surface* window = init_window();
	load_param(window);
	Reset_window (window);
	bool end = 0;
	char* file;
	while (!end)
	{
		switch (menu_principal(window, &file))
		{
		case M_UP:
			en_jeu (window, NULL);
			Reset_window(window);
			break;
		case M_LEFT:
		{
			int * hist;
			int ia;
			int err = chargement (file, &hist, &ia);
			if (err != OK)
				printf ("Erreur chargement : %d\n", err);
			else
				en_jeu (window, hist);
				Reset_window(window);
			break;
		}
		case M_DOWN:
			end = 1;
		}
	}
	//en_jeu (window, NULL);
	save_param(window);
	Free_window (window);
	return 0;

}
