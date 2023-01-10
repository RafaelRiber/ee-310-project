#include <nds.h>
#include "graphics.h"
#include "stdio.h"

#include "test_gameboard.h"

#include "carrier_sprite_0.h"
#include "carrier_sprite_1.h"
#include "carrier_sprite_2.h"
#include "carrier_sprite_3.h"
#include "carrier_sprite_4.h"

#include "destroyer_sprite_0.h"
#include "destroyer_sprite_1.h"

#include "submarine_sprite_0.h"
#include "submarine_sprite_1.h"
#include "submarine_sprite_2.h"

#include "cruiser_sprite_0.h"
#include "cruiser_sprite_1.h"
#include "cruiser_sprite_2.h"
 
#include "battleship_sprite_0.h"
#include "battleship_sprite_1.h"
#include "battleship_sprite_2.h"
#include "battleship_sprite_3.h"

#include "battleship_title.h"
#include "battleship_title_sub.h"

#include "hosting_waiting_sub.h"

#include "block.h"
#include "battleships.h"
#define GB_BG 0
#define GB_BG_MP_BASE 0 
#define GB_BG_TILE_BASE 1


unsigned int *BATTLESHIP_SPRITE_TILES[BATTLESHIP_SIZE] = {battleship_sprite_0Tiles,battleship_sprite_1Tiles,battleship_sprite_2Tiles,battleship_sprite_3Tiles};
unsigned int *CRUISER_SPRITE_TILES[CRUISER_SIZE] = {cruiser_sprite_0Tiles, cruiser_sprite_1Tiles, cruiser_sprite_2Tiles};
unsigned int *SUBMARINE_SPRITE_TILES[SUBMARINE_SIZE] = {submarine_sprite_0Tiles,submarine_sprite_1Tiles,submarine_sprite_2Tiles};
unsigned int *DESTROYER_SPRITE_TILES[DESTROYER_SIZE] = {destroyer_sprite_0Tiles, destroyer_sprite_1Tiles};
unsigned int *CARRIER_SPRITE_TILES[CARRIER_SIZE] = {carrier_sprite_0Tiles, carrier_sprite_1Tiles, carrier_sprite_2Tiles, carrier_sprite_3Tiles, carrier_sprite_3Tiles};
unsigned int **SHIP_TILES[NUM_SHIPS] = {
	[BATTLESHIP] = BATTLESHIP_SPRITE_TILES,
	[CRUISER] = CRUISER_SPRITE_TILES,
	[SUBMARINE] = SUBMARINE_SPRITE_TILES,
	[DESTROYER] = DESTROYER_SPRITE_TILES,
	[CARRIER] = CARRIER_SPRITE_TILES,
};


unsigned int BATTLESHIP_SPRITE_TILESLEN[BATTLESHIP_SIZE] = {battleship_sprite_0TilesLen,battleship_sprite_1TilesLen,battleship_sprite_2TilesLen,battleship_sprite_3TilesLen};
unsigned int CRUISER_SPRITE_TILESLEN[CRUISER_SIZE] = {cruiser_sprite_0TilesLen, cruiser_sprite_1TilesLen, cruiser_sprite_2TilesLen};
unsigned int SUBMARINE_SPRITE_TILESLEN[SUBMARINE_SIZE] = {submarine_sprite_0TilesLen,submarine_sprite_1TilesLen,submarine_sprite_2TilesLen};
unsigned int DESTROYER_SPRITE_TILESLEN[DESTROYER_SIZE] = {destroyer_sprite_0TilesLen, destroyer_sprite_1TilesLen};
unsigned int CARRIER_SPRITE_TILESLEN[CARRIER_SIZE] = {carrier_sprite_0TilesLen, carrier_sprite_1TilesLen, carrier_sprite_2TilesLen, carrier_sprite_3TilesLen, carrier_sprite_3TilesLen};

unsigned int *SHIP_TILELENS[NUM_SHIPS] = {
	[BATTLESHIP] = BATTLESHIP_SPRITE_TILESLEN,
	[CRUISER] = CRUISER_SPRITE_TILESLEN,
	[SUBMARINE] = SUBMARINE_SPRITE_TILESLEN,
	[DESTROYER] = DESTROYER_SPRITE_TILESLEN,
	[CARRIER] = CARRIER_SPRITE_TILESLEN,
};

