/*
 * action_menu_principal.c
 *
 *  Created on: 21 avr. 2016
 *      Author: nathan
 */

#include "action_menu_principal.h"

#include <math.h>
#include "globals.h"
#include "affichage_menu_principal.h"
#include "sound.h"

void deplacement_menu (menu_t m, SDL_Event* event)
{
	char* entries [3][3] =  {{"", "Charger", ""}, {"Jouer", "HEX", "Quitter"}, {"", "Options", ""}};
	switch (event->type)
	{
	case SDL_MOUSEMOTION:
	{
		vec2 pos = {event->motion.x - m->pos.x + m->l/2 - m->r, event->motion.y - m->pos.y};
		int ligne =  round(pos.y / (1.5 * m->r) - .5);
		int colone = round(pos.x / (m->l + 1.) - .5);
		vec2 relative = {pos.x % (m->l + 1), pos.y % (int)(1.5 * m->r)};
		vec2 proj = {relative.y + relative.x / RAC3, relative.y - relative.x / RAC3};
		vec2 hex;
		if (ligne % 2)
		{
			if (proj.y < 0 && proj.x < m->r)
			{
				/* haut */
				hex.y = ligne - 1;
				hex.x = colone - ligne / 2;
			}
			else if (relative.x > m->l / 2)
			{
				/* bas droite */
				hex.y = ligne;
				hex.x = colone - ligne / 2;
			}
			else
			{
				/* bas gauche */
				hex.y = ligne;
				hex.x = colone - ligne / 2 - 1;
			}
		}
		else
		{
			if (-proj.y > m->r/2)
			{
				/* haut droite */
				hex.y = ligne - 1;
				hex.x = colone - ligne / 2 + 1;
			}
			else if (proj.x < m->r/2)
			{
				/* haut gauche */
				hex.y = ligne - 1;
				hex.x = colone - ligne / 2;
			}
			else
			{
				/* bas */
				hex.y = ligne;
				hex.x = colone - ligne / 2;
			}
		}

		if (0 > hex.x || hex.x >= 3 || 0 > hex.y || hex.y >= 3  || (hex.x != 1 && hex.y != 1))
		{
			hex.x = 1;
			hex.y = 1;
		}
		if (hex.x != m->cur.x || hex.y != m->cur.y)
		{
			Affiche_entry(m, entries [m->cur.x][m->cur.y], NORMAL);
			m->cur.x = hex.x;
			m->cur.y = hex.y;
			Affiche_entry(m, entries [hex.x][hex.y], POINTE);
			play_clik();
		}
		break;
	}
	case SDL_KEYDOWN:
		while (!SDL_PollEvent(event))
		{
			Affiche_entry(m, entries [m->cur.x][m->cur.y], NORMAL);
			switch (event->key.keysym.sym)
			{
			case SDLK_RIGHT:
				++(m->cur.x);
				if (m->cur.y != 1)
					m->cur.y = 1;
				break;
			case SDLK_LEFT:
				--(m->cur.x);
				if (m->cur.y != 1)
					m->cur.y = 1;
				break;
			case SDLK_UP:
				--(m->cur.y);
				if (m->cur.x != 1)
					m->cur.x = 1;
				break;
			case SDLK_DOWN:
				++(m->cur.y);
				if (m->cur.x != 1)
					m->cur.x = 1;
				break;
			default:
				break;
			}
			if (m->cur.x >= 3)
				m->cur.x = 0;
			if (m->cur.x < 0)
				m->cur.x = 2;
			if (m->cur.y >= 3)
				m->cur.y = 0;
			if (m->cur.y < 0)
				m->cur.y = 2;
			Affiche_entry(m, entries [m->cur.x][m->cur.y], POINTE);
			play_clik();
			SDL_Delay (200);
		}
		break;
	}
}

bool selection_menu (menu_t m, int* r)
{
	switch (10 * m->cur.x + m->cur.y)
	{
	case 01:
		*r = M_CHARGER;
		return false;
	case 10:
		*r = M_JOUER;
		play_gong();
		return true;
	case 11:
		*r = M_HEX;
		return false;
	case 12:
		*r = M_QUITTER;
		return true;
	case 21:
		*r = M_OPTIONS;
		return false;
	default:
		return false;
	}
}
