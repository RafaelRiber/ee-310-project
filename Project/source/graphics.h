#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "nds.h"
#include "battleships.h"
extern u16* gfx;

#define JOIN_BUTTON_TOP 76
#define JOIN_BUTTON_BOTTOM 104
#define JOIN_BUTTON_LEFT 85
#define JOIN_BUTTON_RIGHT 171

#define HOST_BUTTON_TOP 113
#define HOST_BUTTON_BOTTOM 142
#define HOST_BUTTON_LEFT 85
#define HOST_BUTTON_RIGHT 171

void configure_graphics();
void update_ships();
void allocate_sprite(u16 ** gfx, unsigned short * pal, unsigned int palLen, unsigned int * tiles, unsigned int tilesLen, char isMain );
void new_shot_sprite(int isHit, int x, int y);
void load_backgrounds(int screen);
#endif