unsigned short *BATTLESHIP_SPRITE_PAL[BATTLESHIP_SIZE] = {battleship_sprite_0Pal,battleship_sprite_1Pal,battleship_sprite_2Pal,battleship_sprite_3Pal};
unsigned short *CRUISER_SPRITE_PAL[CRUISER_SIZE] = {cruiser_sprite_0Pal, cruiser_sprite_1Pal, cruiser_sprite_2Pal};
unsigned short *SUBMARINE_SPRITE_PAL[SUBMARINE_SIZE] = {submarine_sprite_0Pal,submarine_sprite_1Pal,submarine_sprite_2Pal};
unsigned short *DESTROYER_SPRITE_PAL[DESTROYER_SIZE] ={destroyer_sprite_0Pal, destroyer_sprite_1Pal};
unsigned short *CARRIER_SPRITE_PAL[CARRIER_SIZE] = {carrier_sprite_0Pal, carrier_sprite_1Pal, carrier_sprite_2Pal, carrier_sprite_3Pal, carrier_sprite_3Pal};
unsigned short **SHIP_PALS[NUM_SHIPS] = {
	[BATTLESHIP] = BATTLESHIP_SPRITE_PAL,
	[CRUISER] =CRUISER_SPRITE_PAL,
	[SUBMARINE]= SUBMARINE_SPRITE_PAL,
	[DESTROYER]= DESTROYER_SPRITE_PAL,
	[CARRIER] =CARRIER_SPRITE_PAL,
};
unsigned int BATTLESHIP_SPRITE_PALLEN[BATTLESHIP_SIZE] = {battleship_sprite_0PalLen,battleship_sprite_1PalLen,battleship_sprite_2PalLen,battleship_sprite_3PalLen};
unsigned int CRUISER_SPRITE_PALLEN[CRUISER_SIZE] = {cruiser_sprite_0PalLen, cruiser_sprite_1PalLen, cruiser_sprite_2PalLen};
unsigned int SUBMARINE_SPRITE_PALLEN[SUBMARINE_SIZE] = {submarine_sprite_0PalLen,submarine_sprite_1PalLen,submarine_sprite_2PalLen};
unsigned int DESTROYER_SPRITE_PALLEN[DESTROYER_SIZE] ={destroyer_sprite_0PalLen, destroyer_sprite_1PalLen};
unsigned int CARRIER_SPRITE_PALLEN[CARRIER_SIZE] = {carrier_sprite_0PalLen, carrier_sprite_1PalLen, carrier_sprite_2PalLen, carrier_sprite_3PalLen, carrier_sprite_3PalLen};
unsigned int *SHIP_PALLENS[NUM_SHIPS] = {
	[BATTLESHIP] = BATTLESHIP_SPRITE_PALLEN,
	[CRUISER] = CRUISER_SPRITE_PALLEN,
	[SUBMARINE]= SUBMARINE_SPRITE_PALLEN,
	[DESTROYER]= DESTROYER_SPRITE_PALLEN,
	[CARRIER] =CARRIER_SPRITE_PALLEN,
};

void display_gameboard() {
	swiCopy(test_gameboardTiles, BG_TILE_RAM(GB_BG_TILE_BASE), test_gameboardTilesLen / 2);
	swiCopy(test_gameboardPal, BG_PALETTE, test_gameboardPalLen / 2);
	swiCopy(test_gameboardMap, BG_MAP_RAM(GB_BG_MP_BASE), test_gameboardMapLen / 2);
}

void display_gameboard_sub() {
	swiCopy(test_gameboardTiles, BG_TILE_RAM_SUB(GB_BG_TILE_BASE), test_gameboardTilesLen / 2);
	swiCopy(test_gameboardPal, BG_PALETTE_SUB, test_gameboardPalLen / 2);
	swiCopy(test_gameboardMap, BG_MAP_RAM_SUB(GB_BG_MP_BASE), test_gameboardMapLen / 2);
}

void display_title() {
	swiCopy(battleship_titleTiles, BG_TILE_RAM(GB_BG_TILE_BASE), battleship_titleTilesLen / 2);
	swiCopy(battleship_titlePal, BG_PALETTE, test_gameboardPalLen / 2);
	swiCopy(battleship_titleMap, BG_MAP_RAM(GB_BG_MP_BASE), battleship_titleMapLen / 2);

	swiCopy(battleship_title_subTiles, BG_TILE_RAM_SUB(GB_BG_TILE_BASE), battleship_title_subTilesLen / 2);
	swiCopy(battleship_title_subPal, BG_PALETTE_SUB, battleship_title_subPalLen / 2);
	swiCopy(battleship_title_subMap, BG_MAP_RAM_SUB(GB_BG_MP_BASE), battleship_title_subMapLen / 2);

}

void display_waiting_for_enemy() {
	swiCopy(hosting_waiting_subTiles, BG_TILE_RAM_SUB(GB_BG_TILE_BASE), hosting_waiting_subTilesLen / 2);
	swiCopy(hosting_waiting_subPal, BG_PALETTE_SUB, hosting_waiting_subPalLen / 2);
	swiCopy(hosting_waiting_subMap, BG_MAP_RAM_SUB(GB_BG_MP_BASE), hosting_waiting_subMapLen / 2);
}

