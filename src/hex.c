/********************************************************
 *			Jeux Hex			*
 *	
 ********************************************************/

#include <SDL/SDL.h>
#include "window.h"
#include "globals.h"
#include "menu_principal.h"
#include "en_jeu.h"
#include "param.h"

int main(int argc, char **argv)
{
	SDL_Surface* window = init_window();
	load_param(window);
	Reset_window (window);
	bool end = 0;
	while (!end)
	{
		int retour = menu_principal(window);
		Reset_window(window);
		switch (retour)
		{
		case M_JOUER:
			en_jeu (window);
			Reset_window(window);
			break;
		case M_QUITTER:
			end = 1;
		}
	}
	save_param(window);
	Free_window (window);
	return 0;
}
