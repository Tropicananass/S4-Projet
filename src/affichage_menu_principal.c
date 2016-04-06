#include "affichage_menu_principal.h"

#include <SDL_ttf.h>
#include "globals.h"

void Affiche_menu_principal (SDL_Surface* window)
{
	int width = window->w;
	int height = window->h;
	
	SDL_Surface* entry;
	
	if (TTF_Init ()) //Check error
		fprintf (stderr, "Erreur d'inistialisation SDL : %s\n", SDL_GetError());
	
	SDL_Rect position = {0, 0};
	
	SDL_Color fontColor = {255, 255, 255};
	
	int size_titre = 10;
	int size_entries = 22;
	int margev_titre = 20;
	int margev_entries = 4;
	
	/* Titre */
	
	TTF_Font *fontMenu = TTF_OpenFont("ressources/cac_champagne.ttf",60); // charger une fonte (un peu moche)
	if (fontMenu == NULL)
	{
		perror ("camarche pas");
		exit (1);
    	}
	entry = TTF_RenderText_Blended(fontMenu, "HEX", fontColor);
	position.x = window->w / 2 - size_titre * 3;
	position.y = margev_titre;
	
	SDL_BlitSurface (entry, NULL, window, &position);
	SDL_Flip (window);
	//SDL_FreeSurface(entry);
	
	/* Jouer */
	//entry = SDL_CreateRGBSurface (SDL_HWSURFACE, ,  , window->format->BitsPerPixel, 0, 0, 0, 0);
	//SDL_FillRect (entry, NULL, BPP_entries);
	
	/* Charger */
	/* Option */
	/* Quitter */
}
