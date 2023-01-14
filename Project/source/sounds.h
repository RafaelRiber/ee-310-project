/*
 * sounds.h
 *
 *  Created on: 14 Jan 2023
 *      Author: Rafael
 */

#ifndef SOUNDS_H_
#define SOUNDS_H_
#include "battleships.h"

#include <maxmod9.h>
#include "soundbank.h"
#include "soundbank_bin.h"

void init_audio();
//void Audio_PlaySoundEX( int i );
void play_music();

void play_sound_effect(int i);



#endif /* SOUNDS_H_ */
