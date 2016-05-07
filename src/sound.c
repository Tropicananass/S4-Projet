/*
 * sound.c
 *
 *  Created on: 23 avr. 2016
 *      Author: nathan
 */

#include "sound.h"
#include <SDL/SDL_mixer.h>
#include "param.h"

void Load_sound (void)
{
	param->music_vol = .5 * MIX_MAX_VOLUME;
	param->music = Mix_LoadMUS ("ressources/theme.mp3");
	Mix_VolumeMusic (param->music_vol);//param->music_vol);

	param->chunk_vol = .25 * MIX_MAX_VOLUME;
	printf ("%d\n", param->chunk_vol);
	param->click = Mix_LoadWAV("ressources/Click.wav");
	Mix_VolumeChunk (param->click, .5 * param->chunk_vol);

	param->gong = Mix_LoadWAV("ressources/Gong.wav");
	Mix_VolumeChunk (param->gong, param->chunk_vol);
}

void Free_sound (void)
{
	Mix_FreeMusic (param->music);
	Mix_FreeChunk (param->click);
	Mix_FreeChunk (param->gong);
}

void play_clik (void)
{
	Mix_PlayChannel(0, param->click, 0);
}

void play_gong (void)
{
	//Mix_PlayChannel(1, param->gong, 0);
}
