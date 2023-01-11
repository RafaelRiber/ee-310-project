#include <nds.h>
#include "graphics.h"
#include "stdio.h"

#include "test_gameboard.h"
#include "target.h"
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

#include "hit.h"
#include "miss.h"
#include "battleship_title.h"
#include "battleship_title_sub.h"

#include "hosting_waiting_sub.h"


#include "block.h"
#include "battleships.h"
#define GB_BG 0
#define GB_BG_MP_BASE 0 
#define GB_BG_TILE_BASE 1



#define NUM_SHIP_SPRITES (CARRIER_SIZE + BATTLESHIP_SIZE + CRUISER_SIZE + SUBMARINE_SIZE + DESTROYER_SIZE)
#define SHIP_SPRITE_IDX(i) (i + 1)
#define HIT_MISS_SPRITE_IDX(i) (1 + NUM_SHIP_SPRITES + i)
#define TARGET_SPRITE_IDX(i) (i)


unsigned int *BATTLESHIP_SPRITE_TILES[BATTLESHIP_SIZE] = {battleship_sprite_0Tiles,battleship_sprite_1Tiles,battleship_sprite_2Tiles,battleship_sprite_3Tiles};
unsigned int *CRUISER_SPRITE_TILES[CRUISER_SIZE] = {cruiser_sprite_0Tiles, cruiser_sprite_1Tiles, cruiser_sprite_2Tiles};
unsigned int *SUBMARINE_SPRITE_TILES[SUBMARINE_SIZE] = {submarine_sprite_0Tiles,submarine_sprite_1Tiles,submarine_sprite_2Tiles};
unsigned int *DESTROYER_SPRITE_TILES[DESTROYER_SIZE] = {destroyer_sprite_0Tiles, destroyer_sprite_1Tiles};
unsigned int *CARRIER_SPRITE_TILES[CARRIER_SIZE] = {carrier_sprite_0Tiles, carrier_sprite_1Tiles, carrier_sprite_2Tiles, carrier_sprite_3Tiles, carrier_sprite_4Tiles};
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
unsigned int CARRIER_SPRITE_TILESLEN[CARRIER_SIZE] = {carrier_sprite_0TilesLen, carrier_sprite_1TilesLen, carrier_sprite_2TilesLen, carrier_sprite_3TilesLen,  carrier_sprite_4TilesLen};

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
unsigned short *CARRIER_SPRITE_PAL[CARRIER_SIZE] = {carrier_sprite_0Pal, carrier_sprite_1Pal, carrier_sprite_2Pal, carrier_sprite_3Pal, carrier_sprite_4Pal};
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
unsigned int CARRIER_SPRITE_PALLEN[CARRIER_SIZE] = {carrier_sprite_0PalLen, carrier_sprite_1PalLen, carrier_sprite_2PalLen, carrier_sprite_3PalLen,  carrier_sprite_4PalLen};
unsigned int *SHIP_PALLENS[NUM_SHIPS] = {
	[BATTLESHIP] = BATTLESHIP_SPRITE_PALLEN,
	[CRUISER] = CRUISER_SPRITE_PALLEN,
	[SUBMARINE]= SUBMARINE_SPRITE_PALLEN,
	[DESTROYER]= DESTROYER_SPRITE_PALLEN,
	[CARRIER] =CARRIER_SPRITE_PALLEN,
};

uint8 num_hits_misses; 
uint8 num_ship_sprites;
// gfx's for hits and misses.
u16 * hits_misses_sprites[BRD_LEN*BRD_LEN];
/*
main engine: mode 0 
    - bg0: gameboard
    - bgx: effects (explosions..)
    - Sprites: ships
*/

struct background {
	const unsigned int * tiles;
	int tilesLen;
	const unsigned short * pal;
	int palLen;
	const unsigned short * map;
	int mapLen;
};

struct background main_backgrounds[NUM_SCREENS] = {
	[MAIN_MENU] = {
	battleship_titleTiles,
	battleship_titleTilesLen,
	battleship_titlePal,
	battleship_titlePalLen,
	battleship_titleMap,
	battleship_titleMapLen
	},

