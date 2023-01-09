#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "nds.h"
extern u16* gfx;
void configure_graphics();
void update_sprites();
void allocate_sprite(u16 ** gfx, unsigned short * pal, unsigned int palLen, unsigned int * tiles, unsigned int tilesLen, char isMain );
#endif
