// Battleships Sound Library
// EE-310 - Tharen Candi, Rafael Riber

#ifndef SOUNDS_H_
#define SOUNDS_H_
#include "battleships.h"

#include <maxmod9.h>
#include "soundbank.h"
#include "soundbank_bin.h"

void init_audio();
void play_music();
void play_sound_effect(int i);

#endif /* SOUNDS_H_ */