	[SHIP_PLACE] = {
	test_gameboardTiles,
	test_gameboardTilesLen,
	test_gameboardPal,
	test_gameboardPalLen,
	test_gameboardMap,
	test_gameboardMapLen
	},
	[GAME] = {
	test_gameboardTiles,
	test_gameboardTilesLen,
	test_gameboardPal,
	test_gameboardPalLen,
	test_gameboardMap,
	test_gameboardMapLen
	},
};

struct background sub_backgrounds[NUM_SCREENS] = {
	[MAIN_MENU] = {
	battleship_title_subTiles,
	battleship_title_subTilesLen,
	battleship_title_subPal,
	battleship_title_subPalLen,
	battleship_title_subMap,
	battleship_title_subMapLen
	},

	[HOST_WAIT] = {
	hosting_waiting_subTiles,
	hosting_waiting_subTilesLen,
	hosting_waiting_subPal,
	hosting_waiting_subPalLen,
	hosting_waiting_subMap,
	hosting_waiting_subMapLen
	},

	[SHIP_PLACE] = {
	test_gameboardTiles,
	test_gameboardTilesLen,
	test_gameboardPal,
	test_gameboardPalLen,
	test_gameboardMap,
	test_gameboardMapLen
	},
	[GAME] = {
	test_gameboardTiles,
	test_gameboardTilesLen,
	test_gameboardPal,
	test_gameboardPalLen,
	test_gameboardMap,
	test_gameboardMapLen
	},
};






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


	//SUB ENGINE

	REG_DISPCNT_SUB = MODE_0_2D | DISPLAY_BG0_ACTIVE;
	VRAM_C_CR = VRAM_ENABLE | VRAM_C_SUB_BG;
	//BG0 configuration for the background
	BGCTRL_SUB[GB_BG] = BG_COLOR_256 | BG_MAP_BASE(GB_BG_MP_BASE) | BG_TILE_BASE(GB_BG_TILE_BASE) | BG_32x32;
	
	//sprites
    //Set up memory bank to work in sprite mode
	VRAM_D_CR = VRAM_ENABLE | VRAM_D_SUB_SPRITE;
	//Initialize sprite manager and the engine
	oamInit(&oamSub, SpriteMapping_1D_32, false);

	//target sprite
	allocate_sprite(player_target.sprite_buff, targetPal, targetPalLen, targetTiles, targetTilesLen, 1);
	
	
	oamRotateScale(&oamMain, 0, degreesToAngle(-90), intToFixed(1, 8), intToFixed(1, 8));
	oamRotateScale(&oamSub, 0, degreesToAngle(-90), intToFixed(1, 8), intToFixed(1, 8));
	
	//Allocate all ship sprites.
	int i,j; 
	for (i = 0; i < 1; i++) {
		ship * p_ship = player_ships + i;
		//ship p_ship = player_ships[i];
		for (j = 0; j < p_ship->len; j ++) {
				
			allocate_sprite( 
				p_ship->sprite_buffs + j, 
				SHIP_PALS[i][j], SHIP_PALLENS[i][j],
				SHIP_TILES[i][j], SHIP_TILELENS[i][j],
				0
			);
			
		}
	}

	
	num_hits_misses = 0;
	memset(hits_misses_sprites, 0, sizeof(u16 *) * BRD_LEN * BRD_LEN);
}

void load_backgrounds(int screen) {
	if (screen > NUM_SCREENS || screen < 0)
		return;

	if (screen != HOST_WAIT) {
		swiCopy(main_backgrounds[screen].tiles, BG_TILE_RAM(GB_BG_TILE_BASE), main_backgrounds[screen].tilesLen/2);
		swiCopy(main_backgrounds[screen].pal, BG_PALETTE, main_backgrounds[screen].palLen/2);
		swiCopy(main_backgrounds[screen].map, BG_MAP_RAM(GB_BG_MP_BASE), main_backgrounds[screen].mapLen/2);
	}
		
	swiCopy(sub_backgrounds[screen].tiles, BG_TILE_RAM_SUB(GB_BG_TILE_BASE), sub_backgrounds[screen].tilesLen/2);
	swiCopy(sub_backgrounds[screen].pal, BG_PALETTE_SUB, sub_backgrounds[screen].palLen/2);
	swiCopy(sub_backgrounds[screen].map, BG_MAP_RAM_SUB(GB_BG_MP_BASE),sub_backgrounds[screen].mapLen/2);
}

