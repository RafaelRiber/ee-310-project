#include "sounds.h"

void init_audio()
{
	//Init the sound library
	mmInitDefaultMem((mm_addr)soundbank_bin);
	//Load module
	mmLoad(MOD_BEEPY);
	//Load effect
	mmLoadEffect(SFX_GUN);
	mmLoadEffect(SFX_ERROR);
	mmLoadEffect(SFX_LETS_DO_THIS);
	mmLoadEffect(SFX_GAME_OVER);
	mmLoadEffect(SFX_SPLASH);
	mmLoadEffect(SFX_EXPLOSION);
}

void play_music(){
	//Start playing music in a loop
	mmStart(MOD_BEEPY, MM_PLAY_LOOP);
	//Set module volume to 512 (range 0...1024) using the function mmSetModuleVolume(...)
	mmSetModuleVolume(512);
}


void play_sound_effect(int i) {
	mm_sound_effect sound;
	sound.id = i;
	sound.volume = 255;
	sound.panning = 128;
	if (i == SFX_GUN){
		sound.rate = 1536;
	}
	if (i == SFX_LETS_DO_THIS){
		sound.rate = 1024;
	}
	if (i == SFX_ERROR){
		sound.rate = 1024;
	}
	if (i == SFX_SPLASH){
			sound.rate = 1024;
		}
	if (i == SFX_EXPLOSION){
			sound.rate = 1024;
		}
	if (i == SFX_GAME_OVER) {
		sound.rate = 1024;
	}

	//Play the effect using the sound structure
	mmEffectEx(&sound);
}