/*
main engine: mode 0 
    - bg0: gameboard
    - bgx: effects (explosions..)
    - Sprites: ships
*/
void configure_graphics() {
    //MAIN engine
	REG_DISPCNT = MODE_0_2D | DISPLAY_BG0_ACTIVE;
	VRAM_A_CR = VRAM_ENABLE | VRAM_A_MAIN_BG;

	//SUB Engine
	REG_DISPCNT_SUB = MODE_0_2D | DISPLAY_BG0_ACTIVE;
	VRAM_C_CR = VRAM_ENABLE | VRAM_C_SUB_BG;
	BGCTRL_SUB[GB_BG] = BG_COLOR_256 | BG_MAP_BASE(GB_BG_MP_BASE) | BG_TILE_BASE(GB_BG_TILE_BASE) | BG_32x32;

	//BG0 configuration for the background
	BGCTRL[GB_BG] = BG_COLOR_256 | BG_MAP_BASE(GB_BG_MP_BASE) | BG_TILE_BASE(GB_BG_TILE_BASE) | BG_32x32;

    //sprites
	
    //Set up memory bank to work in sprite mode (offset since we are using VRAM A for backgrounds)
	VRAM_B_CR = VRAM_ENABLE | VRAM_B_MAIN_SPRITE_0x06400000;
	//Initialize sprite manager and the engine
	oamInit(&oamMain, SpriteMapping_1D_32, false);

	//Allocate all ship sprites.
	int i,j;
	for (i = 0; i < NUM_SHIPS; i++) {
		ship e_ship = enemy_ships[i];
		ship p_ship = player_ships[i];
		for (j = 0; j < e_ship.len; j ++) {

			allocate_sprite(
				&e_ship.sprite_buffs[j],
				SHIP_PALS[i][j], SHIP_PALLENS[i][j],
				SHIP_TILES[i][j], SHIP_TILELENS[i][j],
				1
			);
		}
		for (j = 0; j < p_ship.len; j++) {
			// allocate_sprite(&p_ship.sprite_buffs[j],
			// 	SHIP_PALS[i][j], SHIP_PALLENS[i][j],
			// 	SHIP_TILES[i][j], SHIP_TILELENS[i][j],
			// 	0
			// );
		}
	}
}
//todo: merge all sprites palletes (from grit) to one sprite pallete.
uint16_t sprite_pallete_memory_usage = 0;
void allocate_sprite(u16 ** gfx, unsigned short * pal, unsigned int palLen, unsigned int * tiles, unsigned int tilesLen, char isMain ) {
	printf("%p\n", tiles);
	OamState * oam = &oamMain;
	u16* sprite_palette = (u16*)SPRITE_PALETTE;
	if (!isMain) {
		oam = &oamSub;
		sprite_palette = (u16*)SPRITE_PALETTE_SUB;
	}
		

	*gfx = oamAllocateGfx(oam, SpriteSize_16x16, SpriteColorFormat_256Color);
	printf("tile: %p\n",*gfx );
	if (*gfx == NULL) {
	
		return;
	}
	
	//memory_usage += (SpriteSize_16x16 & 0xFFF) << 5;
	//Copy data for the graphic (palette and bitmap)

	dmaCopy(pal, sprite_palette, palLen);
	dmaCopy(tiles, *gfx, tilesLen);

	//sprite_pallete_memory_usage += palLen;
	
}

void update_sprites() {
	int i,j;

	for (i =0; i< 1; i ++) {
		// MAIN ENGINE: enemy
		
		ship e_ship = enemy_ships[i];
		for(j = 0; j < e_ship.len; j ++) {
			int x = (GET_X(e_ship.coords[j]) * 16) + 48;
			int y = (GET_Y(e_ship.coords[j]) * 16) + 16;
			//printf("x: %d\n",x);
			oamSet(&oamMain, 	// oam handler
    		(i+1)*j,				// Number of sprite
    		x, y,			// Coordinates
    		0,				// Priority
    		0,				// Palette to use
    		SpriteSize_16x16,			// Sprite size
    		SpriteColorFormat_256Color,	// Color format
    		e_ship.sprite_buffs[j],			// Loaded graphic to display
    		-1,				// Affine rotation to use (-1 none)
    		false,			// Double size if rotating
    		false,			// Hide this sprite
    		false, false,	// Horizontal or vertical flip
    		false			// Mosaic
    		);
		}


		// SUB ENGINE: player
		ship p_ship = player_ships[i];
		for(j = 0; j < p_ship.len; j ++) {

			int x = (GET_X(p_ship.coords[j])*16)+48;
			int y = (GET_Y(p_ship.coords[j])* 16)+ 16;
			
			oamSet(&oamSub, 	// oam handler
    		((i+1)*j)+NUM_SHIPS,				// Number of sprite
    		x, y,			// Coordinates
    		0,				// Priority
    		0,				// Palette to use
    		SpriteSize_16x16,			// Sprite size
    		SpriteColorFormat_256Color,	// Color format
    		p_ship.sprite_buffs[j],			// Loaded graphic to display
    		-1,				// Affine rotation to use (-1 none)
    		false,			// Double size if rotating
    		false,			// Hide thiss sprite
    		false, false,	// Horizontal or vertical flip
    		false			// Mosaic
    		);
		}
		
	}
   	
	
}
