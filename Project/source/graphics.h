#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "nds.h"
extern u16* gfx;

#define JOIN_BUTTON_TOP 76
#define JOIN_BUTTON_BOTTOM 104
#define JOIN_BUTTON_LEFT 85
#define JOIN_BUTTON_RIGHT 171

#define HOST_BUTTON_TOP 113
#define HOST_BUTTON_BOTTOM 142
#define HOST_BUTTON_LEFT 85
#define HOST_BUTTON_RIGHT 171

void display_gameboard();
void display_gameboard_sub();
void display_title();
void display_waiting_for_enemy();
void configure_graphics();
void update_sprites();
void allocate_sprite(u16 ** gfx, unsigned short * pal, unsigned int palLen, unsigned int * tiles, unsigned int tilesLen, char isMain );
#endif
