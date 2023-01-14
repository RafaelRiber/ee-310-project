#include "sounds.h"

void init_audio()
{
	//Init the sound library
	mmInitDefaultMem((mm_addr)soundbank_bin);
	//Load module
	mmLoad(MOD_BEEPY);
	//Load effect
	mmLoadEffect(SFX_GUN);
	mmLoadEffect(SFX_LETS_DO_THIS);
}

void play_music(){
	//Start playing music in a loop
	mmStart(MOD_BEEPY, MM_PLAY_LOOP);
	//Set module volume to 512 (range 0...1024) using the function mmSetModuleVolume(...)
	mmSetModuleVolume(512);
}


void play_sound_effect(int i) {
	//Declare a sound effect
	mm_sound_effect sound;
	//Set the id of the sound effect with the input parameter
	sound.id = i;
	//Set the rate to the default one (1024)

	if (i == SFX_GUN)
		sound.rate = 1536;
	if (i == SFX_LETS_DO_THIS)
		sound.rate = 1024;
	//Set the volume to the maximum (range 0...255)
	sound.volume = 255;
	//Set the panning depending on the effect (0-left....255-right)
	if (i == SFX_GUN)
		sound.panning = 128;
	if (i == SFX_LETS_DO_THIS)
		sound.panning = 128;

	//Play the effect using the sound structure
	mmEffectEx(&sound);
}

