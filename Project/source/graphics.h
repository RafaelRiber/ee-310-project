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
void update_sprites();
void allocate_sprite(u16 ** gfx, const unsigned int * tiles, unsigned int tilesLen, char isMain );
void new_shot_tile(int isHit, int x, int y, int is_main);
void load_backgrounds(int screen);
void hide_player_ships();
void show_player_ships();
void clear_shots();
#endif