// put a hit or miss on the enemey board.
// (MAIN ENGINE)
void new_shot_sprite(int isHit, int x, int y){
	OamState * oam = &oamMain;
	u16 * gfx = oamAllocateGfx(oam, SpriteSize_16x16, SpriteColorFormat_256Color);
	num_hits_misses += 1;
	hits_misses_sprites[num_hits_misses] = gfx;
	if (isHit) {
		dmaCopy(hitTiles, gfx,hitTilesLen);
		
	}
	else {
		dmaCopy(missTiles, gfx,missTilesLen);
	}

	int sx = (x * 16) + 48;
	int sy = (y * 16) + 16;

	oamSet(&oamMain, 	// oam handler
		HIT_MISS_SPRITE_IDX(num_hits_misses),				// Number of sprite
		sx, sy,			// Coordinates
		0,				// Priority
		0,				// Palette to use
		SpriteSize_16x16,			// Sprite size
		SpriteColorFormat_256Color,	// Color format
		gfx,			// Loaded graphic to display
		-1,				// Affine rotation to use (-1 none)
		false,			// Double size if rotating
		false,			// Hide this sprite
		false, false,	// Horizontal or vertical flip
		false			// Mosaic
	);
	

}

void clear_shots() {
	oamClear (&oamMain, num_ship_sprites, num_ship_sprites + num_hits_misses);
}
//todo: merge all sprites palletes (from grit) to one sprite pallete.
uint16_t sprite_pallete_memory_usage = 0;
void allocate_sprite(u16 ** gfx, unsigned short * pal, unsigned int palLen, unsigned int * tiles, unsigned int tilesLen, char isMain ) {

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

void update_ships() {
	int i,j;


	//MAIN ENGINE: TARGET
	oamSet(&oamMain, 
		TARGET_SPRITE_IDX(0), 
		(GET_X(player_target.coords) * 16) + 48,
		(GET_Y(player_target.coords) * 16) + 16,
		0,
		0, 
		SpriteSize_16x16,			// Sprite size
		SpriteColorFormat_256Color,	// Color format
		player_target.sprite_buff,			// Loaded graphic to display
		-1,				// Affine rotation to use (-1 none)
		false,			// Double size if rotating
		player_target.is_hidden,			// Hide this sprite
		false, false,	// Horizontal or vertical flip
		false			// Mosaic
	);
	
	int is_horizontal;
	//SUB ENGINE: PLAYER SHIPS
	for (i =0; i< 1; i ++) {
		
		ship *p_ship = player_ships + i;
		if (GET_X(p_ship->coords[0]) < GET_X(p_ship->coords[1]))
			is_horizontal = -1;
		else
			is_horizontal = 0;

		//printf("pp %p\n",*(e_ship.sprite_buffs + j));
		for(j = 0; j < p_ship->len; j ++) {
			int x = (GET_X(p_ship->coords[j]) * 16) + 48;
			int y = (GET_Y(p_ship->coords[j]) * 16) + 16;
	
			oamSet(&oamSub, 	// oam handler
				SHIP_SPRITE_IDX((i+1)*j),				// Number of sprite
				x, y,			// Coordinates
				0,				// Priority
				0,				// Palette to use
				SpriteSize_16x16,			// Sprite size
				SpriteColorFormat_256Color,	// Color format
				*(p_ship->sprite_buffs + j),			// Loaded graphic to display
				is_horizontal,				// Affine rotation to use (-1 none)
				false,			// Double size if rotating
				p_ship->is_hidden,			// Hide this sprite
				false, false,	// Horizontal or vertical flip
				false			// Mosaic
    		);
			

			
		}


		
	}
   	
	
}
