#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "nds.h"
#include "battleships.h"
extern u16* gfx;
void configure_graphics();
void update_ships();
void allocate_sprite(u16 ** gfx, unsigned short * pal, unsigned int palLen, unsigned int * tiles, unsigned int tilesLen, char isMain );
void new_shot_sprite(int isHit, int x, int y);
void load_backgrounds(game_screens screen);
#endif
